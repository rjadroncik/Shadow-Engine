#ifdef SEGUI_EXPORTS
#define SEGUI_API __declspec(dllexport)
#else
#define SEGUI_API __declspec(dllimport)
#endif

#include <SCFStandard.h>

namespace GUI
{
	bool SEGUI_API __fastcall Initialize(_IN void* hModule);
};
