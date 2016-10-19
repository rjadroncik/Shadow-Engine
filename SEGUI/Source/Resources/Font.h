#pragma once

#include "../../../SECore/Source/SECore.h"
#include "../../../SECore/Source/System/SEObject.h"

#include "../Classes.h"

#include <SCFImaging.h>
#include <SCFXML.h>

using namespace SCFMathematics;
using namespace SCFBase;

namespace Resources
{
	class SEGUI_API CFont : public System::CSEObject
	{
	public:
		CString XmlName() _GET { return STRING("Font"); }

	public:
		CFont();
		virtual ~CFont();

	public:
		inline float Height()  _GET { return m_fHeight; }
		inline float Ascent()  _GET { return m_fAscent; }
		inline float Descent() _GET { return m_fDescent; }

	public:
		void __fastcall Select() const;
		
		//Draws a string positioned at the baseline
		void __fastcall StringDraw(_IN CString& rString, _IN Float2& rPosition) const;
		void __fastcall StringDraw(_IN CString& rString, _IN float fX, _IN float fY) const;

	public:
		void __fastcall StringMeasure(_IN CString& rString, _OUT Float2& rOutSize) _GET;

	public:
		bool __fastcall Create(_INOUT SCFXML::CXMLStreamRead& rStream, _IN SCFImaging::CImage& rImage);

	protected:
		bool __fastcall CreateTexture(_IN SCFImaging::CImage& rImage);
		bool __fastcall CreateDisplayList(_INOUT SCFXML::CXMLStreamRead& rStream);

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	private:
		UINT m_uiListBase;
		UINT m_uiTexture;

		UINT m_uiCharacterMax;
		float* m_fpCharWidths;

	private:
		float m_fHeight;
		float m_fAscent;
		float m_fDescent;
	};
};