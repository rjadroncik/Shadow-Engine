#include "../../SEGame/Source/SEGame.h"

BOOL APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SCF_UNREFERENCED_PARAMETER(hPrevInstance);
	SCF_UNREFERENCED_PARAMETER(lpCmdLine);
	SCF_UNREFERENCED_PARAMETER(nCmdShow);

	CString ExePath;
	ExePath.CharsReserve(1024);

	GetModuleFileName(GetModuleHandle(NULL), ExePath.Value(), ExePath.CharsReserved());
	CFile ExeFile(ExePath);

#ifdef _DEBUG
	//In debug mode, we load only the debug version of the DLL, others are incompatible
	HMODULE hGameDLL = LoadLibrary((ExeFile.Path() + STRING("SEGameD.dll")).Value());
#else
	//In release mode, we choose the best fitting DLL to load, based on CPU capabilities(instruction sets)
	DWORD dwInstructionSets = CInfoCPU::InstructionSets();
	HMODULE hGameDLL = NULL;

	//Try loading the SSE3, SSE & default versions in that order(they don't have to be all present in the distribution)
	if (dwInstructionSets & SCFBase::CPUInstructionSetSSE)
	{
		if ((dwInstructionSets & SCFBase::CPUInstructionSetSSE2) &&
			(dwInstructionSets & SCFBase::CPUInstructionSetSSE3))
		{
			hGameDLL = LoadLibrary((ExeFile.Path() + STRING("SEGameSSE3.dll")).Value());
		}
		else { hGameDLL = LoadLibrary((ExeFile.Path() + STRING("SEGameSSE.dll")).Value()); }
	}
	else { hGameDLL = LoadLibrary((ExeFile.Path() + STRING("SEGame.dll")).Value()); }

	//Be persistent about loading at least the default DLL, if even that fails, shut down
	if (!hGameDLL) { hGameDLL = LoadLibrary((ExeFile.Path() + STRING("SEGame.dll")).Value()); }
	if (!hGameDLL) { MessageBox(NULL, TEXT("The required 'SEGame*.dll' was not found in the main game directory.\nGame cannot be launched without this file, shutting down."), TEXT("Required module not found!"), MB_ICONERROR); return -1; }
#endif

	//Once we have a DLL loaded, we use its exported function(the only one for now) to start up the game.
	//TODO: Each game based on this engine needs its own implementation of [SEGame.dll], this way many games & also mods can be built upon the same engine & infrastructure
	PFNSEGAMERUN SEGameRun = (PFNSEGAMERUN)GetProcAddress(hGameDLL, "SEGameRun");
	return SEGameRun(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}