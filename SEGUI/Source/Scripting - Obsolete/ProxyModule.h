#pragma once

#include "../Scripting/ProxyElement.h"

#include "../GUI/Module.h"

namespace Scripting
{
	class SEGUI_API CProxyModule : public CProxyElement
	{
	public:
		static void Register();
		static CXMLObjectSerializable* New() { return new CProxyModule(); }
	
	public:
		SCF::ENUM ClassKey() _GET { return ClassProxyModule; }

	public:
		CProxyModule();
		virtual ~CProxyModule();

	public:
		GUI::CModule& Value() { return *(GUI::CModule*)m_pObject; }

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);
	};
};
