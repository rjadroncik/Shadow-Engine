#include "Window.h"

#include "Layout.h"
#include "Module.h"
#include "../Resources/Font.h"

#include "../../../SECore/Source/System/EventLog.h"
#include "../../../SECore/Source/System/Settings.h"
#include "../../../SECore/Source/System/Scripting.h"

using namespace GUI;
using namespace System;
using namespace Resources;

void CWindow::OnWindowPaintButtonClose(_INOUT CWindowAbstract& rButton)
{
	char cOpacityOffset = rButton.Enabled() ? 0 : -48;

	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_QUADS);
	{
		glColor4ub(128, 128, 128, 64 + cOpacityOffset);
		glVertex2f(1,                     rButton.Size()[1] - 1);
		glVertex2f(rButton.Size()[0] - 1, rButton.Size()[1] - 1);
		glVertex2f(rButton.Size()[0] - 1, 1);
		glVertex2f(1,                     1);

		glColor4ub(255, 255, 255, 196 + cOpacityOffset);
		glVertex2f(4,                     rButton.Size()[1] - 3);
		glVertex2f(rButton.Size()[0] - 3, 4);
		glVertex2f(rButton.Size()[0] - 4, 3);
		glVertex2f(3,                     rButton.Size()[1] - 4);

		glVertex2f(3,                     4);
		glVertex2f(rButton.Size()[0] - 4, rButton.Size()[1] - 3);
		glVertex2f(rButton.Size()[0] - 3, rButton.Size()[1] - 4);
		glVertex2f(4,                     3);
	}
	glEnd();
}

void CWindow::OnWindowPaintButtonMin(_INOUT CWindowAbstract& rButton)
{
	char cOpacityOffset = rButton.Enabled() ? 0 : -48;

	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_QUADS);
	{
		glColor4ub(128, 128, 128, 64 + cOpacityOffset);
		glVertex2f(1,                     rButton.Size()[1] - 1);
		glVertex2f(rButton.Size()[0] - 1, rButton.Size()[1] - 1);
		glVertex2f(rButton.Size()[0] - 1, 1);
		glVertex2f(1,                     1);

		glColor4ub(255, 255, 255, 196 + cOpacityOffset);
		glVertex2f(3,                     rButton.Size()[1] - 2);
		glVertex2f(rButton.Size()[0] - 4, rButton.Size()[1] - 2);
		glVertex2f(rButton.Size()[0] - 4, rButton.Size()[1] - 4);
		glVertex2f(3,                     rButton.Size()[1] - 4);
	}
	glEnd();
}

void CWindow::OnWindowPaintButtonMax(_INOUT CWindowAbstract& rButton)
{
	char cOpacityOffset = rButton.Enabled() ? 0 : -48;

	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_QUADS);
	{
		glColor4ub(128, 128, 128, 64 + cOpacityOffset);
		glVertex2f(1,                     rButton.Size()[1] - 1);
		glVertex2f(rButton.Size()[0] - 1, rButton.Size()[1] - 1);
		glVertex2f(rButton.Size()[0] - 1, 1);
		glVertex2f(1,                     1);

		glColor4ub(255, 255, 255, 196 + cOpacityOffset);
		glVertex2f(3,                     rButton.Size()[1] - 3);
		glVertex2f(rButton.Size()[0] - 3, rButton.Size()[1] - 3);
		glVertex2f(rButton.Size()[0] - 3, 3);
		glVertex2f(3,                     3);

		glColor4ub(64, 64, 64, 196 + cOpacityOffset);
		glVertex2f(4,                     rButton.Size()[1] - 4);
		glVertex2f(rButton.Size()[0] - 4, rButton.Size()[1] - 4);
		glVertex2f(rButton.Size()[0] - 4, 5);
		glVertex2f(4,                     5);
	}
	glEnd();
}

void Window_OnActionClickedButtonClose(_INOUT CButtonClick& rButton)
{
	((CWindow*)rButton.Parent())->OnWindowClose();
}

CWindow::CWindow()
{
	m_fBorder = 4;

	ZeroVector4(m_RectClient);
	m_fButtonSize = 0.0f;

	m_pButtonMin   = new CButtonClick();
	m_pButtonMax   = new CButtonClick();
	m_pButtonClose = new CButtonClick();

	m_pButtonMin->Parent(*this);
	m_pButtonMax->Parent(*this);
	m_pButtonClose->Parent(*this);

	m_pButtonMin->OnWindowPaint(OnWindowPaintButtonMin);
	m_pButtonMax->OnWindowPaint(OnWindowPaintButtonMax);
	m_pButtonClose->OnWindowPaint(OnWindowPaintButtonClose);

	Float4 Color = { 1.0f, 0.45f, 0.25f, 0.2f };
	m_pButtonMin->ColorBackground(Color);
	m_pButtonMax->ColorBackground(Color);
	m_pButtonClose->ColorBackground(Color);

 	m_pButtonClose->OnActionClick(Window_OnActionClickedButtonClose);

	//Remove system menu buttons from the child list
	this->ChildRemove(*m_pButtonMin);
	this->ChildRemove(*m_pButtonMax);
	this->ChildRemove(*m_pButtonClose);

	OnWindowOpen();
}

CWindow::~CWindow()
{
	delete m_pButtonClose;
	delete m_pButtonMax;
	delete m_pButtonMin;
}

bool CWindow::CanStartDragging(_IN Float2& rMousePos)
{
	return (rMousePos[1] <= m_RectClient[1]);
}

DWORD CWindow::CanStartSizing(_IN Float2& rMousePos)
{
	DWORD dwResult = 0;

	if (rMousePos[0] <= (int)m_fBorder) { dwResult |= AlignLeft; }
	if (rMousePos[1] <= (int)m_fBorder) { dwResult |= AlignTop; }

	if (rMousePos[0] >= (m_Rect[2] - (int)m_fBorder)) { dwResult |= AlignRight; }
	if (rMousePos[1] >= (m_Rect[3] - (int)m_fBorder)) { dwResult |= AlignBottom; }

	return dwResult;
}

CWindowAbstract* CWindow::MouseOver(_IN Float2& rMousePos, _OPT _OUT Float2* pOutLocalPosition) _GET
{
	if (!this->CWindowAbstract::MouseOver(rMousePos)) { return NULL; }

	Float2 LocalMousePos;
	SubtractVectors2(LocalMousePos, rMousePos, (Float2&)m_Rect);

	CWindowAbstract* pWindow = NULL;

	if ((pWindow = m_pButtonMin->MouseOver  (LocalMousePos, pOutLocalPosition)) != 0) { return pWindow; }
	if ((pWindow = m_pButtonMax->MouseOver  (LocalMousePos, pOutLocalPosition)) != 0) { return pWindow; }
	if ((pWindow = m_pButtonClose->MouseOver(LocalMousePos, pOutLocalPosition)) != 0) { return pWindow; }

	SubtractVectors2(LocalMousePos, LocalMousePos, (Float2&)m_RectClient);
	{
		for (UINT i = 0; i < m_Windows.Size(); i++)
		{				   
			pWindow = ((CWindowAbstract&)m_Windows[i]).MouseOver(LocalMousePos, pOutLocalPosition);

			if (pWindow) { return pWindow; }
		}
	}
	AddVectors2(LocalMousePos, LocalMousePos, (Float2&)m_RectClient);

	if (pOutLocalPosition) { CopyVector2(*pOutLocalPosition, LocalMousePos); }

	return (CWindowAbstract*)this;
}

bool CWindow::MouseLocalPos(_IN Float2& rMousePos, _IN CWindowAbstract& rWindow, _OUT Float2& rOutLocalPosition) _GET
{
	if (this->CWindowAbstract::MouseLocalPos(rMousePos, rWindow, rOutLocalPosition)) { return TRUE; }

	Float2 LocalMousePos;
	SubtractVectors2(LocalMousePos, rMousePos,     (Float2&)m_Rect);
	
	if (m_pButtonMin->MouseLocalPos(LocalMousePos, rWindow, rOutLocalPosition)) { return TRUE; }
	if (m_pButtonMax->MouseLocalPos(LocalMousePos, rWindow, rOutLocalPosition)) { return TRUE; }
	if (m_pButtonClose->MouseLocalPos(LocalMousePos, rWindow, rOutLocalPosition)) { return TRUE; }

	SubtractVectors2(LocalMousePos, LocalMousePos, (Float2&)m_RectClient);

	for (UINT i = 0; i < m_Windows.Size(); i++)
	{				   
		if (((CWindowAbstract&)m_Windows[i]).MouseLocalPos(LocalMousePos, rWindow, rOutLocalPosition)) { return TRUE; }
	}					 

	return FALSE;
}

void CWindow::RenderNC()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_QUADS);
	{
		glColor4f(0.5f, 0.5f, 0.5f, 0.6f);
		glVertex2f(m_RectClient[0],                   m_RectClient[1] + m_RectClient[3]);
		glVertex2f(m_RectClient[0] + m_RectClient[2], m_RectClient[1] + m_RectClient[3]);
		glVertex2f(m_RectClient[0] + m_RectClient[2], m_RectClient[1]);
		glVertex2f(m_RectClient[0],                   m_RectClient[1]);

		glColor4f(0.3f, 0.3f, 0.3f, 0.6f);
		glVertex2f(m_fBorder,                   m_RectClient[1]);
		glVertex2f(m_fBorder + m_RectClient[2], m_RectClient[1]);
		glVertex2f(m_fBorder + m_RectClient[2], m_fBorder);
		glVertex2f(m_fBorder,                   m_fBorder);
	}
	glEnd();

	this->RenderNCButtons();
	m_Text.Render();
}

void CWindow::OnMouseMove(_IN SEventMouse& rEvent)
{
	if (m_fpOnMouseMove) { m_fpOnMouseMove(*this, rEvent); } 
}		

void CWindow::RenderNCButtons()
{
	m_pButtonMin->Render();
	m_pButtonMax->Render();
	m_pButtonClose->Render();
}

void CWindow::Render()
{
	glBindTexture(GL_TEXTURE_2D, 0);

	ScissorPush(m_Rect);
	PositionPush();
	{
		Translate(m_Rect[0], m_Rect[1]);
	
		this->RenderBackground();
		this->RenderNC();

		ScissorPush(m_RectClient);
		{
			Translate(m_RectClient[0], m_RectClient[1]);

			for (UINT i = 0; i < m_Windows.Size(); i++)
			{
				((CWindowAbstract&)m_Windows[i]).Render();
			}
		}
		ScissorPop();

		this->OnWindowPaint();
	}
	PositionPop();
	ScissorPop();
}

float CWindow::ButtonsWidth()
{
	float fWidth = 0;

	if (m_pButtonClose->Enabled())                          { fWidth = m_fButtonSize; }
	if (m_pButtonMin->Enabled() || m_pButtonMax->Enabled()) { fWidth += m_fButtonSize * 2; }

	return fWidth;
}

void CWindow::LayOut()
{
	m_fButtonSize = m_fBorder + m_Text.Size()[1];
 
	Float4 LabelLayoutRect = 
	{ 
		0.0f,
		m_fBorder, 
		m_Rect[2] - ButtonsWidth(), 
		m_Text.Size()[1] 
	};

	CWindowAbstract::TextLayOut(m_Text, LabelLayoutRect, AlignCenter);

	m_RectClient[0] = m_fBorder; 
	m_RectClient[1] = m_fBorder + m_fButtonSize; 
	m_RectClient[2] = m_Rect[2] - m_fBorder * 2; 
	m_RectClient[3] = m_Rect[3] - m_fBorder * 2 - m_fButtonSize; 

	float fSize = m_RectClient[1] - m_fBorder;

 	Float4 RectButton = 
	{ 
		m_Rect[2] - fSize - m_fBorder + 1,
		m_fBorder + 1,
		fSize - 2,
		fSize - 2 
	};

	if (m_pButtonClose->Enabled())
	{
		m_pButtonClose->Rect(RectButton);

		if (m_pButtonMin->Enabled() || m_pButtonMax->Enabled())
		{
			RectButton[0] -= fSize;
			m_pButtonMax->Rect(RectButton);

			RectButton[0] -= fSize;
			m_pButtonMin->Rect(RectButton);
		}
		else
		{
			m_pButtonMax->Size(0, 0);
			m_pButtonMin->Size(0, 0);
		}
	}
	else
	{
		m_pButtonClose->Size(0, 0);

		if (m_pButtonMin->Enabled() || m_pButtonMax->Enabled())
		{
			m_pButtonMax->Rect(RectButton);

			RectButton[0] -= fSize;
			m_pButtonMin->Rect(RectButton);
		}
		else
		{
			m_pButtonMax->Size(0, 0);
			m_pButtonMin->Size(0, 0);
		}
	}

	if (m_pLayout)
	{
		_PENDING;
		Float4 LayoutRect = { 0, 0, m_RectClient[2], m_RectClient[3] };
		Float2 LayoutSize;

		m_pLayout->LayOut((CVector<CElement>&)m_Windows, LayoutRect, LayoutSize);

		if (LayoutSize[0] > m_RectClient[2])
		{
			m_Rect[2] = LayoutSize[0] + m_fBorder * 2;
		}
			
		if (LayoutSize[1] > m_RectClient[3])
		{
			m_Rect[3] = LayoutSize[1] + m_fBorder * 2 + m_fButtonSize;
		}
	}
	else
	{
		for (UINT i = 0; i < m_Windows.Size(); i++)
		{
			((CWindowAbstract&)m_Windows[i]).LayOut();
		}
	}
}

void CWindow::OnWindowClose()
{
	((CModule*)this->Parent())->ChildRemove(*this);

/*	m_Rect[2] = 0;
	m_Rect[3] = 0;

	this->OnWindowSize();*/
}

void CWindow::OnWindowMove()
{
	if (m_fpOnWindowMove) { m_fpOnWindowMove(*this); } 

	if (m_Rect[0] < 0) { m_Rect[0] = 0; }
	if (m_Rect[1] < 0) { m_Rect[1] = 0; }
		
	if ((m_Rect[0] + m_Rect[2]) > (int)CSettings::CDisplay::Width())  { m_Rect[0] = (int)CSettings::CDisplay::Width()  - m_Rect[2]; }
	if ((m_Rect[1] + m_Rect[3]) > (int)CSettings::CDisplay::Height()) { m_Rect[1] = (int)CSettings::CDisplay::Height() - m_Rect[3]; }
}

void CWindow::OnWindowSize()
{
	if (m_fpOnWindowSize) { m_fpOnWindowSize(*this); } 

	Float2 MinSize =	
	{
		(float)(2 * m_fBorder + m_Text.Size()[0] + (m_fButtonSize * 3)),
		(float)(2 * m_fBorder                    +  m_fButtonSize)
	};

	if (m_Rect[2] < MinSize[0]) { m_Rect[2] = MinSize[0]; }
	if (m_Rect[3] < MinSize[1]) { m_Rect[3] = MinSize[1]; }

	if ((m_Rect[0] + m_Rect[2]) > (int)CSettings::CDisplay::Width())  { m_Rect[0] = (int)CSettings::CDisplay::Width()  - m_Rect[2]; }
	if ((m_Rect[1] + m_Rect[3]) > (int)CSettings::CDisplay::Height()) { m_Rect[1] = (int)CSettings::CDisplay::Height() - m_Rect[3]; }

	this->LayOut();
}

void CWindow::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
}

void CWindow::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CElement::XMLDeserialize(rReader);

	CString* pValue = NULL;

	GETVALUE { Resizable(CBool(*pValue).Value()); }

	GETVALUE { ButtonMin().Enabled  (CBool(*pValue).Value()); }
	GETVALUE { ButtonMax().Enabled  (CBool(*pValue).Value()); }
	GETVALUE { ButtonClose().Enabled(CBool(*pValue).Value()); }

	//Elements
	rReader.GetBlock();
	{
		UINT uiCount = 0;
		GETVALUE { uiCount = CInt(*pValue).Value(); }

		for (UINT i = 0; i < uiCount; i++)
		{
			rReader.GetBlock();
			GETVALUE { ChildAdd(*(CWindowAbstract*)CScripting::Object(*pValue)); }
		}
	}

	//Text
	rReader.GetBlock();
	{
		GETVALUE { Text().Font(*(CFont*)CScripting::Object(*pValue)); }

		//Skip text alignment for the moment
		rReader.GetValue();

		GETVALUE { Text().Color(CFloat4(*pValue).Value()); }

		//Text data
		//rReader.GetBlock();
		//Text().String(*rReader.GetBlockData());

		//Text data
		Text().String(*rReader.GetBlockValue());
	}

	//Layout
	rReader.GetBlock();
	GETVALUE { Layout((CLayout*)CScripting::Object(*pValue)); }

	LayOut();
}
