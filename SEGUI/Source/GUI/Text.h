#pragma once

#include "../Resources/Font.h"

namespace GUI
{
	class SEGUI_API CText : public SCFBase::CObject
	{
	public:
		CString XmlName() _GET { return STRING("Text"); }

	public:
		CText();
		virtual ~CText();

	public:
		inline void          String(_IN CString rString) _SET { m_csString = rString; this->Update(); }
		inline const CString String()                    _GET { return m_csString; }

		inline void                    Font(_IN Resources::CFont& rFont) _SET { m_pFont = &rFont; this->Update(); }
		inline const Resources::CFont* Font()                            _GET { return m_pFont; }

		inline const Float2& Size() _GET { return m_Size; }
		
		inline const Float2& Position()                      _GET { return m_Position; }
		inline void          Position(_IN Float2& rPosition) _SET { m_Position[0] = rPosition[0]; m_Position[1] = rPosition[1]; }

		inline const Float4& Color()                   _GET { return m_Color; }
		inline void          Color(_IN Float4& rColor) _SET { CopyVector4(m_Color, rColor); }

	public:
		void RangeToRect(_IN Int2&   rRange, _OUT Float4& rOutRect);
		bool RectToRange(_IN Float4& rRect,  _OUT Int2&   rOutRange);
	
		UINT PositionToIndex(_IN Float2& rPosition);

	public:
		void Render();

	private:
		void Update();

	private:
		CString m_csString;
		const Resources::CFont* m_pFont;

		Float2 m_Position;
		Float2 m_Size;
		Float4 m_Color;
	};
};