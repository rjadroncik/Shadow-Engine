#pragma once

#include "../../../SECore/Source/SECore.h"
#include "../../../SECore/Source/Resources/Manager.h"

#include "../../../SECore/Source/System/HIDState.h"

#include "../Resources/Cursor.h"

#include "Label.h"
#include "EditField.h"
#include "Window.h"
#include "Module.h"

#include "LayoutTable.h"
#include "LayoutSplit.h"

namespace GUI
{
	class SEGUI_API CDispatcher
	{
	public:
		//Dispatcher handles only 1 module at a time
		static inline CModule* GUIModule()                    { return s_Objects.GUI.pModule; }
		static inline void     GUIModule(CModule* pGUIModule) { s_Objects.GUI.pModule = pGUIModule; }

	public:
		//Access to current state of the GUI
		static inline CWindowAbstract* ElementFocused()       { return s_Objects.GUI.Element.pFocused; }
		static inline CWindowAbstract* ElementHot()           { return s_Objects.GUI.Element.pHot; }
		static inline CWindowAbstract* ElementMouseCaptured() { return s_Objects.GUI.Element.pMouseCaptured; }

	public:
		//Basic GUI manipulation
		static void ElementMouseCapture(_IN CWindowAbstract& pWindow);
		static void ElementMouseRelease();

	public:
		//System related functionality
		static void Initialize();
		static void HandlerRender();
		static void HandlerWindowSizeChanged();

	public:
		//HID message handlers
		static void HandlerMouseMove();
		static void HandlerMouseButtonDown();
		static void HandlerMouseButtonUp();

		static void HandlerKeyboardKeyDown();
		static void HandlerKeyboardKeyUp();
		static void HandlerKeyboardChar();

	private:
		//Internal HID mouse movement handling
		static void HandlerMouseMoveNormal();
		static void HandlerMouseMoveDragging();
		static bool HandlerMouseMoveDraggingUpdateCursor(_INOUT CWindowAbstract& rWindow);
		static bool HandlerMouseMoveDraggingTry(_INOUT CWindowAbstract& rWindow);

	private:
		//Internal event triggers, these dispatch events to actual GUI windows
		static void FireEventMouseMove       (_INOUT CWindowAbstract& rWindow);
		static void FireEventsMouseButtonDown(_INOUT CWindowAbstract& rWindow);
		static void FireEventsMouseButtonUp  (_INOUT CWindowAbstract& rWindow);
		static void FireEventsMousePresence  (_INOUT CWindowAbstract* pWindow);
		static void FireEventsKeyFocus       (_INOUT CWindowAbstract& rWindow);

	private:
		struct SObjects 
		{
			struct SGUI
			{
				CModule* pModule;

				struct SElement
				{
					CWindowAbstract* pFocused;
					CWindowAbstract* pMouseCaptured;

					CWindowAbstract* pHot;

					Float2 CursorPositionLocal;
					Float2 CursorPositionLocalDrag;
					Float2 CursorPositionDrag;

					bool  bDragging;
					DWORD bDraggingEgdeMask;

					Float2 SizeDrag;

				} Element;

			} GUI;

		} static s_Objects;

	private:
		CDispatcher();
		~CDispatcher();
	};
};


