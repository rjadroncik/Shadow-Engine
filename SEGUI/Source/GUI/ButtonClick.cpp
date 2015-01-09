#include "ButtonClick.h"

#include "Dispatcher.h"

#include <SCFGraphics.h>

using namespace GUI;
using namespace System;

CButtonClick::CButtonClick()
{
	m_bPressed = FALSE;

	m_fpOnActionClick = NULL;
}

CButtonClick::~CButtonClick()
{
}

/*void CButtonClick::OnWindowPaint(IN CEventWindow& rEvent)
{
	SCF::Rect4i rt = this->RectWindow(); rt.iX = 0; rt.iY = 0;
	RectF rect((float)rt.iX, (float)rt.iY, (float)rt.iWidth, (float)rt.iHeight);

	rEvent.Canvas().SetSmoothingMode(SmoothingModeHighQuality);

	if (m_bActive)
	{
		LinearGradientBrush brush(Point(0, 0), Point(0, this->RectWindow().iHeight), *CPaint::SysColors.pFace, *CPaint::SysColors.pLightFace);
		CPaint::Draw3DRectangle(rEvent.Canvas(), rt, m_uiBevel, NULL, NULL, &brush); 
	}
	else 
	{
		LinearGradientBrush brush(Point(0, 0), Point(0, this->RectWindow().iHeight), *CPaint::SysColors.pLightFace, *CPaint::SysColors.pFace);
		CPaint::Draw3DRectangle(rEvent.Canvas(), rt, m_uiBevel, NULL, NULL, &brush); 
	}

	if (m_bHot) 
	{
		CPaint::RectResize(rt, -1);
		if (m_bActive) { CPaint::Draw3DRectangle(rEvent.Canvas(), rt, m_uiBevel, CPaint::SysPens.pLightShadow_3px, CPaint::SysPens.pLightShadow_3px, NULL); }
		else           { CPaint::Draw3DRectangle(rEvent.Canvas(), rt, m_uiBevel, CPaint::SysPens.pOrange_3px,      CPaint::SysPens.pOrange_3px,      NULL); }
		CPaint::RectResize(rt, 1);
	}

	if (m_bActive) { CPaint::Draw3DRectangle(rEvent.Canvas(), rt, m_uiBevel, CPaint::SysPens.pDarkShadow, CPaint::SysPens.pLight,      NULL); }
	else           
	{
		if (this->Border())
		{
			CPaint::Draw3DRectangle(rEvent.Canvas(), rt, m_uiBevel, CPaint::SysPens.pLight,      CPaint::SysPens.pDarkShadow, NULL); 
		}
	}

	StringFormat format;
	format.SetAlignment    (StringAlignmentCenter);
	format.SetLineAlignment(StringAlignmentCenter);

	if (m_bActive) { rect.X++; rect.Y++; }
	rEvent.Canvas().DrawString(this->Label(), -1, CPaint::SysFonts.pArial_11px, rect, &format, CPaint::SysBrushes.pBlack);

	if (this->FocusFrame() && this->Focused())
	{
		CPaint::RectResize(rt, -3);

		rEvent.Canvas().SetSmoothingMode(SmoothingModeNone);
		CPaint::Draw3DRectangle(rEvent.Canvas(), rt, m_uiBevel - 1, CPaint::SysPens.pDottedGray, CPaint::SysPens.pDottedGray, NULL);
		rEvent.Canvas().SetSmoothingMode(SmoothingModeHighQuality);
	}
}*/

void CButtonClick::OnMouseButtonPressed(_IN SEventMouse& rEvent)
{
	if (!this->Enabled()) { return; }
								   
	if (rEvent.Data.ucButton == 0) 
	{
		m_bPressed = TRUE;
		CDispatcher::ElementMouseCapture(*this);
	}
}

void CButtonClick::OnMouseButtonReleased(_IN SEventMouse& rEvent)
{
	if (!this->Enabled()) { return; }

	if (rEvent.Data.ucButton == 0) 
	{
		CDispatcher::ElementMouseRelease();
		m_bPressed = FALSE;

		Float4 rt = { 0, 0, m_Rect[2], m_Rect[3] };

		if (PointInRect((SCFGraphics::Point2i&)rEvent.Position, (SCFGraphics::Rect4i&)rt)) { ActionClick(); }
	}
}

void CButtonClick::OnMouseMove(_IN SEventMouse& rEvent)
{
	if (!this->Enabled()) { return; }

	Float4 rt = { 0, 0, m_Rect[2], m_Rect[3] };

	if ((CDispatcher::ElementMouseCaptured() != this) || !CHIDState::CMouse::ButtonDown(0)) { return; }

	if (PointInRect((SCFGraphics::Point2i&)rEvent.Position, (SCFGraphics::Rect4i&)rt)) 
	{
		m_bPressed = TRUE;
	}
	else 
	{
		m_bPressed = FALSE;
	}
} 

void CButtonClick::Render()
{
	ScissorPush(m_Rect);
	PositionPush();
	{
		Translate(m_Rect[0], m_Rect[1]);

		if (m_bPressed) { Translate(1, 1); }

		this->RenderBackground();
		this->OnWindowPaint();
	}
	PositionPop();
	ScissorPop();
}

void CButtonClick::LayOut()
{
}