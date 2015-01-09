#pragma once

#ifdef CORE_EXPORTS
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif

#include <SCFObjectExtensions.h>
#include <SCFMathematics.h>

#include <OpenGL\gl.h>

#undef GL_VERSION_1_1
#undef GL_VERSION_1_2
#undef GL_VERSION_1_3
#undef GL_VERSION_1_4
#undef GL_VERSION_1_5
#undef GL_VERSION_2_0

#include <OpenGL\glext.h>
#include <OpenGL\glu.h>

#include "Classes.h"

using namespace SCFMathematics;
using namespace SCFObjectExtensions;

#include <SCFStandardUndef.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace GUI
{
	class CFontBitmap : public CObject
	{
	public:
		//Dynamic casting
		void* AsType(_IN ENUM eType) const; 

	public:
		UINT    ClassKey()       _GET { return ClassFontBitmap; }
		CString ClassKeyString() _GET { return CString(STRING("FontBitmap")); }

		CFontBitmap();
		virtual ~CFontBitmap();

	public:
		inline int Height()  _GET { return m_iHeight; }
		inline int Ascent()  _GET { return m_iAscent; }
		inline int Descent() _GET { return m_iDescent; }

		inline const CString Name() _GET { return m_csName; }

	public:
		inline void Select() const { if (s_pFontCurrent != this) { SelectInternal(); } }

		//Draws a string positioned at the baseline
		void StringDraw(_IN CString& rString, _IN Vector2i& rPosition) const;
		void StringDraw(_IN CString& rString, _IN int iX, _IN int iY)  const;
		
		inline void StringDraw(_IN CString& rString) const { glCallLists(rString.Length(), GL_UNSIGNED_SHORT, rString.Value()); }

	public:
		void StringMeasure(_IN CString& rString, _OUT Vector2i& rOutSize) _GET;

	public:
		bool Create(_IN CString rFontName, _IN UINT uiHeight, _IN HDC hDC, _IN UINT uiCount);
		void SelectInternal() const;

	private:
		UINT m_uiListBase;
		UINT m_uiCharCount;

		int* m_ipCharWidths;

	private:
		int m_iHeight;
		int m_iAscent;
		int m_iDescent;

		CString m_csName;

	private:
		static CFontBitmap* s_pFontCurrent;
		static Vector4i     s_Viewport;
	};
};