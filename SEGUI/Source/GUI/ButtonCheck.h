#pragma once
#include "ButtonMultiState.h"

#define BTN_STYLE_CLICK_CIRCLE_SIZE	13

class SHADOWUI_API CButtonCheck : public CButtonMultiState
{
public:
	UINT    ClassKey()       _GET { return ClassButtonCheck; }
	CString ClassKeyString() _GET { return TEXT("ButtonCheck"); }

	CButtonCheck(CWindowAbstract* pParent);
	virtual ~CButtonCheck();

	//Compute automatic size
	void AutoRect(OUT Rect4i* pOutRect);

protected:
	void OnWindowPaint(IN CEventWindow& rEvent);
};
