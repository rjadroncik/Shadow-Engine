#pragma once
#include "GUIEvent.h"


class GUI_API CGUIEventWindow : public CGUIEvent
{
	friend CGUIDispatcher;

public:
	Rect4i&   RectWindow() { return m_RectWindow; }
	Rect4i&   RectClient() { return m_RectClient; }
	Rect4i&   RectUpdate() { return m_RectUpdate; }

protected:
	CGUIEventWindow();
	virtual ~CGUIEventWindow();
	
	void	RectWindow(IN Rect4i& rRect)      { m_RectWindow = rRect; } 
	void	RectClient(IN Rect4i& rRect)      { m_RectClient = rRect; }
	void	RectUpdate(IN Rect4i& rRect)      { m_RectUpdate = rRect; }

private:
	Rect4i    m_RectWindow;
	Rect4i    m_RectClient;
	Rect4i    m_RectUpdate;
};
