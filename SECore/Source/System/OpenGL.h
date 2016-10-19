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
#undef GL_VERSION_1_2
#include <OpenGL\glext.h>
#include <OpenGL\glu.h>

#include "../Errors.h"

using namespace SCFMathematics;
using namespace SCFBase;

#define LOAD_FUNCTION(functype, funcname) ((funcname = (functype)wglGetProcAddress(#funcname)) == NULL)

namespace System
{
	enum VendorList
	{
		VENDOR_NOT_SUPPORTED,
		VENDOR_ATI,
		VENDOR_NVIDIA
	};

	struct SDisplayMode
	{
		//The [uiRefreshRate] information is relevant only for full-screen modes
		struct SResolution
		{
			UINT uiHorizontal;
			UINT uiVertical;

			UINT uiRefreshRate;
		};

		//The most basic characteristics of an OpenGL display mode (windowed or full-screen)
		UINT uiColorBits;
		UINT uiDepthBits;
		UINT uiStencilBits;
	};

	class SECORE_API COpenGL
	{
	public:
		static void Initialize();

	public:
		//The [COpenGL] class maintains a zero based list of supported display modes that can be queried/applied
		static UINT ModeSupportedCount();
		static void ModeSupported(_IN UINT uiIndex, _OUT SDisplayMode* pOutDisplayMode);
		
		static bool ModeSupports(_IN SDisplayMode* pDisplayMode);

		static bool ModeSelect(_IN void* hWindow, _IN bool bFullscreen, _IN SDisplayMode* pDisplayMode);

	public:
 		static CString Extensions();
		static CString Vendor();
		static CString Renderer();
		static CString Version();

	public:
		static UINT VersionNumber();
		static ENUM VendorID();

	private:
		static void InitializeOpenGLState();

	private:
		COpenGL();
		~COpenGL();
	};
};
