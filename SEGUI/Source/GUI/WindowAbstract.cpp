#include "WindowAbstract.h"
#include "Dispatcher.h"

using namespace GUI;

CWindowAbstract::CWindowAbstract()
{
	m_pParent    = NULL;

	m_bEnabled   = TRUE;

	m_bResizable = TRUE;
	m_bFocusable = FALSE;

	SetVector4(m_ColorBackground, 0.375f, 0.375f, 0.375f, 0.5f);

	//Basic window events
	m_fpOnWindowOpen            = NULL;
	m_fpOnWindowClose           = NULL;        
	m_fpOnWindowMove            = NULL;           
	m_fpOnWindowSize            = NULL;           
	m_fpOnWindowPaint           = NULL;          
	m_fpOnWindowPaintBackground = NULL;
	m_fpOnWindowShow            = NULL;           
	m_fpOnWindowHide            = NULL;           

	//Mouse tracking
	m_fpOnMouseEnter             = NULL;
	m_fpOnMouseLeave             = NULL;
	m_fpOnMouseButtonPressed     = NULL;
	m_fpOnMouseButtonReleased    = NULL;
	m_fpOnMouseButtonDoubleClick = NULL;
	m_fpOnMouseMove              = NULL;
	m_fpOnMouseWheel             = NULL;
	m_fpOnMouseCaptuereLost      = NULL;

	//Keyboard tracking
	m_fpOnKeyPressed       = NULL;
	m_fpOnKeyReleased      = NULL;
	m_fpOnKeyFocusReceived = NULL;
	m_fpOnKeyFocusLost     = NULL;
}

CWindowAbstract::~CWindowAbstract()
{
}

void CWindowAbstract::Parent(_IN CWindowContainer& rParent) _SET
{
	//prevents infinite recursion when [ChildAdd(..)]  is called on a container
	if (m_pParent == (CWindowContainer*)&rParent) { return; }

	if (m_pParent) { m_pParent->ChildRemove(*this); }

	m_pParent = (CWindowContainer*)&rParent;
	m_pParent->ChildAdd(*this);
}

void CWindowAbstract::RenderBackground()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glColor4fv(m_ColorBackground);

	glBegin(GL_QUADS);
	{
		glVertex2f(0,         m_Rect[3]);
		glVertex2f(m_Rect[2], m_Rect[3]);
		glVertex2f(m_Rect[2], 0);
		glVertex2f(0,         0);
	}
	glEnd();

	this->OnWindowPaintBackground();
}

void CWindowAbstract::TextLayOut(_INOUT CText& rText, _IN Float4& rRect, _IN DWORD dwAligment)
{
	if (!rText.Font()) { return; }

	Float2 Tmp;

	switch (dwAligment & AligmentMaskHorizontal)
	{
	case AlignCenter:
		{
			Tmp[0] = (float)(int)(rRect[0] + (rRect[2] - rText.Size()[0]) / 2);
			break;
		}
	case AlignLeft:
		{
			Tmp[0] = rRect[0];
			break;
		}
	case AlignRight:
		{
			Tmp[0] = rRect[0] + rRect[2] - rText.Size()[0];
			break;
		}
	}

	switch (dwAligment & AligmentMaskVertical)
	{
	case AlignCenter:
		{
			Tmp[1] = (float)(int)(rRect[1] + (rRect[3] + rText.Size()[1]) / 2 - rText.Font()->Descent());
			break;
		}
	case AlignTop:
		{
			Tmp[1] = rRect[1] + rText.Font()->Ascent();
			break;
		}
	case AlignBottom:
		{
			Tmp[1] = rRect[1] + rRect[3] - rText.Font()->Descent();
			break;
		}
	}

	rText.Position(Tmp);
}

bool CWindowAbstract::Hot() _GET
{
	return (this == CDispatcher::ElementHot());
}

bool CWindowAbstract::Focused() _GET
{
	return (this == CDispatcher::ElementFocused());
}

bool CWindowAbstract::MouseCaptured() _GET
{
	return (this == CDispatcher::ElementMouseCaptured());
}
		  
CWindowAbstract* CWindowAbstract::MouseOver(_IN Float2& rMousePos, _OPT _OUT Float2* pOutLocalPosition) _GET
{
	if ((rMousePos[0] >= m_Rect[0]) && (rMousePos[0] < (m_Rect[0] + m_Rect[2])) &&
		(rMousePos[1] >= m_Rect[1]) && (rMousePos[1] < (m_Rect[1] + m_Rect[3])))
	{ 
		if (pOutLocalPosition) { SubtractVectors2(*pOutLocalPosition, rMousePos, (Float2&)m_Rect); }

		return (CWindowAbstract*)this;
	}

	return NULL;
}

bool CWindowAbstract::MouseLocalPos(_IN Float2& rMousePos, _IN CWindowAbstract& rWindow, _OUT Float2& rOutLocalPosition) _GET
{
	if (this == &rWindow)
	{ 
		SubtractVectors2(rOutLocalPosition, rMousePos, (Float2&)m_Rect);
		return TRUE;
	}

	return FALSE;
}