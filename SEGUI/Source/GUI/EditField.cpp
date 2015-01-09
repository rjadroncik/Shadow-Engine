#include "EditField.h"

#include "../../../SECore/Source/System/HIDState.h"
#include "../../../SECore/Source/System/VirtualKeyCodes.h"

#include "../../../SECore/Source/Resources/DataPool.h"
#include "../../../SECore/Source/System/Scripting.h"

#include "Dispatcher.h"

using namespace GUI;
using namespace System;
using namespace Resources;

Float2 CEditField::s_MousePositionStart;
Float2 CEditField::s_MousePositionEnd;

int CEditField::s_iCarretIndexStart;

CEditField::CEditField()
{
	m_dwTextAlignment = AlignCenter;

	ZeroVector2(m_Selection);
	ZeroVector4(m_SelectionRect);

	m_iCaretIndex = 0;
	m_fpOnActionEnter = NULL;

	this->Focusable(TRUE);
}

CEditField::~CEditField()
{
	OnWindowClose();
}

void CEditField::SelectionRender()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glColor4ub(255, 255, 255, 64);

	glBegin(GL_QUADS);
	{
		glVertex2f(m_SelectionRect[0],                      m_SelectionRect[1] + m_SelectionRect[3]);
		glVertex2f(m_SelectionRect[0] + m_SelectionRect[2], m_SelectionRect[1] + m_SelectionRect[3]);
		glVertex2f(m_SelectionRect[0] + m_SelectionRect[2], m_SelectionRect[1]);
		glVertex2f(m_SelectionRect[0],                      m_SelectionRect[1]);
	}
	glEnd();
}

void CEditField::Render()
{
	PositionPush();
	{
		Translate(m_Rect[0], m_Rect[1]);

		this->RenderBackground();
		m_Text.Render();

		this->SelectionRender();
		this->OnWindowPaint();

		//if (this->Focused()) { CUserInterface::CarretRender(); }
	}
	PositionPop();
}

void CEditField::LayOut()
{
	CopyVector2(m_SizeIdeal, m_Text.Size());
	if (!m_SizeMin[0] && !m_SizeMin[1]) { CopyVector2(m_SizeMin, m_Text.Size()); }
	
	this->EnforceMinSize();

	Float4 LayoutRect = { 0, 0, m_Rect[2], m_Rect[3] };
	CWindowAbstract::TextLayOut(m_Text, LayoutRect, m_dwTextAlignment);
	
	//CUserInterface::CarretRect(this->CaretRect());
	//CUserInterface::CarretTimerReset();
	this->SelectionUpdate();
}

const Float4& CEditField::CaretRect()
{
	static Float4 Result;
	ZeroVector4(Result);

 	Int2 CaretSelection = { m_iCaretIndex, 0 };

	m_Text.RangeToRect(CaretSelection, Result);

	Result[0] += Result[2];
	Result[2] = 2;

	return Result;
}

void CEditField::OnKeyPressed(_IN SEventKey& rEvent)
{
	switch(rEvent.ucVirtualKey)
	{
	case VK_HOME:
		{
			m_iCaretIndex = 0;

			if (CHIDState::CKeyboard::KeyDown(VK_SHIFT))
			{
				m_Selection[0] = __min(m_iCaretIndex,  s_iCarretIndexStart);
				m_Selection[1] = __abs(m_iCaretIndex - s_iCarretIndexStart);
			}
			else
			{
				m_Selection[0] = m_iCaretIndex;
				m_Selection[1] = 0;
			}

			//CUserInterface::CarretRect(this->CaretRect());
			//CUserInterface::CarretTimerReset();
			this->SelectionUpdate();

			if (m_fpOnKeyPressed) { m_fpOnKeyPressed(*this, rEvent); }
			return;
		}
	case VK_END:
		{
			m_iCaretIndex = (int)m_Text.String().Length();

			if (CHIDState::CKeyboard::KeyDown(VK_SHIFT))
			{
				m_Selection[0] = __min(m_iCaretIndex,  s_iCarretIndexStart);
				m_Selection[1] = __abs(m_iCaretIndex - s_iCarretIndexStart);
			}
			else
			{
				m_Selection[0] = m_iCaretIndex;
				m_Selection[1] = 0;
			}

			//CUserInterface::CarretRect(this->CaretRect());
			//CUserInterface::CarretTimerReset();
			this->SelectionUpdate();

			if (m_fpOnKeyPressed) { m_fpOnKeyPressed(*this, rEvent); } 
			return;
		}
	case VK_SHIFT:
		{
			//If there is nothing selected, start selection at current index, otherwise continue selection
			if (m_Selection[1] == 0) { s_iCarretIndexStart = m_iCaretIndex;	}
			return;
		}
	case VK_RETURN:
		{
			this->ActionEnter();
			return;
		}
	case VK_LEFT:
		{
			if (m_iCaretIndex > 0) { m_iCaretIndex--; }

			if (CHIDState::CKeyboard::KeyDown(VK_SHIFT))
			{
				m_Selection[0] = __min(m_iCaretIndex,  s_iCarretIndexStart);
				m_Selection[1] = __abs(m_iCaretIndex - s_iCarretIndexStart);
			}
			else
			{
				m_Selection[0] = m_iCaretIndex;
				m_Selection[1] = 0;
			}

			//CUserInterface::CarretRect(this->CaretRect());
			//CUserInterface::CarretTimerReset();
			this->SelectionUpdate();

			if (m_fpOnKeyPressed) { m_fpOnKeyPressed(*this, rEvent); }
			return;
		}
	case VK_RIGHT:
		{
			if (m_iCaretIndex < (int)m_Text.String().Length()) { m_iCaretIndex++; }

			if (CHIDState::CKeyboard::KeyDown(VK_SHIFT))
			{
				m_Selection[0] = __min(m_iCaretIndex,  s_iCarretIndexStart);
				m_Selection[1] = __abs(m_iCaretIndex - s_iCarretIndexStart);
			}
			else
			{
				m_Selection[0] = m_iCaretIndex;
				m_Selection[1] = 0;
			}

			//CUserInterface::CarretRect(this->CaretRect());
			//CUserInterface::CarretTimerReset();
			this->SelectionUpdate();

			if (m_fpOnKeyPressed) { m_fpOnKeyPressed(*this, rEvent); } 
			return;
		}
	case VK_BACK:
		{
			if (m_Selection[1])
			{
				m_Text.String(CStringRange(m_Text.String(), 0, m_Selection[0]) + CStringRange(m_Text.String(), m_Selection[0] + m_Selection[1], m_Text.String().Length() - (m_Selection[0] + m_Selection[1])));
				m_Selection[1] = 0;
			}
			else
			{
				m_Text.String(CStringRange(m_Text.String(), 0, __max(m_Selection[0] - 1, 0)) + CStringRange(m_Text.String(), m_Selection[0], m_Text.String().Length() - m_Selection[0]));

				m_Selection[0] = __max(m_Selection[0] - 1, 0);
				m_Selection[1] = 0;
			}

			this->LayOut();

			m_iCaretIndex = m_Selection[0];

			//CUserInterface::CarretRect(this->CaretRect());
			//CUserInterface::CarretTimerReset();
			this->SelectionUpdate();

			if (m_fpOnKeyPressed) { m_fpOnKeyPressed(*this, rEvent); } 
			return;
		}
	case VK_DELETE:
		{
			if (m_Selection[1])
			{
				m_Text.String(CStringRange(m_Text.String(), 0, m_Selection[0]) + CStringRange(m_Text.String(), m_Selection[0] + m_Selection[1], m_Text.String().Length() - (m_Selection[0] + m_Selection[1])));
				m_Selection[1] = 0;
			}
			else
			{
				m_Text.String(CStringRange(m_Text.String(), 0, m_Selection[0]) + CStringRange(m_Text.String(), __min(m_Selection[0] + 1, (int)m_Text.String().Length()), __max((int)m_Text.String().Length() - (m_Selection[0] + 1), 0)));
				m_Selection[1] = 0;
			}

			this->LayOut();

			m_iCaretIndex = m_Selection[0];

			//CUserInterface::CarretRect(this->CaretRect());
			//CUserInterface::CarretTimerReset();
			this->SelectionUpdate();

			if (m_fpOnKeyPressed) { m_fpOnKeyPressed(*this, rEvent); } 
			return;
		}
	}

 	if (rEvent.cCharacter)
	{
		CString Tmp(" ");
		Tmp.AtPut(0, rEvent.cCharacter);
		   
		m_Text.String(CStringRange(m_Text.String(), 0, m_Selection[0]) + Tmp + CStringRange(m_Text.String(), m_Selection[0] + m_Selection[1], m_Text.String().Length() - (m_Selection[0] + m_Selection[1])));
 	
		m_Selection[0] = __min(m_Selection[0] + 1, (int)m_Text.String().Length());
		m_Selection[1] = 0;
	
		m_iCaretIndex = m_Selection[0];
	}

	if (m_fpOnKeyPressed) { m_fpOnKeyPressed(*this, rEvent); } 

	this->LayOut();
	//CUserInterface::CarretRect(this->CaretRect());
	this->SelectionUpdate();
}

void CEditField::OnKeyReleased(_IN SEventKey& rEvent)
{
	if (m_fpOnKeyReleased) { m_fpOnKeyReleased(*this, rEvent); } 
}

void CEditField::OnKeyFocusReceived(_IN SEventKey& rEvent)
{ 
	//CUserInterface::CarretRect(this->CaretRect());

	if (m_fpOnKeyFocusReceived) { m_fpOnKeyFocusReceived(*this, rEvent); }
}

void CEditField::OnKeyFocusLost(_IN SEventKey& rEvent)
{
	if (m_fpOnKeyFocusLost) { m_fpOnKeyFocusLost(*this, rEvent); } 
}

void CEditField::OnMouseButtonPressed(_IN SEventMouse& rEvent)
{
	CDispatcher::ElementMouseCapture(*this);

	CopyVector2(s_MousePositionStart, rEvent.Position);
	CopyVector2(s_MousePositionEnd,   rEvent.Position);

	this->SelectionUpdate();
	//CUserInterface::CarretRect(this->CaretRect());

	if (m_fpOnMouseButtonPressed) { m_fpOnMouseButtonPressed(*this, rEvent); }
}

void CEditField::OnMouseButtonReleased(_IN SEventMouse& rEvent) 
{ 
	CDispatcher::ElementMouseRelease();

	this->SelectionUpdate();

	m_iCaretIndex = m_Text.PositionToIndex(rEvent.Position);

	//CUserInterface::CarretRect(this->CaretRect());

	if (m_fpOnMouseButtonReleased) { m_fpOnMouseButtonReleased(*this, rEvent); } 
}

void CEditField::OnMouseButtonDoubleClick(_IN SEventMouse& rEvent)
{ 
	if (m_fpOnMouseButtonDoubleClick) { m_fpOnMouseButtonDoubleClick(*this, rEvent); }
}

void CEditField::OnMouseMove(_IN SEventMouse& rEvent) 
{
	if (CDispatcher::ElementMouseCaptured() == this)
	{
		CopyVector2(s_MousePositionEnd, rEvent.Position);

 		this->SelectionUpdateWhileDragging();
	}

	if (m_fpOnMouseMove) { m_fpOnMouseMove(*this, rEvent); } 
}

void CEditField::SelectionUpdateWhileDragging()
{
	Float4 Rect;

	Rect[0] = __min(s_MousePositionStart[0], s_MousePositionEnd[0]);
	Rect[1] = __min(s_MousePositionStart[1], s_MousePositionEnd[1]);

	Rect[2] = __abs(s_MousePositionStart[0] - s_MousePositionEnd[0]);
	Rect[3] = __abs(s_MousePositionStart[1] - s_MousePositionEnd[1]);

	m_Text.RectToRange(Rect, m_Selection);
	m_Text.RangeToRect(m_Selection, m_SelectionRect);
}

void CEditField::SelectionUpdate()
{
	m_Text.RangeToRect(m_Selection, m_SelectionRect);
}

void CEditField::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
}

void CEditField::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CString* pValue = NULL;

	//Text
	rReader.GetBlock();
	{
		GETVALUE { Text().Font(*(CFont*)CScripting::Object(*pValue)); }

		GETVALUE { TextAlignment(CXMLEnumeration::Translate(STRING("TextAlignments"), *pValue)->Value()); }

		GETVALUE { Text().Color(CFloat4(*pValue).Value()); }

		//Text data
		//rReader.GetBlock();
		//Text().String(*rReader.GetBlockData());

		//Text data
		Text().String(*rReader.GetBlockValue());
	}

	LayOut();
}
