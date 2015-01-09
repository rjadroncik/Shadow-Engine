#pragma once

#include "WindowAbstract.h"

namespace GUI
{
	class SEGUI_API CButton : public CWindowAbstract  
	{
	public:
		CButton();
		virtual ~CButton();

	public:
		void OnMouseEnter(_IN SEventMouse& rEvent);
		void OnMouseLeave(_IN SEventMouse& rEvent);
	};
};