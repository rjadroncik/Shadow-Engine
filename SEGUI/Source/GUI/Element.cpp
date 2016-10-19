#include "Element.h"

#include "../../../SECore/Source/System/Settings.h"

using namespace GUI;
using namespace System;

Float2 CElement::s_Positions[64] = { 0 };
UINT   CElement::s_uiPositionCount = 0;

Int4 CElement::s_ScissorRects[64] = { 0 };
UINT CElement::s_uiScissorRectCount = 0;

CElement::CElement()
{
	ZeroVector4(m_Rect);

	SetVector2(m_SizeMin, 0, 0);
	SetVector2(m_SizeMax, 2048, 2048);
	SetVector2(m_SizeIdeal, 0, 0);
}

CElement::~CElement()
{

}

void CElement::PositionPush()
{
	s_uiPositionCount++;
	CopyVector2(s_Positions[s_uiPositionCount], s_Positions[s_uiPositionCount - 1]); 
	glPushMatrix();
}

void CElement::PositionPop()
{
	glPopMatrix();
	s_uiPositionCount--;
}

void CElement::ScissorPush(_IN Float4& rRect)
{
	s_uiScissorRectCount++;
	s_ScissorRects[s_uiScissorRectCount][0] = (int)rRect[0]; 
	s_ScissorRects[s_uiScissorRectCount][1] = (int)rRect[1]; 
	s_ScissorRects[s_uiScissorRectCount][2] = (int)rRect[2]; 
	s_ScissorRects[s_uiScissorRectCount][3] = (int)rRect[3]; 

	s_ScissorRects[s_uiScissorRectCount][0] += (int)PositionTop()[0];
	s_ScissorRects[s_uiScissorRectCount][1] = (CSettings::CDisplay::Height() - ((UINT)(PositionTop()[1]) + s_ScissorRects[s_uiScissorRectCount][1] + s_ScissorRects[s_uiScissorRectCount][3]));

	glScissor(s_ScissorRects[s_uiScissorRectCount][0], s_ScissorRects[s_uiScissorRectCount][1], s_ScissorRects[s_uiScissorRectCount][2], s_ScissorRects[s_uiScissorRectCount][3]);
}

void CElement::ScissorPop()
{
	s_uiScissorRectCount--;
	glScissor(s_ScissorRects[s_uiScissorRectCount][0], s_ScissorRects[s_uiScissorRectCount][1], s_ScissorRects[s_uiScissorRectCount][2], s_ScissorRects[s_uiScissorRectCount][3]);
}

void CElement::EnforceMinSize()
{
	if (m_Rect[2] < m_SizeMin[0]) { m_Rect[2] = m_SizeMin[0]; }
	if (m_Rect[3] < m_SizeMin[1]) { m_Rect[3] = m_SizeMin[1]; }
}

void CElement::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
}

void CElement::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CString* pValue = NULL;

	GETVALUE { Rect   (CFloat4(*pValue).Value()); }
	GETVALUE { SizeMin(CFloat2(*pValue).Value()); }
	GETVALUE { SizeMax(CFloat2(*pValue).Value()); }
}