#pragma once

#include "WindowAbstract.h"

namespace GUI
{
	class SEGUI_API CLabel : public CWindowAbstract
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassLabel; }

	public:
		CLabel();
		virtual ~CLabel();

	public:
		 inline void       TextAlignment(_IN SCF::DWORD dwAlignment) _SET { m_dwTextAlignment = dwAlignment; }
		 inline SCF::DWORD TextAlignment()                           _GET { return m_dwTextAlignment; }

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
		SCF::DWORD m_dwTextAlignment;
	};
};