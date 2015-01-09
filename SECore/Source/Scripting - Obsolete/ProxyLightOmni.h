#pragma once

#include "ProxyLight.h"

#include "../Rendering/LightOmni.h"

namespace Scripting
{
	class SECORE_API CProxyLightOmni : public CProxyLight
	{
	public:
		static void Register();
		static CXMLObjectSerializable* New() { return new CProxyLightOmni(); }

	public:
		SCF::ENUM ClassKey() _GET { return ClassProxyLightOmni; }
		CString   AsString() _GET { return STRING("{ProxyLightOmni}"); }

	public:
		CProxyLightOmni();
		virtual ~CProxyLightOmni();

	public:
		Rendering::CLightOmni& Value() { return *(Rendering::CLightOmni*)m_pObject; }

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);
	};
};
