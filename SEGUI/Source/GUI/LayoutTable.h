#pragma once

#include "Layout.h"

namespace GUI
{
	class SEGUI_API CLayoutTable : public CLayout
	{
	public:
		CString XmlName() _GET { return STRING("LayoutTable"); }

	public:
		CLayoutTable();
		virtual ~CLayoutTable();

	public:
		void        TableSize(_IN Int2& rSize) _SET { m_Table.Size[0] = rSize[0]; m_Table.Size[1] = rSize[1]; }
		const Int2& TableSize()                _GET { return m_Table.Size; }               
	
	public:
		void  CellPadding(_IN float fPadding) _SET { m_Table.fPadding = fPadding; }
		float CellPadding()                   _GET { return m_Table.fPadding; }                 

	public:
		void LayOut(_IN CVector<CElement>& rElements, _IN Float4& rRect, _OUT Float2& rOutSize);

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	protected:
 		struct STable
		{
			Int2 Size;
			float fPadding;

		} m_Table;
	};
};