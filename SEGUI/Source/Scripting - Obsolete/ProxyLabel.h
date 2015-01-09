#pragma once

#include "../Scripting/ProxyElement.h"

#include "../GUI/Label.h"

namespace Scripting
{
	class SEGUI_API CProxyLabel : public CProxyElement
	{
	public:
		static void Register();
		static CXMLObjectSerializable* New() { return new CProxyLabel(); }
	
	public:
		SCF::ENUM ClassKey() _GET { return ClassProxyLabel; }

	public:
		CProxyLabel();
		virtual ~CProxyLabel();

	public:
		void Text(_IN CString& rText) { ((GUI::CLabel*)m_pObject)->Text().String(rText); }

	public:
		GUI::CLabel& Value() { return *(GUI::CLabel*)m_pObject; }

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);
	};
};
