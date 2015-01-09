#pragma once
#include "Button.h"

class SHADOWUI_API CButtonMultiState : public CButton
{
public:
 	BOOL Active()					{ return m_bActive; }
	void Active(IN BOOL bActive)	{ m_bActive = bActive; }

	void ActionActivate();
	void ActionDeactivate();

protected:
	CButtonMultiState();
	virtual ~CButtonMultiState();

	void OnMouseLeftDown(IN CEventMouse& rEvent);
	void OnMouseLeftUp  (IN CEventMouse& rEvent);
	void OnMouseMove    (IN CEventMouse& rEvent);

protected:
	BOOL m_bActive;
	BOOL m_bActiveLast;
};
