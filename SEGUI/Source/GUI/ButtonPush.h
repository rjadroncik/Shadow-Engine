#pragma once
#include "ButtonMultiState.h"

class CSpinner;

class SHADOWUI_API CButtonPush : public CButtonMultiState
{
	friend CSpinner;

public:
	UINT    ClassKey()       _GET { return ClassButtonPush; }
	CString ClassKeyString() _GET { return TEXT("ButtonPush"); }

	CButtonPush(CWindowAbstract* pParent);
	virtual ~CButtonPush();

 	COLOR_RGBA ColorRaised();
	COLOR_RGBA ColorPushed();

	void ColorRaised(IN COLOR_RGBA color);
	void ColorPushed(IN COLOR_RGBA color);

	//Compute automatic size
	void AutoRect(OUT Rect4i* pOutRect);

protected:
 	void OnWindowPaint(IN CEventWindow& rEvent);

protected:
 	struct SColorInfo
	{
		Color*	pRaised;
		Color*	pPushed;

	} m_Colors;
};
