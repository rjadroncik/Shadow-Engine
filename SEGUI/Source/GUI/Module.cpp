#include "Module.h"

#include "../../../SECore/Source/Resources/Manager.h"
#include "../../../SECore/Source/System/Settings.h"
#include "../../../SECore/Source/System/Scripting.h"

#include "../Resources/Cursor.h"
#include "Window.h"
#include "EditField.h"

#include <SCFWinAPIUndef.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace GUI;
using namespace System;
using namespace Resources;

CModule::CModule()
{

}

CModule::CCursor::CCursor()
{
	m_bShow = TRUE;

	m_Position[0] = 0;
	m_Position[1] = 0;

	m_PositionPrecise[0] = 0;
	m_PositionPrecise[1] = 0;

	m_pCursor = NULL;

	m_Bounds[0] = 0;
	m_Bounds[1] = 0;
	m_Bounds[2] = (float)CSettings::CDisplay::Width();
	m_Bounds[3] = (float)CSettings::CDisplay::Height();

	for (UINT i = 0; i < CursorTypeCount; i++) { m_pCursors[i] = NULL; }
}

CModule::CCursor::~CCursor()
{

}

CModule::~CModule()
{

}

bool CModule::CCursor::Show()               _GET { return m_bShow; }
void CModule::CCursor::Show(_IN bool bShow) _SET { m_bShow = bShow; ShowCursor(bShow); }

void CModule::CCursor::Use(_IN UINT uiIndex) _SET 
{
	m_pCursor = m_pCursors[uiIndex];
	if (m_pCursor) { SetCursor((HCURSOR)m_pCursor); }
}

void CModule::CCursor::Enlist(_IN ENUM eType, _IN Resources::CCursor* pCursor) _SET
{
	switch (eType)
	{
	case CursorArrow:    { m_pCursors[eType] = LoadCursor(NULL, IDC_ARROW);    break; }
	case CursorSizeNESW: { m_pCursors[eType] = LoadCursor(NULL, IDC_SIZENESW); break; }
	case CursorSizeNWSE: { m_pCursors[eType] = LoadCursor(NULL, IDC_SIZENWSE); break; }
	case CursorSizeWE:   { m_pCursors[eType] = LoadCursor(NULL, IDC_SIZEWE);   break; }
	case CursorSizeNS:   { m_pCursors[eType] = LoadCursor(NULL, IDC_SIZENS);   break; }
	}

	//m_pCursors[eType] = pCursor;
}

void CModule::CCursor::Position(_IN float fX, _IN float fY)
{
	m_PositionPrecise[0] = fX; 
	m_PositionPrecise[1] = fY; 

	m_Position[0] = (float)(int)(m_PositionPrecise[0]);
	m_Position[1] = (float)(int)(m_PositionPrecise[1]);

	if (m_Position[0] < m_Bounds[0]) { m_PositionPrecise[0] = m_Bounds[0]; m_Position[0] = m_Bounds[0]; }
	if (m_Position[1] < m_Bounds[1]) { m_PositionPrecise[1] = m_Bounds[1]; m_Position[1] = m_Bounds[1]; }

	if (m_Position[0] >= m_Bounds[2]) { m_PositionPrecise[0] = m_Bounds[2]; m_Position[0] = m_Bounds[2]; }
	if (m_Position[1] >= m_Bounds[3]) { m_PositionPrecise[1] = m_Bounds[3]; m_Position[1] = m_Bounds[3]; }
}

void CModule::CCursor::Move(_IN float fX, _IN float fY)
{
	m_PositionPrecise[0] += fX; 
	m_PositionPrecise[1] += fY; 

	m_Position[0] = (float)(int)(m_PositionPrecise[0]);
	m_Position[1] = (float)(int)(m_PositionPrecise[1]);
	
	if (m_Position[0] < m_Bounds[0]) { m_PositionPrecise[0] = m_Bounds[0]; m_Position[0] = m_Bounds[0]; }
	if (m_Position[1] < m_Bounds[1]) { m_PositionPrecise[1] = m_Bounds[1]; m_Position[1] = m_Bounds[1]; }

	if (m_Position[0] >= m_Bounds[2]) { m_PositionPrecise[0] = m_Bounds[2]; m_Position[0] = m_Bounds[2]; }
	if (m_Position[1] >= m_Bounds[3]) { m_PositionPrecise[1] = m_Bounds[3]; m_Position[1] = m_Bounds[3]; }
}


CWindowAbstract* CModule::MouseOver(_IN Float2& rMousePos, _OPT _OUT Float2* pOutLocalPosition) _GET
{
	for (int i = m_Windows.Size() - 1; i >= 0; i--)
	{				   
		CWindowAbstract* pWindow = ((CWindowAbstract&)m_Windows[i]).MouseOver(rMousePos, pOutLocalPosition);

		if (pWindow) { return pWindow; }
	}

	return NULL;
}

bool CModule::MouseLocalPos(_IN Float2& rMousePos, _IN CWindowAbstract& rElement, _OUT Float2& rOutLocalPosition) _GET
{
	for (int i = m_Windows.Size() - 1; i >= 0; i--)
	{				   
		if (((CWindowAbstract&)m_Windows[i]).MouseLocalPos(rMousePos, rElement, rOutLocalPosition)) { return TRUE; }
	}					 

	return FALSE;
}

void CModule::ChildMakeTopmost(_IN CWindowAbstract& rChild) _SET
{
	for (UINT i = 0; i < (m_Windows.Size() - 1); i++)
	{
		if (&m_Windows[i] == &rChild)
		{
			m_Windows.Swap(i, i + 1);
		}
	}
}

void CModule::Render()
{
	if (!m_Windows.Size()) { return; }

	Float4 ScreenRect = { 0.0f, 0.0f, (float)CSettings::CDisplay::Width(), (float)CSettings::CDisplay::Height() };

	ScissorPush(ScreenRect);
	glEnable(GL_BLEND);
	PositionPush();
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		for (UINT i = 0; i < m_Windows.Size(); i++)
		{
			((CWindowAbstract&)m_Windows[i]).Render();
		}

		//if (Cursor.m_bShow && Cursor.m_pCursor) { Cursor.m_pCursor->RenderAt(Cursor.m_Position); }
	}
	PositionPop();
	glDisable(GL_BLEND);
	ScissorPop();
}

void CModule::LayOut() 
{
	Cursor.BoundsRect(m_Rect);
}

void CModule::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
}

void CModule::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CString* pValue = NULL;	

	//Cursors
	rReader.GetBlock();
	{
		UINT uiCount = 0;
		GETVALUE { uiCount = CInt(*pValue).Value(); }

		for (UINT i = 0; i < uiCount; i++)
		{
			rReader.GetBlock();

			GETVALUE 
			{
				Resources::CCursor* pCursor = (Resources::CCursor*)CScripting::Object(*pValue);

				GETVALUE 
				{ 
					ENUM eCursorType = CXMLEnumeration::Translate(STRING("CursorTypes"), *pValue)->Value();
					Cursor.Enlist(eCursorType, pCursor);
				}
			}
		}
	}

	//Windows
	rReader.GetBlock();
	{
		UINT uiCount = 0;
		GETVALUE { uiCount = CInt(*pValue).Value(); }

		for (UINT i = 0; i < uiCount; i++)
		{
			rReader.GetBlock();
			GETVALUE { ChildAdd(*(CWindow*)CScripting::Object(*pValue)); }
		}
	}

	Cursor.Use(CursorArrow);
}