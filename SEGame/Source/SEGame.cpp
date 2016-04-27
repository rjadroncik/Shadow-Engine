#include "SEGame.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	SCF_UNREFERENCED_PARAMETER(hModule);
	SCF_UNREFERENCED_PARAMETER(lpReserved);

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}


using namespace System;
using namespace Simulation;
using namespace Resources;
using namespace GUI;

void HandlerCleanUp()
{
	//Simplest possible clean up handler - delete all loaded objects, no custom clean up code
	CManager::ObjectsDelete();
}

void HandlerUpdateStart()
{
}

#include "../../SECore/Source/System/Statistics.h"
#include "../../SEGUI/Source/GUI/Label.h"

#include <SCFObjectExtensionsPrivate.h>

void HandlerUpdateEnd()
{
	BETAONLY(CLabel* s_pMemoryUsageValue = (CLabel*)CScripting::Object(STRING("Label.System.Memory_Usage_Value")));
	BETAONLY(CLabel* s_pObjectCountValue = (CLabel*)CScripting::Object(STRING("Label.System.Object_Count_Value")));
	BETAONLY(CLabel* s_pBlockCountValue  = (CLabel*)CScripting::Object(STRING("Label.System.Block_Count_Value")));

	BETAONLY(if (s_pMemoryUsageValue) { s_pMemoryUsageValue->Text().String(CInt64(SCFPrivate::CMemorySystem::AllocatedBytes() + CMemory::AllocatedBytes() + SCFPrivate::CFSBHeap::AllocatedBytesTotal()).ToString()); })
	BETAONLY(if (s_pObjectCountValue) { s_pObjectCountValue->Text().String(CInt(CObject::TotalSystemCount()).ToString()); })
	BETAONLY(if (s_pBlockCountValue)  { s_pBlockCountValue->Text().String (CInt(SCFPrivate::CMemorySystem::BlockCount() + CMemory::BlockCount() + SCFPrivate::CFSBHeap::BlockCountTotal()).ToString()); })

	CLabel* s_pRPSValue = (CLabel*)CScripting::Object(STRING("Label.Statistics.RPS_Value"));
	CLabel* s_pUPSValue = (CLabel*)CScripting::Object(STRING("Label.Statistics.UPS_Value"));

	if (s_pRPSValue) { s_pRPSValue->Text().String(CInt((int)CStatistics::CRealTime::RPS()).ToString()); }
	if (s_pUPSValue) { s_pUPSValue->Text().String(CInt((int)CStatistics::CRealTime::UPS()).ToString()); }

	CLabel* s_pShadowsValue = (CLabel*)CScripting::Object(STRING("Label.Optimizations.Shadows_Processed_Value"));
	CLabel* s_pObjectsValue = (CLabel*)CScripting::Object(STRING("Label.Optimizations.Objects_Processed_Value"));
	CLabel* s_pLightsValue  = (CLabel*)CScripting::Object(STRING("Label.Optimizations.Lights_Processed_Value"));

	if (s_pShadowsValue) { s_pShadowsValue->Text().String(CInt(CStatistics::CRendering::ProcessedShadows()).ToString()); }
	if (s_pObjectsValue) { s_pObjectsValue->Text().String(CInt(CStatistics::CRendering::ProcessedObjects()).ToString()); }
	if (s_pLightsValue)  { s_pLightsValue->Text().String (CInt(CStatistics::CRendering::ProcessedLights ()).ToString()); }
}

int SEGAME_API __stdcall SEGameRun(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SCF_UNREFERENCED_PARAMETER(hPrevInstance);
	SCF_UNREFERENCED_PARAMETER(lpCmdLine);
	SCF_UNREFERENCED_PARAMETER(nCmdShow);

	//Initialize system - create window, start up OpenGL & the renderer, ...
	CSystem::Initialize(SCFTEXT("Settings/Main.xml"), hInstance);
	//We register a custom clean up hander, this way we can add additional clean up steps as needed
	CSystem::HandlerCleanUp(HandlerCleanUp);
	//GUI resides in separate DLL & need to be initialized separately
	GUI::Initialize(GetModuleHandle((STRING("SEGUI") + CSystem::DllSuffix() + STRING(".dll")).Value()));

	//Construct resources - XML-deserialization & actual data loading
	for (SCF::UINT i = 0; i < CSettings::CResources::DefinitionFiles().Size(); i++)
	{
		CManager::ObjectsLoad((CString&)CSettings::CResources::DefinitionFiles()[i]);
	}
	
	//DEBUG: saves(XML-serializes) all the loaded objects, just for reference & checking 
	CManager::ObjectsSave(STRING("Objects.Saved\\"));

	//Initialize startup objects, here we configure the engine, telling it which scene we want to use & what GUI module we want to use for user interaction
	CSimulation::Scene    ( (CScene*)CScripting::Object(CSettings::StartupScene()));
	CDispatcher::GUIModule((CModule*)CScripting::Object(CSettings::StartupGUIModule()));

	//Display the window on screen, centered
	System::CWindow::Center();
	System::CWindow::Show();

	//Register handlers for performing game logic
	CRTLoop::HandlerUpdateStart(&HandlerUpdateStart);
	CRTLoop::HandlerUpdateEnd  (&HandlerUpdateEnd);

	//Start up the real-time loop & the associated simulation
	CRTLoop::Start();
	CSimulation::Start();
	
	//Here we actually enter the message loop, this is when the engine really starts to run
	System::CWindow::MessageLoopEnter();
	
	//After we exit the message loop, we properly shut down the engine
	CSystem::Shutdown();
	return FALSE;
}