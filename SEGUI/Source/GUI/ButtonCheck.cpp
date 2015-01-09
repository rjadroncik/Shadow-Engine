#include "StdAfx.h"
#include "ButtonCheck.h"
#include "Paint.h"
#include "Event.h"

CButtonCheck::CButtonCheck(CWindowAbstract* pParent)
{
	this->WindowCreate(Win32ClassCButton, WS_CHILD, WS_EX_TRANSPARENT, pParent);
}

CButtonCheck::~CButtonCheck()
{
}

void CButtonCheck::OnWindowPaint(IN CEventWindow& rEvent)
{
	//Prepare variables
	Rect4i rt = this->RectWindow(); rt.iX = 0; rt.iY = 0;
	RectF rect((float)rt.iX, (float)rt.iY, (float)rt.iWidth, (float)rt.iHeight);

	//Prepare drawing canvas
	rEvent.Canvas().SetSmoothingMode(SmoothingModeHighQuality);

	//Prepare circle rectangle
	rt.iX = 0;
	rt.iY = (rt.iHeight - BTN_STYLE_CLICK_CIRCLE_SIZE) / 2;

	rt.iWidth = rt.iX + BTN_STYLE_CLICK_CIRCLE_SIZE;
	rt.iHeight = rt.iY + BTN_STYLE_CLICK_CIRCLE_SIZE;

	//Prepare text rectangle 
	rect.X = (float)rt.iWidth + 3; rect.Width -= rect.X;

	//Draw rounded rectangle background
	LinearGradientBrush brush(RectF((float)rt.iX, (float)rt.iY, (float)(rt.iWidth - rt.iX), (float)(rt.iHeight - rt.iY)), *CPaint::SysColors.pLight, *CPaint::SysColors.pFace, LinearGradientModeForwardDiagonal);
	CPaint::Draw3DRectangle(rEvent.Canvas(), rt, 1, NULL, NULL, &brush);

	if (m_bHot) 
	{
		//Draw hot highlight rim
		CPaint::RectResize(rt, - 1);
		CPaint::Draw3DRectangle(rEvent.Canvas(), rt, 1, CPaint::SysPens.pOrange_3px, CPaint::SysPens.pOrange_3px, NULL);
		CPaint::RectResize(rt, 1);
	}

	//Draw button rim
	if (this->Border()) { CPaint::Draw3DRectangle(rEvent.Canvas(), rt, 1, CPaint::SysPens.pDarkShadow, CPaint::SysPens.pLight, NULL); }

	StringFormat format;
	format.SetAlignment(StringAlignmentNear);
	format.SetLineAlignment(StringAlignmentCenter);

	//Draw label
	rEvent.Canvas().DrawString(this->Label(), -1, CPaint::SysFonts.pArial_11px, rect, &format, CPaint::SysBrushes.pBlack);

	//Draw check-mark
	if (m_bActive)
	{	
		rt.iX += 2;
		rt.iY += 2;
	
		rt.iWidth  -= rt.iX + 3;
		rt.iHeight -= rt.iY + 3;

 		//CPaint::RectResize(rt, -4);
		CPaint::Draw3DCheckMark(rEvent.Canvas(), rt, CPaint::SysPens.pDarkShadow, CPaint::SysPens.pDarkShadow, CPaint::SysBrushes.pBlack);
		//CPaint::RectResize(rt, 4);

		rt.iX -= 2;
		rt.iY -= 2;

		rt.iWidth  += rt.iX + 3;
		rt.iHeight += rt.iY + 3;
	}

	//Draw focused dotted outline
	if (this->FocusFrame() && this->Focused())
	{
		Rect rt2((int)rect.X - 1, rt.iY, (int)rect.Width, 13);

		rEvent.Canvas().SetSmoothingMode(SmoothingModeNone);
		rEvent.Canvas().DrawRectangle(CPaint::SysPens.pDottedGray, rt2);
		rEvent.Canvas().SetSmoothingMode(SmoothingModeHighQuality);
	}
}

void CButtonCheck::AutoRect(OUT Rect4i* pOutRect)
{
	Graphics	graphics(this->WindowHandle());
	RectF		rt;
	PointF		pt(0, 0);

	graphics.MeasureString(this->Label(), -1, CPaint::SysFonts.pArial_11px, pt, &rt);

	pOutRect->iWidth  = (int)rt.Width + BTN_STYLE_CLICK_CIRCLE_SIZE + 3 + 2;
	pOutRect->iHeight = (int)rt.Height + 4;
}