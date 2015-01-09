#pragma once

#include "ProxyLight.h"

#include "../Rendering/LightSpot.h"

namespace Scripting
{
	class SECORE_API CProxyLightSpot : public CProxyLight
	{
	public:
		static void Register();
		static CXMLObjectSerializable* New() { return new CProxyLightSpot(); }

	public:
		SCF::ENUM ClassKey() _GET { return ClassProxyLightSpot; }
		CString   AsString() _GET { return STRING("{ProxyLightSpot}"); }

	public:
		CProxyLightSpot();
		virtual ~CProxyLightSpot();

	public:
		void    CutoffSet(_IN CFloat& rCutoff) _SET { ((Rendering::CLight*)m_pObject)->Cutoff(rCutoff.Value()); }
		CFloat* Cutoff()                       _GET { return new CFloat(((Rendering::CLight*)m_pObject)->Cutoff()); }

	public:
		Rendering::CLightSpot& Value() { return *(Rendering::CLightSpot*)m_pObject; }

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);
	};
};
