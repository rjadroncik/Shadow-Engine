#pragma once

#include "../Scripting/ProxyElement.h"

#include "../GUI/EditField.h"

namespace Scripting
{
	class SEGUI_API CProxyEditField : public CProxyElement
	{
	public:
		static void Register();
		static CXMLObjectSerializable* New() { return new CProxyEditField(); }
	
	public:
		SCF::ENUM ClassKey() _GET { return ClassProxyEditField; }

	public:
		CProxyEditField();
		virtual ~CProxyEditField();

	public:
		GUI::CEditField& Value() { return *(GUI::CEditField*)m_pObject; }

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);
	};
};
