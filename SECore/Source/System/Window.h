#pragma once

#include "../Rendering/Renderer.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace System
{
	typedef void (*HANDLER_EVENT)();

	class SECORE_API CWindow
	{
	public:
		static void          HandlerSizeChanged(_IN HANDLER_EVENT pHandler) { s_State.pSizeChanged = pHandler; }
		static HANDLER_EVENT HandlerSizeChanged()                           { return s_State.pSizeChanged; }

	public:
		static bool IsActive() { return s_State.bActive; }

	public:
		static HWND Handle() { return s_Runtime.hWnd; }
		static HDC  DC()     { return s_Runtime.hDC; }

	public:
		static void Show();
		static void Hide();
		static void Update();
		static void Center();

	public:
		static bool Create(_IN HWND hParent = NULL);
		static bool Destroy();
		static bool ClassRegister();

	public:
		static inline void SwapBuffers() { ::SwapBuffers(s_Runtime.hDC); }

	public:
		static void MessageLoopEnter();
		static void OnSizeChanged();
		static LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		struct SRuntime
		{
			HGLRC hRC;
			HDC   hDC;
			HWND  hWnd;

		} static s_Runtime;

		struct SState
		{
			bool bActive;

			HANDLER_EVENT pSizeChanged;

		} static s_State;

	private:
		CWindow();
		~CWindow();
	};
};