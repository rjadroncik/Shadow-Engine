#include "Window.h"
#include "System.h"

#include "Settings.h"

#include "HID.h"
#include "HIDState.h"

#include "RTLoop.h"

#include "Scripting.h"

#include "../Simulation/Simulation.h"

#include "../Rendering/RendererDGC.h"

using namespace System;
using namespace Resources;

CWindow::SRuntime CWindow::s_Runtime = { NULL, NULL, NULL };
CWindow::SState   CWindow::s_State = { FALSE, NULL };

void CWindow::Center()
{
	RECT DesktopRect;
	RECT WindowRect;

	HWND hDesktop = GetDesktopWindow();
	
	GetWindowRect(hDesktop, &DesktopRect);
	GetWindowRect(s_Runtime.hWnd, &WindowRect);

	SetWindowPos(s_Runtime.hWnd, NULL,
		(DesktopRect.right - DesktopRect.left - WindowRect.right + WindowRect.left) / 2,
		(DesktopRect.bottom - DesktopRect.top - WindowRect.bottom + WindowRect.top) / 2,
		0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW);				
}

void CWindow::Show()
{
	CEventLog::EventNew(STRING("Showing window"));
	ShowWindow         (s_Runtime.hWnd, SW_SHOW);	
	SetForegroundWindow(s_Runtime.hWnd);

	Update();
}

void CWindow::Hide()
{
	CEventLog::EventNew(STRING("Hiding window"));
	ShowWindow(s_Runtime.hWnd, SW_HIDE);	
}

void CWindow::Update()
{
	CWindow::OnSizeChanged();
}

bool CWindow::ClassRegister()
{
	CEventLog::EventNew(STRING("Registering window"));

	WNDCLASSEX wc;

	wc.cbSize        = sizeof(WNDCLASSEX);			
	wc.style         = CS_OWNDC;	
	wc.lpfnWndProc   = (WNDPROC)CWindow::WindowProcedure;			
	wc.cbClsExtra    = 0;							
	wc.cbWndExtra    = 0;							
	wc.hInstance     = CSystem::Instance();					
	wc.hIcon         = LoadIcon(CSystem::Instance(), MAKEINTRESOURCE(102));
	wc.hCursor       = NULL;//LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;					
	wc.lpszMenuName  = NULL;						
	wc.lpszClassName = SCFTEXT("ShadowEngineClass");				
	wc.hIconSm       = NULL;							

	if (!RegisterClassEx(&wc))  
	{
		SCFError(ErrorWindowClassRegisterFailed);
		return FALSE;
	}

	return TRUE;
}

bool CWindow::Destroy()
{
	CEventLog::EventNew(STRING("Destroying window"));

	ChangeDisplaySettings(NULL, 0);

	if (s_Runtime.hRC)
	{
		if (!wglMakeCurrent(NULL, NULL))     
		{
			SCFError(ErrorGLContextMakeCurrentFailed);
			return FALSE;
		}
		if (!wglDeleteContext(s_Runtime.hRC))
		{
			SCFError(ErrorGLContextDeleteFailed);
			return FALSE;
		}

		s_Runtime.hRC = NULL; 
	}

	if (s_Runtime.hDC && !ReleaseDC(s_Runtime.hWnd, s_Runtime.hDC))
	{
		SCFError(ErrorWindowDCReleaseFailed);
		return FALSE;
	}

	if (s_Runtime.hWnd && !DestroyWindow(s_Runtime.hWnd)) 
	{
		SCFError(ErrorWindowDestroyFailed);
		return FALSE;
	}

	if (!UnregisterClass(SCFTEXT("ShadowEngineClass"), CSystem::Instance())) 
	{
		SCFError(ErrorWindowClassUnregisterFailed);
		return FALSE;
	}

	return TRUE;
}

bool CWindow::Create(_IN HWND hParent)
{
	CEventLog::EventNew(STRING("Creating window"));

	if (hParent)
	{

		return FALSE;
	}
	else
	{
		UINT  uiPixelFormat = 0;
		DWORD dwExStyle     = WS_EX_APPWINDOW;
		DWORD dwStyle       = NULL;

		if (CSettings::CDisplay::FullScreen())
		{
			CSystem::FullscreenSwitchTo();
			dwStyle = WS_POPUP;				   
		}
		else { dwStyle = WS_OVERLAPPEDWINDOW; }

		//Prepare rectangle of the correct window size
		RECT Rect = { 0, 0, CSettings::CDisplay::Width(), CSettings::CDisplay::Height() };
		AdjustWindowRectEx(&Rect, dwStyle, FALSE, dwExStyle);

		//Create window
		s_Runtime.hWnd = CreateWindowEx(dwExStyle, SCFTEXT("ShadowEngineClass"), SCFTEXT("Shadow Engine"), dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, Rect.left, Rect.top, Rect.right - Rect.left, Rect.bottom - Rect.top, NULL, NULL, CSystem::Instance(), NULL);
		if (!s_Runtime.hWnd) 
		{ 
			SCFError(ErrorWindowCreateFailed); 
			return FALSE;
		}

		static PIXELFORMATDESCRIPTOR pfd = { sizeof(PIXELFORMATDESCRIPTOR), 1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA, (BYTE)CSettings::CDisplay::BitDepth(), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 8, 0, PFD_MAIN_PLANE, 0, 0, 0, 0	};

		//Set pixel format
		if ((s_Runtime.hDC = GetDC(s_Runtime.hWnd)) == 0)                  
		{
			SCFError(ErrorWindowDCGetFailed);
			return FALSE;
		}
		if ((uiPixelFormat = ChoosePixelFormat(s_Runtime.hDC, &pfd)) == 0)
		{
			SCFError(ErrorPixelFormatChooseFailed);
			return FALSE;
		}
		if (!SetPixelFormat(s_Runtime.hDC, uiPixelFormat, &pfd))
		{
			SCFError(ErrorPixelFormatSetFailed);
			return FALSE;
		}

		//Prepare OpenGL rendering context
		if ((s_Runtime.hRC = wglCreateContext(s_Runtime.hDC)) == 0) 
		{ 
			SCFError(ErrorGLContextCreateFailed);
			return FALSE;
		}
		if (!wglMakeCurrent(s_Runtime.hDC, s_Runtime.hRC))
		{
			SCFError(ErrorGLContextMakeCurrentFailed); 
			return FALSE;
		}

		return TRUE;
	}
}

void CWindow::OnSizeChanged()
{
	RECT RectClient;
	GetClientRect(s_Runtime.hWnd, &RectClient);

	CSettings::CDisplay::Width(RectClient.right);
	CSettings::CDisplay::Height(RectClient.bottom);

	if (Simulation::CSimulation::Scene() && Simulation::CSimulation::Scene()->CameraCurrent())
	{
		//Simulation::CSimulation::Scene()->CameraCurrent()->Viewport(50, 50, CSettings::CDisplay::Width() - 100, CSettings::CDisplay::Height() - 100);
		Simulation::CSimulation::Scene()->CameraCurrent()->Viewport(0, 0, CSettings::CDisplay::Width(), CSettings::CDisplay::Height());
	}

	if (s_State.pSizeChanged) { s_State.pSizeChanged(); }
}

LRESULT CALLBACK CWindow::WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	SCF_UNREFERENCED_PARAMETER(lParam);

	switch (message) 
	{
	case WM_LBUTTONDOWN: { CHIDState::CMouse::s_uiButtonLastDown = VK_LBUTTON; CHIDState::CMouse::s_baButtonsDown[VK_LBUTTON] = TRUE; if (CHID::CMouse::HandlerButtonDown()) { CHID::CMouse::HandlerButtonDown()(); } break; }
	case WM_MBUTTONDOWN: { CHIDState::CMouse::s_uiButtonLastDown = VK_MBUTTON; CHIDState::CMouse::s_baButtonsDown[VK_MBUTTON] = TRUE; if (CHID::CMouse::HandlerButtonDown()) { CHID::CMouse::HandlerButtonDown()(); } break; }
	case WM_RBUTTONDOWN: { CHIDState::CMouse::s_uiButtonLastDown = VK_RBUTTON; CHIDState::CMouse::s_baButtonsDown[VK_RBUTTON] = TRUE; if (CHID::CMouse::HandlerButtonDown()) { CHID::CMouse::HandlerButtonDown()(); } break; }
	case WM_XBUTTONDOWN: 
		{
			if (HIWORD(wParam) == XBUTTON1) { CHIDState::CMouse::s_uiButtonLastDown = VK_XBUTTON1; CHIDState::CMouse::s_baButtonsDown[VK_XBUTTON1] = TRUE; if (CHID::CMouse::HandlerButtonDown()) { CHID::CMouse::HandlerButtonDown()(); } break; }
			else                            { CHIDState::CMouse::s_uiButtonLastDown = VK_XBUTTON2; CHIDState::CMouse::s_baButtonsDown[VK_XBUTTON2] = TRUE; if (CHID::CMouse::HandlerButtonDown()) { CHID::CMouse::HandlerButtonDown()(); } break; }
		}
	case WM_LBUTTONUP: { CHIDState::CMouse::s_uiButtonLastUp = VK_LBUTTON; CHIDState::CMouse::s_baButtonsDown[VK_LBUTTON] = FALSE; if (CHID::CMouse::HandlerButtonUp()) { CHID::CMouse::HandlerButtonUp()(); } break; }
	case WM_MBUTTONUP: { CHIDState::CMouse::s_uiButtonLastUp = VK_MBUTTON; CHIDState::CMouse::s_baButtonsDown[VK_MBUTTON] = FALSE; if (CHID::CMouse::HandlerButtonUp()) { CHID::CMouse::HandlerButtonUp()(); } break; }
	case WM_RBUTTONUP: { CHIDState::CMouse::s_uiButtonLastUp = VK_RBUTTON; CHIDState::CMouse::s_baButtonsDown[VK_RBUTTON] = FALSE; if (CHID::CMouse::HandlerButtonUp()) { CHID::CMouse::HandlerButtonUp()(); } break; }
	case WM_XBUTTONUP: 
		{
			if (HIWORD(wParam) == XBUTTON1) { CHIDState::CMouse::s_uiButtonLastUp = VK_XBUTTON1; CHIDState::CMouse::s_baButtonsDown[VK_XBUTTON1] = FALSE; if (CHID::CMouse::HandlerButtonUp()) { CHID::CMouse::HandlerButtonUp()(); } break; }
			else                            { CHIDState::CMouse::s_uiButtonLastUp = VK_XBUTTON2; CHIDState::CMouse::s_baButtonsDown[VK_XBUTTON2] = FALSE; if (CHID::CMouse::HandlerButtonUp()) { CHID::CMouse::HandlerButtonUp()(); } break; }
		}

	case WM_MOUSEMOVE:
		{
			CHIDState::CMouse::s_CursorPosition[0] = (float)LOWORD(lParam);
			CHIDState::CMouse::s_CursorPosition[1] = (float)HIWORD(lParam);

			if (CHID::CMouse::HandlerMove()) { CHID::CMouse::HandlerMove()(); }
			break; 
		}

	case WM_KEYDOWN:
		{
			CHIDState::CKeyboard::s_uiKeyLastDown   = (UINT)wParam;
			CHIDState::CKeyboard::s_uiKeyLastRepeat = (BYTE)lParam;
			CHIDState::CKeyboard::s_usCharLast      = 0;

			CHIDState::CKeyboard::s_baKeysDown[(UINT)wParam] = TRUE;

			if (CHID::CKeyboard::HandlerKeyDown()) { CHID::CKeyboard::HandlerKeyDown()(); }
			break;
		}
	case WM_KEYUP:
		{
			CHIDState::CKeyboard::s_uiKeyLastDown   = (UINT)wParam;
			CHIDState::CKeyboard::s_uiKeyLastRepeat = 1;
			CHIDState::CKeyboard::s_usCharLast      = 0;

			CHIDState::CKeyboard::s_baKeysDown[(UINT)wParam] = FALSE;

			if (CHID::CKeyboard::HandlerKeyUp()) { CHID::CKeyboard::HandlerKeyUp()(); }
			break;
		}
	case WM_CHAR:
		{
			CHIDState::CKeyboard::s_uiKeyLastDown   = 0;
			CHIDState::CKeyboard::s_uiKeyLastRepeat = (BYTE)lParam;
			CHIDState::CKeyboard::s_usCharLast      = (TCHAR)wParam;

			if (wParam < 32) { break; }
			if (CHID::CKeyboard::HandlerChar()) { CHID::CKeyboard::HandlerChar()(); }
			break;
		}

	case WM_ACTIVATE: 
		{
			if (HIWORD(wParam)) { s_State.bActive = FALSE; }
			else                { s_State.bActive = TRUE; }

			return FALSE;
		}
	case WM_SYSCOMMAND:
		{
			if ((wParam == SC_RESTORE) || (wParam == SC_MAXIMIZE))
			{
				PostMessage(hWnd, WM_SIZE, NULL,  NULL);
			}

			return DefWindowProc(hWnd, message, wParam, lParam);
		}

	case WM_SIZE:
	case WM_SIZING: 
		{ 
			if (!s_State.bActive)  { break; }

			CWindow::OnSizeChanged();
			return TRUE; 
		}

	case WM_CLOSE: 
	case WM_DESTROY: { CSystem::Shutdown(); break; }

	default: { return DefWindowProc(hWnd, message, wParam, lParam); }
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void CWindow::MessageLoopEnter()
{
	MSG msg;
	while (CRTLoop::IsRunning())  
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))  
		{
			if (msg.message == WM_QUIT) { CSystem::Shutdown(); }
			else                    
			{
				if ((msg.message >= WM_KEYFIRST) && (msg.message < WM_KEYLAST) && CHID::CKeyboard::TypingEnabled()) 
				{ 
					TranslateMessage(&msg);
				}

				DispatchMessage(&msg);
			}
		}
		else 
		{
			if (CWindow::IsActive()) { CRTLoop::Next(); }
			else { SleepEx(200, TRUE); }
		}
	}
}