#pragma once
#include "ButtonMultiState.h"

#define BTN_STYLE_CLICK_CIRCLE_SIZE	13

class SHADOWUI_API CButtonRadio : public CButtonMultiState
{
public:
	UINT    ClassKey()       _GET { return ClassButtonRadio; }
	CString ClassKeyString() _GET { return TEXT("ButtonRadio"); }

	CButtonRadio(CWindowAbstract* pParent);
	virtual ~CButtonRadio();

	//Compute automatic size
	void AutoRect(OUT Rect4i* pOutRect);

protected:
	void OnWindowPaint(IN CEventWindow& rEvent);
};
