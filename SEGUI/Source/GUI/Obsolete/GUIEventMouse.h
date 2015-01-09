#pragma once
#include "GUIEvent.h"

class GUI_API CGUIEventMouse : public CGUIEvent
{
	friend CGUIDispatcher;

public:
	Point2i& Position()   { return m_Position; }
	int      WheelDelta() { return m_iDelta; }

	BOOL ButtonLeft()   { return m_dwFlags & MK_LBUTTON; }
	BOOL ButtonMiddle() { return m_dwFlags & MK_MBUTTON; }
	BOOL ButtonRight()  { return m_dwFlags & MK_RBUTTON; }

	BOOL ButtonX1()   { return m_dwFlags & MK_XBUTTON1; }
	BOOL ButtonX2()   { return m_dwFlags & MK_XBUTTON2; }

	BOOL KeyShift()   { return m_dwFlags & MK_SHIFT; }
	BOOL KeyControl() { return m_dwFlags & MK_CONTROL; }

protected:
	CGUIEventMouse();
	virtual ~CGUIEventMouse();

	void Position  (IN Point2i& rPosition)  { m_Position = (Point2i&)rPosition; }
	void Flags     (IN DWORD dwFlags)       { m_dwFlags  = dwFlags; }
 	void WheelDelta(IN int iDelta)          { m_iDelta   = iDelta; }

private: 
	Point2i m_Position;
 	DWORD   m_dwFlags;

	int     m_iDelta;
};
