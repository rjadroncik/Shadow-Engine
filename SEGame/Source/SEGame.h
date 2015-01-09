#pragma once

#ifdef SEGAME_EXPORTS
#define SEGAME_API __declspec(dllexport)
#else
#define SEGAME_API __declspec(dllimport)
#endif

#include "../../SECore/Source/Simulation/Simulation.h"
#include "../../SECore/Source/System/RTLoop.h"
#include "../../SECore/Source/System/System.h"
#include "../../SECore/Source/System/Settings.h"
#include "../../SECore/Source/System/Scripting.h"
#include "../../SECore/Source/Resources/Manager.h"

#include "../../SEGUI/Source/SEGUI.h"
#include "../../SEGUI/Source/GUI/Module.h"
#include "../../SEGUI/Source/GUI/Dispatcher.h"

typedef int (__stdcall *PFNSEGAMERUN) (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

int SEGAME_API __stdcall SEGameRun(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
