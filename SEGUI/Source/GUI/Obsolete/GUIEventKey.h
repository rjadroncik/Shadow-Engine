#pragma once
#include "GUIEvent.h"

class GUI_API CGUIEventKey : public CGUIEvent
{
	friend CGUIDispatcher;

public:
	UINT VirtualKey() { return m_uiKeyCode; }
	UINT Character () { return m_uiKeyCode; }

protected:
	CGUIEventKey();
	virtual ~CGUIEventKey();

	void VirtualKey(IN UINT uiVKey)  { m_uiKeyCode = uiVKey; }
	void Character (IN UINT uiChar)  { m_uiKeyCode = uiChar; }

private:
	UINT m_uiKeyCode;
};
