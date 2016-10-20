#pragma once

#include "../../../SECore/Source/SECore.h"
#include "../../../SECore/Source/System/SEObject.h"

#include "../Classes.h"

using namespace SCFMathematics;
using namespace SCFBase;

namespace GUI
{
	class SEGUI_API CElement : public System::CSEObject
	{
	public:
		System::Category Category() _GET { return System::Category::Controls; }

	public:
		virtual ~CElement();

	public:
		virtual bool IsContainer() _GET { return FALSE; }

	public:
		virtual void Render() = 0;
		virtual void LayOut() = 0;

	public:
		inline void          Rect(_IN Float4& rRect) _SET { CopyVector4(m_Rect, rRect); this->LayOut(); }
		inline const Float4& Rect()                  _GET { return m_Rect; }

		inline void          Position(_IN float fX, _IN float fY) _SET { m_Rect[0] = fX;           m_Rect[1] = fY; }
		inline void          Position(_IN Float2& rPosition)      _SET { m_Rect[0] = rPosition[0]; m_Rect[1] = rPosition[1]; }
		inline const Float2& Position()                           _GET { return (Float2&)m_Rect; }

		inline void          Size(_IN float fX, _IN float fY) _SET { m_Rect[2] = fX;       m_Rect[3] = fY;       this->LayOut(); }
		inline void          Size(_IN Float2& rSize)          _SET { m_Rect[2] = rSize[0]; m_Rect[3] = rSize[1]; this->LayOut(); }
		inline const Float2& Size()                           _GET { return *(Float2*)(&m_Rect[2]); }
	
	public:
		inline void          SizeMin(_IN float fX, _IN float fY) _SET { m_SizeMin[0] = fX;       m_SizeMin[1] = fY; }
		inline void          SizeMin(_IN Float2& rSize)          _SET { CopyVector2(m_SizeMin, rSize); }
		inline const Float2& SizeMin()                           _GET { return m_SizeMin; }
		
		inline void          SizeMax(_IN float fX, _IN float fY) _SET { m_SizeMax[0] = fX;       m_SizeMax[1] = fY; }
		inline void          SizeMax(_IN Float2& rSize)          _SET { CopyVector2(m_SizeMax, rSize); }
		inline const Float2& SizeMax()                           _GET { return m_SizeMax; }

	public:
		const Float2& SizeIdeal() _GET { return m_SizeIdeal; }
		  
	public:
		static void PositionPush();
		static void PositionPop();

		static inline const Float2& PositionTop() { return s_Positions[s_uiPositionCount]; }

		static inline void Translate(_IN float fX, _IN float fY) { glTranslatef(fX,         fY,         0); s_Positions[s_uiPositionCount][0] += fX;         s_Positions[s_uiPositionCount][1] += fY; }
		static inline void Translate(_IN Float2& rOffset)        { glTranslatef(rOffset[0], rOffset[1], 0); s_Positions[s_uiPositionCount][0] += rOffset[0]; s_Positions[s_uiPositionCount][1] += rOffset[1]; }

	public:
		static void ScissorPush(_IN Float4& rRect);
		static void ScissorPop();

		static inline const Int4& ScissorTop() { return s_ScissorRects[s_uiScissorRectCount]; }

	protected:
		void EnforceMinSize();

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	protected:
		Float4 m_Rect;

	protected:
		Float2 m_SizeMin;
		Float2 m_SizeMax;
		Float2 m_SizeIdeal;

	private:
		static Float2 s_Positions[64];
		static UINT   s_uiPositionCount;

	private:
		static Int4 s_ScissorRects[64];
		static UINT s_uiScissorRectCount;

	protected:
		CElement();
	};
};
