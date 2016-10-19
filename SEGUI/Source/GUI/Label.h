#pragma once

#include "WindowAbstract.h"

namespace GUI
{
	class SEGUI_API CLabel : public CWindowAbstract
	{
	public:
		CString XmlName() _GET { return STRING("Label"); }

	public:
		CLabel();
		virtual ~CLabel();

	public:
		 inline void       TextAlignment(_IN DWORD dwAlignment) _SET { m_dwTextAlignment = dwAlignment; }
		 inline DWORD TextAlignment()                           _GET { return m_dwTextAlignment; }

	public:
		inline CText& Text() _GET { return (CText&)m_Text; }

	public:
		void Render();
		void LayOut();

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	protected:
		CText      m_Text;
		DWORD m_dwTextAlignment;
	};
};