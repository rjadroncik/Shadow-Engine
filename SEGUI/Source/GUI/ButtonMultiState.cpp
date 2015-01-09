#include "StdAfx.h"
#include "ButtonMultiState.h"
#include "Event.h"

CButtonMultiState::CButtonMultiState()
{
	m_bActive     = FALSE;
	m_bActiveLast = FALSE;

	AllowDeactivate(TRUE);
}

CButtonMultiState::~CButtonMultiState()
{
}

void CButtonMultiState::OnMouseLeftDown(IN CEventMouse& rEvent)
{
	CButton::OnMouseLeftDown(rEvent);

	m_bActiveLast =  m_bActive;
	m_bActive     = !m_bActive;

	this->Redraw(TRUE);
}

void CButtonMultiState::OnMouseLeftUp(IN CEventMouse& rEvent)
{
	Rect4i rt = this->RectWindow(); rt.iX = 0; rt.iY = 0;

	if (PointInRect(rEvent.Position(), rt)) 
	{
		if (m_bActiveLast && this->AllowDeactivate()) { ActionDeactivate(); }
		else                                          { ActionActivate(); }
	}
	else { m_bActive = m_bActiveLast; }

	CButton::OnMouseLeftUp(rEvent);
}

void CButtonMultiState::OnMouseMove(IN CEventMouse& rEvent)
{
	CButton::OnMouseMove(rEvent);

	Rect4i rt = this->RectWindow(); rt.iX = 0; rt.iY = 0;

	//Continue only if the iX mouse button is pressed
	if (!this->MouseCapture() || !rEvent.ButtonLeft()) { return; }

	if (PointInRect(rEvent.Position(), rt)) 
	{
		//The mouse is inside the window, display pushed button, redraw if necessary
		if (m_bActive == m_bActiveLast)
		{
			m_bActive = !m_bActiveLast;
			this->Redraw(TRUE);
		}	
	}
	else 
	{
		//The mouse is outside the window, display normal button, redraw if necessary
		if (m_bActive != m_bActiveLast)
		{
			m_bActive = m_bActiveLast;
			this->Redraw(TRUE);
		}
	}
}

void CButtonMultiState::ActionActivate()
{   
	m_bActive = TRUE;

	CAction ClickAction(ActionActivated, this);
	ActionPerform(ClickAction);
}

void CButtonMultiState::ActionDeactivate()
{
	m_bActive = FALSE; 

	CAction ClickAction(ActionDeactivated, this);
	ActionPerform(ClickAction);
}
