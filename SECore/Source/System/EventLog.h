#pragma once

#ifdef SECORE_EXPORTS
#define SECORE_API __declspec(dllexport)
#else
#define SECORE_API __declspec(dllimport)
#endif

#include <SCFObjectExtensions.h>
#include <SCFMathematics.h>

#include <SCFWinAPI.h>
#include <OpenGL\gl.h>

#undef GL_VERSION_1_1
#undef GL_VERSION_1_2
#undef GL_VERSION_1_3
#undef GL_VERSION_1_4
#undef GL_VERSION_1_5
#undef GL_VERSION_2_0

#include <OpenGL\glext.h>
#include <OpenGL\glu.h>

#include "../Errors.h"

using namespace SCFMathematics;
using namespace SCFBase;

namespace System
{
	class SECORE_API CEventLog
	{
	public:
		static void Enabled(_IN bool bEnabled);
		static bool Enabled();

		static void    Filename(_IN CString& rFilename);
		static CString Filename();

		//Blocks are used to structure text in the log file
		static void BlockNew(_IN CString& rString);
		static void BlockClose();

		//Warnings are simple notification strings
		static void WarningNew   (_IN CString& rString);

		//Error are notification strings that should be used prior to application termination to report debug information
		static void ErrorNew     (_IN CString& rString);

		//Events are notification strings automatically appended with a confirmation string
		static void EventNew     (_IN CString& rString);
		static void EventContinue(_IN CString& rString);

		static void Flush();
		static void Finish();

	private:
		CEventLog();
		~CEventLog();
	};
};