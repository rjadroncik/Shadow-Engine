#pragma once

#include "../../../SECore/Source/Scripting/Proxy.h"

#include "../GUI/Element.h"

namespace Scripting
{
	class SEGUI_API CProxyElement : public CProxy
	{
	public:
		static void Register();
		static void PropertiesAdd(_OUT SCFScripting::CClass& rOutClass);

	public:
		CProxyElement();
		virtual ~CProxyElement();

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	protected:
		GUI::CElement* m_pObject;
	};
};
