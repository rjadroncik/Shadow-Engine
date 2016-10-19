#pragma once

#include "Layout.h"

namespace GUI
{
	enum LayoutSplitTypes
	{
		LS_VERTICAL   = 0x01,
		LS_HORIZONTAL = 0x02,

		LS_FIXED_1ST  = 0x10,
		LS_FIXED_2ND  = 0x20,
	};

	class SEGUI_API CLayoutSplit : public CLayout
	{
	public:
		CString XmlName() _GET { return STRING("LayoutSplit"); }

	public:
		CLayoutSplit();
		virtual ~CLayoutSplit();

	public:
		void SplitType(_IN ENUM eType) _SET { m_Split.eType = eType; }
		ENUM SplitType()               _GET { return m_Split.eType; }   

		void SplitValue(_IN UINT uiValue) _SET { m_Split.uiValue = uiValue; }
		UINT SplitValue()                 _GET { return m_Split.uiValue; }   

		void SplitPadding(_IN UINT uiPadding) { m_Split.uiPadding = uiPadding; }
		UINT SplitPadding()                   { return m_Split.uiPadding; }                 

	public:
		void LayOut(_IN CVector<CElement>& rElements, _IN Float4& rRect, _OUT Float2& rOutSize);

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	protected:
		struct SSplit
		{
			ENUM eType;

			//If [eType == LS_VERTICAL/*LS_HORIZONTAL*/] [uiValue] is the 
			//relative width ratio of the first element in percent

			//If [eType == LS_VERTICAL/*LS_HORIZONTAL*/ | LS_FIXED_1ST/*LS_FIXED_2ND*/]
			//[uiValue] is the fixed width one of the elements in pixels
			UINT uiValue;
			UINT uiPadding;

		} m_Split;
	};
};