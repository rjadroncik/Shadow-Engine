#include "StdAfx.h"
#include "GUIEventMouse.h"

CGUIEventMouse::CGUIEventMouse()
{
	m_iDelta  = 0;
	m_dwFlags = 0;

	m_Position.iX = INT_MIN;
	m_Position.iY = INT_MIN;
}

CGUIEventMouse::~CGUIEventMouse()
{
} 