#pragma once

#include "Window.h"

namespace System
{
	typedef void (*HANDLER_CLEANUP)();
	typedef void (*HANDLER_RENDER_GUI)();

	class SECORE_API CSystem
	{
	public:
		static void            HandlerCleanUp(_IN HANDLER_CLEANUP pHandlerCleanUp) { s_Settings.pHandlerCleanUp = pHandlerCleanUp; }
		static HANDLER_CLEANUP HandlerCleanUp()                                    { return s_Settings.pHandlerCleanUp; }

	public:
		static inline HANDLER_RENDER_GUI HandlerGUIRender()                                { return s_Settings.pHandlerGUIRender; }
		static inline void               HandlerGUIRender(_IN HANDLER_RENDER_GUI pHandler) { s_Settings.pHandlerGUIRender = pHandler; }

	public:
		static HINSTANCE Instance() { return s_Runtime.hInstance; }

	public:
		static CString DllSuffix();
		static CString Version();
		static CInt    VersionNumber();

	public:
		static void Initialize(_IN CString& rSettingsFilename, _IN HINSTANCE hInstance);

		static void Shutdown(); 
		static void ShutdownOnError(_IN CString& rErrorMessage); 

	public:
		static void SystemInfoWriteToLog();
		static bool FullscreenSwitchTo();

	private:
		struct SRuntime
		{
			HINSTANCE hInstance;

		} static s_Runtime;

		struct SSettings
		{
			HANDLER_RENDER_GUI pHandlerGUIRender;
			HANDLER_CLEANUP pHandlerCleanUp;

		} static s_Settings;

	private:
		CSystem();
		~CSystem();
	};
};