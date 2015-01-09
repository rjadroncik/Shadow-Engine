#include "System.h"

#include <SCFTimer.h>

#include "Settings.h"

#include "HID.h"
#include "HIDState.h"

#include "RTLoop.h"

#include "Scripting.h"

#include "../Simulation/Simulation.h"
#include "../Simulation/PhysicsSolver.h"

#include "../Rendering/RendererDGC.h"
#include "../Rendering/RendererIGP.h"

#include "../Resources/DataPool.h"
#include "../Resources/Manager.h"

using namespace System;
using namespace Resources;

CSystem::SRuntime  CSystem::s_Runtime;
CSystem::SSettings CSystem::s_Settings;

void CSystem::Initialize(_IN CString& rSettingsFilename, _IN HINSTANCE hInstance)
{
/*	#ifdef _DEBUG
	{
		CObject::TrackingStart();
	}
	#endif*/

	CEventLog::Filename(STRING("EventLog.xml"));
	CEventLog::BlockNew(STRING("Starting Shadow Engine v") + CSystem::Version());
	{
		//Reset system settings
		s_Settings.pHandlerGUIRender = NULL;
		s_Settings.pHandlerCleanUp = NULL;

		//Save instance id for later use
		s_Runtime.hInstance = hInstance; 

		//Write information about CPU, memory, motherboard, ... into log file for debugging
		//TODO: This information needs to be processed & used to automatically tweak the engine for optimal performance 
		SystemInfoWriteToLog();

		//Init SCF components
		CEventLog::EventNew(STRING("Initializing SCFMathematics"));
		CEventLog::EventNew(STRING("Initializing SCFTimer"));
		SCFTimer::TimerInitialize();

		//Prepare scripting
		CScripting::Initialize(GetModuleHandle((STRING("SECore") + DllSuffix() + STRING(".dll")).Value()));

		//Load settings
		CSettings::Load(rSettingsFilename);

		//Initialize data storage
		CDataPool::Initialize();
		CManager::Initialize();

		//Prepare window
		CWindow::ClassRegister();
		CWindow::Create();

		//Initialize OpenGL renderer - initial state, queries, extensions, code-path
		COpenGL::Initialize();
		if (!COpenGL15::Initialize() || 
		    !COpenGL20::Initialize()) { ShutdownOnError(STRING("Initialization of graphics hardware failed!\nGraphics card does not support all necessary features.\nShutting down...")); }

		//Init classes
		CHID::Initialize();
		CHIDState::Initialize();
		CMaterialUsage::Initialize();

		switch (CRendererSettings::CodePath())
		{
		case CodePathOpenGL20DGC: { if (!CRendererDGC::Initialize()) { ShutdownOnError(STRING("Initialization of renderer failed!\nShutting down...")); } break; }
		case CodePathOpenGL20IGP: { if (!CRendererIGP::Initialize()) { ShutdownOnError(STRING("Initialization of renderer failed!\nShutting down...")); } break; }
		}
	}
	CEventLog::BlockClose();

	if (CError::Last())
	{
		switch (CError::Last())
		{
		case ErrorHardwareUnsupported:
			{
				CSystem::ShutdownOnError(STRING("An error ocurred during during initialization.\r\nIt seems your hardware does not support all required features."));
				break;
			}
		default:
			{
				CSystem::ShutdownOnError(STRING("An unhandled error occured during initialization."));
				break;
			}
		}
	}
}

void CSystem::ShutdownOnError(_IN CString& rErrorMessage) 
{
	CEventLog::BlockNew(STRING("Shutting down on error"));
	CEventLog::EventNew(rErrorMessage);
	CEventLog::Finish();

	MessageBox(NULL, rErrorMessage.Value(), SCFTEXT("Shutting down on error"), MB_ICONERROR);
	ExitProcess((SCF::UINT)-1);
}

void CSystem::Shutdown()
{
	if (!CRTLoop::IsRunning()) { return; }

	CEventLog::BlockNew(STRING("Shutting down"));

	CRTLoop::Pause();
	CWindow::Hide();

	if (CSystem::HandlerCleanUp()) { CSystem::HandlerCleanUp()(); }
	CWindow::Destroy();

	CEventLog::BlockClose();
	CEventLog::Finish();

	/*#ifdef _DEBUG
	{
		CObject::TrackingStop();

		CVector* pResults = &CObjectBounded::TrackingResults();

		CStreamFileWrite StreamWrite(STRING("ObejctsLeaked.txt"));
		CStreamWriteText StreamWriteText(StreamWrite);

		for (SCF::UINT i = 0; i < pResults->Size(); i++)
		{
			StreamWriteText.PutString(pResults->At(i).ToString() + STRING("\n"));
		}

		pResults->AllRemove();
	}
	#endif*/

	ExitProcess(0);
}

bool CSystem::FullscreenSwitchTo()
{
	DEVMODE DevMode;
	memset(&DevMode, 0, sizeof(DevMode));

	if (!EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS, &DevMode))
	{
		SCFError(ErrorDisplaySettingsEnumFailed);
		return FALSE;
	}

	DevMode.dmPelsWidth  = CSettings::CDisplay::Width();
	DevMode.dmPelsHeight = CSettings::CDisplay::Height();
	DevMode.dmBitsPerPel = CSettings::CDisplay::BitDepth();
	DevMode.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	int result = ChangeDisplaySettings(&DevMode, CDS_FULLSCREEN);	

	if (result != DISP_CHANGE_SUCCESSFUL)
	{
		SCFError(ErrorDisplayModeSetFailed); 
		return FALSE;
	}

	return TRUE;
}

CString CSystem::DllSuffix()
{
	#ifdef _DEBUG
		return STRING("D");
	#else
		#ifdef _SSE
			#ifdef _SSE3
				return STRING("SSE3");
			#else
				return STRING("SSE");
			#endif
		#else
			return STRING("");
		#endif
	#endif
}
CString CSystem::Version()
{
	#ifdef _DEBUG
		return STRING("0.0951a Debug");
	#else
		#ifdef _SSE
			#ifdef _SSE3
				return STRING("0.0951a SSE3");
			#else
				return STRING("0.0951a SSE");
			#endif
		#else
			return STRING("0.0951a");
		#endif
	#endif
}
CInt CSystem::VersionNumber()
{
	return 0x0000951a;
}

void CSystem::SystemInfoWriteToLog()
{
	CEventLog::BlockNew(STRING("SystemInfo"));
	{
		CString Report = CInfoCPU::Report();

		CStreamStringRead ReportStream(Report);
		CStreamReadTextUCS2 ReportStreamText(ReportStream);

		CString Line;
		while (ReportStreamText.GetLine(Line))
		{
			CEventLog::EventNew(Line);
		}
	}
	CEventLog::BlockClose();
}
