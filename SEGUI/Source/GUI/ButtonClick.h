#pragma once
#include "Button.h"

namespace GUI
{
	class SEGUI_API CButtonClick;
		
	typedef void (*HANDLER_ACTION_BUTTON_CLICK)(_INOUT CButtonClick&);

	class SEGUI_API CButtonClick : public CButton
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassButtonClick; }

	public:
		CButtonClick();
		virtual ~CButtonClick();

	public:
		void ActionClick() { if (m_fpOnActionClick) { m_fpOnActionClick(*this); } }
	
	public:
		void                        OnActionClick(HANDLER_ACTION_BUTTON_CLICK fpActionHandler) _SET { m_fpOnActionClick = fpActionHandler; }
		HANDLER_ACTION_BUTTON_CLICK OnActionClick()                                            _GET { return m_fpOnActionClick; }

	public:
		void Render();
		void LayOut();

	protected:
		void OnMouseButtonPressed (_IN SEventMouse& rEvent);
		void OnMouseButtonReleased(_IN SEventMouse& rEvent);
		void OnMouseMove          (_IN SEventMouse& rEvent);

	protected: 
		bool m_bPressed;

	protected:
		HANDLER_ACTION_BUTTON_CLICK m_fpOnActionClick;
	};
};
