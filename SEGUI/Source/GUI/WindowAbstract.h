#pragma once

#include "Text.h"
#include "Element.h"

namespace GUI
{
	struct SEventMouse
	{
		Float2 Position;

		union UData
		{
			SCF::BYTE ucButton;
			char      cWheelDelta;

		} Data;
	};

	struct SEventKey
	{
		SCF::BYTE  ucVirtualKey;
		SCF::TCHAR cCharacter;
	};

 	class SEGUI_API CWindowAbstract;
	class SEGUI_API CWindowContainer;

	typedef void (*HANDLER_EVENT_WINDOW)(_INOUT CWindowAbstract&);
	typedef void (*HANDLER_EVENT_MOUSE) (_INOUT CWindowAbstract&, _IN SEventMouse&);
	typedef void (*HANDLER_EVENT_KEY)   (_INOUT CWindowAbstract&, _IN SEventKey&);

	class SEGUI_API CWindowAbstract : public CElement
	{
	public:
		//Basic window events
		virtual void OnWindowOpen           () { if (m_fpOnWindowOpen)            { m_fpOnWindowOpen(*this); } this->LayOut(); this->OnWindowSize(); }
		virtual void OnWindowClose          () { if (m_fpOnWindowClose)           { m_fpOnWindowClose(*this); } }
		virtual void OnWindowMove           () { if (m_fpOnWindowMove)            { m_fpOnWindowMove(*this); } }
		virtual void OnWindowSize           () { if (m_fpOnWindowSize)            { m_fpOnWindowSize(*this); } this->LayOut(); }
		virtual void OnWindowPaint          () { if (m_fpOnWindowPaint)           { m_fpOnWindowPaint(*this); } }
		virtual void OnWindowPaintBackground() { if (m_fpOnWindowPaintBackground) { m_fpOnWindowPaintBackground(*this); } }
		virtual void OnWindowShow           () { if (m_fpOnWindowShow)            { m_fpOnWindowShow(*this); } }
		virtual void OnWindowHide           () { if (m_fpOnWindowHide)            { m_fpOnWindowHide(*this); } }

		//Mouse tracking
		virtual void OnMouseEnter            (_IN SEventMouse& rEvent) { if (m_fpOnMouseEnter)             { m_fpOnMouseEnter            (*this, rEvent); } }
		virtual void OnMouseLeave            (_IN SEventMouse& rEvent) { if (m_fpOnMouseLeave)             { m_fpOnMouseLeave            (*this, rEvent); } }
		virtual void OnMouseButtonPressed    (_IN SEventMouse& rEvent) { if (m_fpOnMouseButtonPressed)     { m_fpOnMouseButtonPressed    (*this, rEvent); } }
		virtual void OnMouseButtonReleased   (_IN SEventMouse& rEvent) { if (m_fpOnMouseButtonReleased)    { m_fpOnMouseButtonReleased   (*this, rEvent); } }
		virtual void OnMouseButtonDoubleClick(_IN SEventMouse& rEvent) { if (m_fpOnMouseButtonDoubleClick) { m_fpOnMouseButtonDoubleClick(*this, rEvent); } }
		virtual void OnMouseMove             (_IN SEventMouse& rEvent) { if (m_fpOnMouseMove)              { m_fpOnMouseMove             (*this, rEvent); } }
		virtual void OnMouseWheel            (_IN SEventMouse& rEvent) { if (m_fpOnMouseWheel)             { m_fpOnMouseWheel            (*this, rEvent); } }
		virtual void OnMouseCaptuereLost     (_IN SEventMouse& rEvent) { if (m_fpOnMouseCaptuereLost)      { m_fpOnMouseCaptuereLost     (*this, rEvent); } }

		//Keyboard tracking
		virtual void OnKeyPressed      (_IN SEventKey& rEvent) { if (m_fpOnKeyPressed)       { m_fpOnKeyPressed      (*this, rEvent); } }
		virtual void OnKeyReleased     (_IN SEventKey& rEvent) { if (m_fpOnKeyReleased)      { m_fpOnKeyReleased     (*this, rEvent); } }
		virtual void OnKeyFocusReceived(_IN SEventKey& rEvent) { if (m_fpOnKeyFocusReceived) { m_fpOnKeyFocusReceived(*this, rEvent); } }
		virtual void OnKeyFocusLost    (_IN SEventKey& rEvent) { if (m_fpOnKeyFocusLost)     { m_fpOnKeyFocusLost    (*this, rEvent); } }

	public:
		//Basic window events
		void OnWindowOpen           (HANDLER_EVENT_WINDOW fpEventHandler) { m_fpOnWindowOpen            = fpEventHandler; }
		void OnWindowClose          (HANDLER_EVENT_WINDOW fpEventHandler) { m_fpOnWindowClose           = fpEventHandler; }
		void OnWindowMove           (HANDLER_EVENT_WINDOW fpEventHandler) { m_fpOnWindowMove            = fpEventHandler; }
		void OnWindowSize           (HANDLER_EVENT_WINDOW fpEventHandler) { m_fpOnWindowSize            = fpEventHandler; }
		void OnWindowPaint          (HANDLER_EVENT_WINDOW fpEventHandler) { m_fpOnWindowPaint           = fpEventHandler; }
		void OnWindowPaintBackground(HANDLER_EVENT_WINDOW fpEventHandler) { m_fpOnWindowPaintBackground = fpEventHandler; }
		void OnWindowShow           (HANDLER_EVENT_WINDOW fpEventHandler) { m_fpOnWindowShow            = fpEventHandler; }
		void OnWindowHide           (HANDLER_EVENT_WINDOW fpEventHandler) { m_fpOnWindowHide            = fpEventHandler; }

		//Mouse tracking
		void OnMouseEnter            (HANDLER_EVENT_MOUSE fpEventHandler) { m_fpOnMouseEnter             = fpEventHandler; }
		void OnMouseLeave            (HANDLER_EVENT_MOUSE fpEventHandler) { m_fpOnMouseLeave             = fpEventHandler; }
		void OnMouseButtonPressed    (HANDLER_EVENT_MOUSE fpEventHandler) { m_fpOnMouseButtonPressed     = fpEventHandler; }
		void OnMouseButtonReleased   (HANDLER_EVENT_MOUSE fpEventHandler) { m_fpOnMouseButtonReleased    = fpEventHandler; }
		void OnMouseButtonDoubleClick(HANDLER_EVENT_MOUSE fpEventHandler) { m_fpOnMouseButtonDoubleClick = fpEventHandler; }
		void OnMouseMove             (HANDLER_EVENT_MOUSE fpEventHandler) { m_fpOnMouseMove              = fpEventHandler; }
		void OnMouseWheel            (HANDLER_EVENT_MOUSE fpEventHandler) { m_fpOnMouseWheel             = fpEventHandler; }
		void OnMouseCaptuereLost     (HANDLER_EVENT_MOUSE fpEventHandler) { m_fpOnMouseCaptuereLost      = fpEventHandler; }

		//Keyboard tracking
		void OnKeyPressed      (HANDLER_EVENT_KEY fpEventHandler) { m_fpOnKeyPressed       = fpEventHandler; }
		void OnKeyReleased     (HANDLER_EVENT_KEY fpEventHandler) { m_fpOnKeyReleased      = fpEventHandler; }
		void OnKeyFocusReceived(HANDLER_EVENT_KEY fpEventHandler) { m_fpOnKeyFocusReceived = fpEventHandler; }
		void OnKeyFocusLost    (HANDLER_EVENT_KEY fpEventHandler) { m_fpOnKeyFocusLost     = fpEventHandler; }

	public:
		void              Parent(_IN CWindowContainer& rParent) _SET;
		CWindowContainer* Parent()                              _GET { return m_pParent; }

	public:
		//Returns self if mouse is over this element, NULL otherwise
		virtual CWindowAbstract* MouseOver    (_IN Float2& rMousePos, _OPT _OUT Float2* pOutLocalPosition = NULL) _GET;
		virtual bool             MouseLocalPos(_IN Float2& rMousePos, _IN CWindowAbstract& rWindow, _OUT Float2& rOutLocalPosition) _GET;

	public:
		void Enabled(_IN bool bEnabled) _SET { m_bEnabled = bEnabled; }
		bool Enabled()                  _GET { return m_bEnabled; }

		void Resizable(_IN bool bResizable) _SET { m_bResizable = bResizable; }
		bool Resizable()                    _GET { return m_bResizable; }
	
		void Focusable(_IN bool bFocusable) _SET { m_bFocusable = bFocusable; }
		bool Focusable()                    _GET { return m_bFocusable; }
		
	public:
		bool Hot()           _GET;
		bool Focused()       _GET;
		bool MouseCaptured() _GET;

	public:
		inline const Float4& ColorBackground()                   _GET { return m_ColorBackground; }
		inline void          ColorBackground(_IN Float4& rColor) _SET { CopyVector4(m_ColorBackground, rColor); }

	protected:
		virtual void RenderBackground();

	protected:
		CWindowAbstract();
		virtual ~CWindowAbstract();

	protected:
		static void TextLayOut(_INOUT CText& rText, _IN Float4& rRect, _IN SCF::DWORD dwAligment);

	private:
		CWindowContainer* m_pParent;

	private:
		bool m_bEnabled;

		bool m_bResizable;
		bool m_bFocusable;

		Float4 m_ColorBackground;

	protected:
		//Basic window events
		HANDLER_EVENT_WINDOW m_fpOnWindowOpen;
		HANDLER_EVENT_WINDOW m_fpOnWindowClose;        
		HANDLER_EVENT_WINDOW m_fpOnWindowMove;           
		HANDLER_EVENT_WINDOW m_fpOnWindowSize;           
		HANDLER_EVENT_WINDOW m_fpOnWindowPaint;          
		HANDLER_EVENT_WINDOW m_fpOnWindowPaintBackground;
		HANDLER_EVENT_WINDOW m_fpOnWindowShow;           
		HANDLER_EVENT_WINDOW m_fpOnWindowHide;           

		//Mouse tracking
		HANDLER_EVENT_MOUSE m_fpOnMouseEnter;
		HANDLER_EVENT_MOUSE m_fpOnMouseLeave;
		HANDLER_EVENT_MOUSE m_fpOnMouseButtonPressed;
		HANDLER_EVENT_MOUSE m_fpOnMouseButtonReleased;
		HANDLER_EVENT_MOUSE m_fpOnMouseButtonDoubleClick;
		HANDLER_EVENT_MOUSE m_fpOnMouseMove;
		HANDLER_EVENT_MOUSE m_fpOnMouseWheel;
		HANDLER_EVENT_MOUSE m_fpOnMouseCaptuereLost;

		//Keyboard tracking
		HANDLER_EVENT_KEY m_fpOnKeyPressed;
		HANDLER_EVENT_KEY m_fpOnKeyReleased;
		HANDLER_EVENT_KEY m_fpOnKeyFocusReceived;
		HANDLER_EVENT_KEY m_fpOnKeyFocusLost;
	};
};
