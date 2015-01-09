#pragma once

#include "Proxy.h"

#include "../Resources/Texture.h"

namespace Scripting
{
	class SECORE_API CProxyTexture : public CProxy
	{
	public:
		static void Register();
		static CXMLObjectSerializable* New() { return new CProxyTexture(); }

	public:
		SCF::ENUM ClassKey() _GET { return ClassProxyTexture; }
		CString   AsString() _GET { return STRING("{ProxyTexture}"); }

	public:
		CProxyTexture();
		virtual ~CProxyTexture();

	public:
		Resources::CTexture& Value() { return *m_pObject; }

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	protected:
		Resources::CTexture* m_pObject;
	};
};
