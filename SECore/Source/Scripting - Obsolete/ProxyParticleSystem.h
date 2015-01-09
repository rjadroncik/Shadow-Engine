#pragma once

#include "ProxyObjectScene.h"

#include "../Simulation/ParticleSystem.h"

namespace Scripting
{
	class SECORE_API CProxyParticleSystem : public CProxyObjectScene
	{
	public:
		static void Register();
		static CXMLObjectSerializable* New() { return new CProxyParticleSystem(); }

	public:
		SCF::ENUM ClassKey() _GET { return ClassProxyParticleSystem; }
		CString   AsString() _GET { return STRING("{ProxyParticleSystem}"); }
 
	public:
		CProxyParticleSystem();
		virtual ~CProxyParticleSystem();

	public:
		Simulation::CParticleSystem& Value() { return *(Simulation::CParticleSystem*)m_pObject; }

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);
	};
};
