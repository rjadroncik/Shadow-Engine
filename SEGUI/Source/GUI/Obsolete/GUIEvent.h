#pragma once

class CGUIDispatcher;

class GUI_API CGUIEvent
{
	friend CGUIDispatcher;

protected:
	CGUIEvent();
	virtual ~CGUIEvent();
};
