#pragma once

#ifdef SEEDITOR_EXPORTS
#define SEEDITOR_API __declspec(dllexport)
#else
#define SEEDITOR_API __declspec(dllimport)
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

#include "Errors.h"

#include "../../SECore/Source/System/EventLog.h"
#include "../../SECore/Source/System/OpenGL20.h"

using namespace SCFMathematics;
using namespace SCFBase;

class SEEDITOR_API CStub
{
public:
	CStub()  { int x = 0; SCF_UNREFERENCED_PARAMETER(x); } 
	~CStub() { int x = 0; SCF_UNREFERENCED_PARAMETER(x); }
};