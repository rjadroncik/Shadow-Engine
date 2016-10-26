#include "Dispatcher.h"

#include "../../../SECore/Source/Simulation/Simulation.h"
#include "../../../SECore/Source/System/System.h"
#include "../../../SECore/Source/System/Settings.h"
#include "../../../SECore/Source/System/Statistics.h"
#include "../../../SECore/Source/System/HID.h"
#include "../../../SECore/Source/System/Scripting.h"
#include "../../../SECore/Source/Resources/Manager.h"
#include "../../../SECore/Source/Resources/DataPool.h"

#include "Label.h"
#include "Module.h"

#include <SCFDebugging.h>
#include <SCFObjectExtensionsPrivate.h>

using namespace GUI;
using namespace System;
using namespace SCFDebugging;

CDispatcher::SObjects CDispatcher::s_Objects;

void CDispatcher::HandlerKeyboardKeyDown()
{
	if (s_Objects.GUI.Element.pFocused)
	{
		SEventKey EventKey = { (BYTE)CHIDState::CKeyboard::KeyLastDown(), 0 };

		for (UINT i = 0; i < CHIDState::CKeyboard::KeyLastRepeat(); i++)
		{
			s_Objects.GUI.Element.pFocused->OnKeyPressed(EventKey);
		}
	}
}

void CDispatcher::HandlerKeyboardKeyUp()
{
	if (s_Objects.GUI.Element.pFocused)
	{
		SEventKey EventKey = { (BYTE)CHIDState::CKeyboard::KeyLastUp(), 0 };

		s_Objects.GUI.Element.pFocused->OnKeyReleased(EventKey);
	}
}

void CDispatcher::HandlerKeyboardChar()
{
	if (s_Objects.GUI.Element.pFocused)
	{
		SEventKey EventKey = { 0, (TCHAR)CHIDState::CKeyboard::CharLast() };

		for (UINT i = 0; i < CHIDState::CKeyboard::KeyLastRepeat(); i++)
		{
			s_Objects.GUI.Element.pFocused->OnKeyPressed(EventKey);
		}
	}
}

void CDispatcher::Initialize()
{
	s_Objects.GUI.pModule = NULL;

	s_Objects.GUI.Element.pHot           = NULL;
	s_Objects.GUI.Element.pFocused       = NULL;
	s_Objects.GUI.Element.pMouseCaptured = NULL;

	s_Objects.GUI.Element.bDragging         = FALSE;
	s_Objects.GUI.Element.bDraggingEgdeMask = 0;

 	CSystem::HandlerGUIRender(CDispatcher::HandlerRender);
	System::CWindow::HandlerSizeChanged(CDispatcher::HandlerWindowSizeChanged);

	CHID::CMouse::HandlerMove      (CDispatcher::HandlerMouseMove);
	CHID::CMouse::HandlerButtonDown(CDispatcher::HandlerMouseButtonDown);
	CHID::CMouse::HandlerButtonUp  (CDispatcher::HandlerMouseButtonUp);
	
	CHID::CKeyboard::HandlerKeyDown(CDispatcher::HandlerKeyboardKeyDown);
	CHID::CKeyboard::HandlerKeyUp  (CDispatcher::HandlerKeyboardKeyUp);
	CHID::CKeyboard::HandlerChar   (CDispatcher::HandlerKeyboardChar);

	CHID::CKeyboard::TypingEnabled(TRUE);
}

void CDispatcher::ElementMouseCapture(_IN CWindowAbstract& pWindow)
{ 
	SEventMouse EventMouse;
	CopyVector2(EventMouse.Position, s_Objects.GUI.Element.CursorPositionLocal);

	if (s_Objects.GUI.Element.pMouseCaptured) { s_Objects.GUI.Element.pMouseCaptured->OnMouseCaptuereLost(EventMouse); }

	s_Objects.GUI.Element.pMouseCaptured = (CWindowAbstract*)&pWindow;
}

void CDispatcher::ElementMouseRelease() 
{
	SEventMouse EventMouse;
	CopyVector2(EventMouse.Position, s_Objects.GUI.Element.CursorPositionLocal);

	if (s_Objects.GUI.Element.pMouseCaptured) { s_Objects.GUI.Element.pMouseCaptured->OnMouseCaptuereLost(EventMouse); }

	s_Objects.GUI.Element.pMouseCaptured = NULL;
}

void CDispatcher::HandlerMouseMove()
{
	if (!s_Objects.GUI.pModule) { return; }

	s_Objects.GUI.pModule->Cursor.Position(CHIDState::CMouse::CursorPosition()[0], CHIDState::CMouse::CursorPosition()[1]);

	if (s_Objects.GUI.Element.bDragging) { HandlerMouseMoveDragging(); }
	else                                 { HandlerMouseMoveNormal(); }
}

void CDispatcher::HandlerMouseMoveDragging()
{
	if (!s_Objects.GUI.pModule) { return; }

	if (CHIDState::CMouse::ButtonDown(VK_LBUTTON))
	{
		if (!s_Objects.GUI.Element.bDraggingEgdeMask)
		{
			Float2 NewPos;
			SubtractVectors2(NewPos, s_Objects.GUI.pModule->Cursor.Position(), s_Objects.GUI.Element.CursorPositionLocalDrag);

			s_Objects.GUI.Element.pHot->Position(NewPos);
			s_Objects.GUI.Element.pHot->OnWindowMove();
		}
		else
		{
			{
				Float2 NewSize;
				SubtractVectors2(NewSize, s_Objects.GUI.pModule->Cursor.Position(), s_Objects.GUI.Element.CursorPositionDrag);
				AddVectors2(NewSize, NewSize, s_Objects.GUI.Element.SizeDrag);

				if (s_Objects.GUI.Element.bDraggingEgdeMask & AlignRight)
				{
					s_Objects.GUI.Element.pHot->Size(NewSize[0], s_Objects.GUI.Element.pHot->Size()[1]);
				}

				if (s_Objects.GUI.Element.bDraggingEgdeMask & AlignBottom)
				{
					s_Objects.GUI.Element.pHot->Size(s_Objects.GUI.Element.pHot->Size()[0], NewSize[1]);
				}
			}

			{
				Float2 NewPos;
				SubtractVectors2(NewPos, s_Objects.GUI.pModule->Cursor.Position(), s_Objects.GUI.Element.CursorPositionLocalDrag);
				
				Float2 NewSize;
				SubtractVectors2(NewSize, s_Objects.GUI.pModule->Cursor.Position(), s_Objects.GUI.Element.CursorPositionDrag);
				SubtractVectors2(NewSize, s_Objects.GUI.Element.SizeDrag, NewSize);

				if (s_Objects.GUI.Element.bDraggingEgdeMask & AlignLeft)
				{
					s_Objects.GUI.Element.pHot->Position(NewPos[0],  s_Objects.GUI.Element.pHot->Position()[1]);
					s_Objects.GUI.Element.pHot->Size    (NewSize[0], s_Objects.GUI.Element.pHot->Size()[1]);
				}

				if (s_Objects.GUI.Element.bDraggingEgdeMask & AlignTop)
				{
					s_Objects.GUI.Element.pHot->Position(s_Objects.GUI.Element.pHot->Position()[0], NewPos[1]);
					s_Objects.GUI.Element.pHot->Size    (s_Objects.GUI.Element.pHot->Size()[0],     NewSize[1]);
				}
			}

			s_Objects.GUI.Element.pHot->OnWindowSize();

			if ((s_Objects.GUI.Element.bDraggingEgdeMask & AlignLeft) || 
				(s_Objects.GUI.Element.bDraggingEgdeMask & AlignTop))
			{
				s_Objects.GUI.Element.pHot->OnWindowMove();
			}
		}
	}
	else
	{
		s_Objects.GUI.Element.bDragging         = FALSE;
		s_Objects.GUI.Element.bDraggingEgdeMask = 0;
	}
}

bool CDispatcher::HandlerMouseMoveDraggingUpdateCursor(_INOUT CWindowAbstract& rWindow)
{
	if (!s_Objects.GUI.pModule) { return FALSE; }

	if (rWindow.IsDraggable())
	{
		if (rWindow.Resizable())
		{
			s_Objects.GUI.Element.bDraggingEgdeMask = ((CWindow&)rWindow).CanStartSizing(s_Objects.GUI.Element.CursorPositionLocal);

			switch (s_Objects.GUI.Element.bDraggingEgdeMask)
			{
			case AlignLeft  | AlignBottom:
			case AlignRight | AlignTop:    { s_Objects.GUI.pModule->Cursor.Use(CursorSizeNESW); break; }
			case AlignLeft  | AlignTop:
			case AlignRight | AlignBottom: { s_Objects.GUI.pModule->Cursor.Use(CursorSizeNWSE); break; }

			case AlignLeft:
			case AlignRight:  { s_Objects.GUI.pModule->Cursor.Use(CursorSizeWE); break; }
			case AlignTop:
			case AlignBottom: { s_Objects.GUI.pModule->Cursor.Use(CursorSizeNS); break; }
			}

		} else { s_Objects.GUI.Element.bDraggingEgdeMask = 0; }
	}

	return FALSE;
}

bool CDispatcher::HandlerMouseMoveDraggingTry(_INOUT CWindowAbstract& rWindow)
{
	if (!s_Objects.GUI.pModule) { return FALSE; }

	if (rWindow.IsDraggable())
	{
		if (CHIDState::CMouse::ButtonLastDown() == VK_LBUTTON)
		{
			if (s_Objects.GUI.Element.bDraggingEgdeMask != 0)
			{
				if (!rWindow.Resizable()) { return FALSE; }

				s_Objects.GUI.Element.bDragging = TRUE;

				CopyVector2(s_Objects.GUI.Element.CursorPositionLocalDrag, s_Objects.GUI.Element.CursorPositionLocal);
				CopyVector2(s_Objects.GUI.Element.CursorPositionDrag, s_Objects.GUI.pModule->Cursor.Position());
				CopyVector2(s_Objects.GUI.Element.SizeDrag, rWindow.Size());

				s_Objects.GUI.pModule->ChildMakeTopmost(rWindow);
				return TRUE;
			}

			if (((CWindow&)rWindow).CanStartDragging(s_Objects.GUI.Element.CursorPositionLocal))
			{
				s_Objects.GUI.Element.bDragging         = TRUE;
				s_Objects.GUI.Element.bDraggingEgdeMask = 0;

				CopyVector2(s_Objects.GUI.Element.CursorPositionLocalDrag, s_Objects.GUI.Element.CursorPositionLocal);

				s_Objects.GUI.pModule->ChildMakeTopmost(rWindow);
				return TRUE;
			}
		}
	}

	return FALSE;
}

void CDispatcher::HandlerMouseMoveNormal()
{
	if (!s_Objects.GUI.pModule) { return; }

	s_Objects.GUI.pModule->Cursor.Use(CursorArrow);

	if (s_Objects.GUI.Element.pMouseCaptured)
	{
		s_Objects.GUI.pModule->MouseLocalPos(s_Objects.GUI.pModule->Cursor.Position(), *s_Objects.GUI.Element.pMouseCaptured, s_Objects.GUI.Element.CursorPositionLocal);
	
		FireEventsMousePresence( s_Objects.GUI.Element.pMouseCaptured);
		FireEventMouseMove     (*s_Objects.GUI.Element.pMouseCaptured);
	}
	else
	{
		//Get the element which is located underneath the mouse (if any)
		CWindowAbstract* pWindow = s_Objects.GUI.pModule->MouseOver(s_Objects.GUI.pModule->Cursor.Position(), &s_Objects.GUI.Element.CursorPositionLocal);

		if (pWindow && (pWindow == s_Objects.GUI.Element.pHot))
		{
			HandlerMouseMoveDraggingUpdateCursor(*pWindow);
			FireEventMouseMove(*pWindow);
		}
		else { FireEventsMousePresence(pWindow); }
	}
}

void CDispatcher::HandlerMouseButtonDown()
{
	if (!s_Objects.GUI.pModule) { return; }

	if (s_Objects.GUI.Element.pMouseCaptured)
	{
		s_Objects.GUI.pModule->MouseLocalPos(s_Objects.GUI.pModule->Cursor.Position(), *s_Objects.GUI.Element.pMouseCaptured, s_Objects.GUI.Element.CursorPositionLocal);

		FireEventsKeyFocus       (*s_Objects.GUI.Element.pMouseCaptured);
		FireEventsMouseButtonDown(*s_Objects.GUI.Element.pMouseCaptured);
	}
	else
	{
		//Get the element which is located underneath the mouse (if any)
		CWindowAbstract* pWindow = s_Objects.GUI.pModule->MouseOver(s_Objects.GUI.pModule->Cursor.Position(), &s_Objects.GUI.Element.CursorPositionLocal);
		if (pWindow)
		{
			if (pWindow == s_Objects.GUI.Element.pHot)
			{
				HandlerMouseMoveDraggingTry(*pWindow);
			}

			FireEventsKeyFocus       (*pWindow);
			FireEventsMouseButtonDown(*pWindow);
		}
	}
}

void CDispatcher::HandlerMouseButtonUp()
{
	if (!s_Objects.GUI.pModule) { return; }

	if (s_Objects.GUI.Element.pMouseCaptured)
	{
		s_Objects.GUI.pModule->MouseLocalPos(s_Objects.GUI.pModule->Cursor.Position(), *s_Objects.GUI.Element.pMouseCaptured, s_Objects.GUI.Element.CursorPositionLocal);

		FireEventsMouseButtonUp(*s_Objects.GUI.Element.pMouseCaptured);
		FireEventsMousePresence( s_Objects.GUI.Element.pMouseCaptured);
	}
	else
	{
		//Get the element which is located underneath the mouse (if any)
		CWindowAbstract* pWindow = s_Objects.GUI.pModule->MouseOver(s_Objects.GUI.pModule->Cursor.Position(), &s_Objects.GUI.Element.CursorPositionLocal);
		if (pWindow)
		{
			FireEventsMouseButtonUp(*pWindow);
		}
	}
}

void CDispatcher::FireEventsMousePresence(_INOUT CWindowAbstract* pWindow)
{
	SEventMouse EventMouse;
	CopyVector2(EventMouse.Position, s_Objects.GUI.Element.CursorPositionLocal);

	if (s_Objects.GUI.Element.pHot) { s_Objects.GUI.Element.pHot->OnMouseLeave(EventMouse); }

	s_Objects.GUI.Element.pHot = pWindow;

	if (s_Objects.GUI.Element.pHot) { s_Objects.GUI.Element.pHot->OnMouseEnter(EventMouse); }
}

void CDispatcher::FireEventMouseMove(_INOUT CWindowAbstract& rWindow)
{
	SEventMouse EventMouse;
	CopyVector2(EventMouse.Position, s_Objects.GUI.Element.CursorPositionLocal);

	rWindow.OnMouseMove(EventMouse); 
}

void CDispatcher::FireEventsKeyFocus(_INOUT CWindowAbstract& rWindow)
{
	if (CHIDState::CMouse::ButtonLastDown() == VK_LBUTTON)
	{
		if (rWindow.Focusable())
		{
			if (s_Objects.GUI.Element.pFocused) { s_Objects.GUI.Element.pFocused->OnKeyFocusLost(SEventKey()); }

			s_Objects.GUI.Element.pFocused = &rWindow;

			if (s_Objects.GUI.Element.pFocused) 
			{
				s_Objects.GUI.Element.pFocused->OnKeyFocusReceived(SEventKey()); 
				//CUserInterface::CarretShow(TRUE);
			}
			//else { CUserInterface::CarretShow(FALSE); }
		}
	}
}

void CDispatcher::FireEventsMouseButtonDown(_INOUT CWindowAbstract& rWindow)
{
	SEventMouse EventMouse;
	CopyVector2(EventMouse.Position, s_Objects.GUI.Element.CursorPositionLocal);

	if (CHIDState::CMouse::ButtonLastDown() == VK_LBUTTON)
	{
		EventMouse.Data.ucButton = 0;
		rWindow.OnMouseButtonPressed(EventMouse);
	}
	if (CHIDState::CMouse::ButtonLastDown() == VK_RBUTTON)
	{
		EventMouse.Data.ucButton = 1;
		rWindow.OnMouseButtonPressed(EventMouse);
	}
}

void CDispatcher::FireEventsMouseButtonUp(_INOUT CWindowAbstract& rWindow)
{
	SEventMouse EventMouse;
	CopyVector2(EventMouse.Position, s_Objects.GUI.Element.CursorPositionLocal);

	if (CHIDState::CMouse::ButtonLastDown() == VK_LBUTTON)
	{
		EventMouse.Data.ucButton = 0;
		rWindow.OnMouseButtonReleased(EventMouse);
	}
	if (CHIDState::CMouse::ButtonLastDown() == VK_RBUTTON)
	{
		EventMouse.Data.ucButton = 1;
		rWindow.OnMouseButtonReleased(EventMouse);
	}
}


void CDispatcher::HandlerRender()
{
	if (s_Objects.GUI.pModule && Simulation::CSimulation::Scene()->CameraCurrent()) 
	{ 
		s_Objects.GUI.pModule->Render(); 
	}
}

void CDispatcher::HandlerWindowSizeChanged()
{
	if (s_Objects.GUI.pModule && Simulation::CSimulation::Scene()->CameraCurrent()) 
	{ 
		s_Objects.GUI.pModule->Size((float)Simulation::CSimulation::Scene()->CameraCurrent()->Viewport()[2],
									(float)Simulation::CSimulation::Scene()->CameraCurrent()->Viewport()[3]); 
	}
}
