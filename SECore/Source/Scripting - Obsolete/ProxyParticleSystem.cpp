#include "ProxyParticleSystem.h"
#include "Scripting.h"

using namespace Scripting;

using namespace SCFXML;
using namespace SCFScripting;
//using namespace SCFScriptingXML;

//CProxyParticleSystem::SProperties CProxyParticleSystem::s_Properties;

CProxyParticleSystem::CProxyParticleSystem()
{
}

CProxyParticleSystem::~CProxyParticleSystem()
{
}

void CProxyParticleSystem::Register()
{
	CXMLObjectSerializable::ConstructorRegister(ClassProxyParticleSystem, CProxyParticleSystem::New);

	CClass* pClass = new CClass(CRegistry::Class(ClassProxyObjectScene));
	{
/*		CProperty* pProperty;

		pProperty = new CProperty(ClassFloat);
		pClass->PropertyAdd(STRING("fov"), *pProperty);

		pProperty = new CProperty(ClassFloat);
		pClass->PropertyAdd(STRING("near"), *pProperty);
	
		pProperty = new CProperty(ClassFloat);
		pClass->PropertyAdd(STRING("far"), *pProperty);*/
	}
	CRegistry::ClassRegister(STRING("ParticleSystem"), ClassProxyParticleSystem, *pClass);
}

/*void CProxyParticleSystem::PropertySet(_IN SCF::ENUM eName, _IN CObject& rValue) _SET
{
	if (eName == s_Properties.uiFov)
	{
		((Rendering::CCamera*)m_pObject)->FOV(((CFloat&)rValue).Value());
		return;
	}

	if (eName == s_Properties.uiNear)
	{
		((Rendering::CCamera*)m_pObject)->Near(((CFloat&)rValue).Value());
		return;
	}

	if (eName == s_Properties.uiFar)
	{
		((Rendering::CCamera*)m_pObject)->Far(((CFloat&)rValue).Value());
		return;
	}

	CProxyObjectScene::PropertySet(eName, rValue);
}

CObject* CProxyParticleSystem::PropertyGet(_IN SCF::ENUM eName) _GET
{
	return CProxyObjectScene::PropertyGet(eName);
}*/

void CProxyParticleSystem::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
}

void CProxyParticleSystem::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CString* pPosition = rReader.GetValue();
	CString* pRotation = rReader.GetValue();
	CString* pMaterial = rReader.GetValue();

	CString* pValue = NULL;

	if (pMaterial)
	{
		m_pObject = new Simulation::CParticleSystem(((CProxyMaterial*)CScripting::Object(*pMaterial))->Value());
		ADDREF(*(m_pObject));

		if (pPosition) { m_pObject->Position(CFloat4(*pPosition).Value()); }
		if (pRotation) { m_pObject->Rotation(CFloat3(*pRotation).Value()); }

		//Emitter
		rReader.GetBlock();
		GETVALUE { ((Simulation::CParticleSystem*)m_pObject)->EmitterBox(CFloat3(*pValue).Value()); }

		//Particles
		rReader.GetBlock();
		{
			GETVALUE { ((Simulation::CParticleSystem*)m_pObject)->ParticleCount((SCF::USHORT)CInt(*pValue).Value()); }
			GETVALUE { ((Simulation::CParticleSystem*)m_pObject)->ParticleLife((SCF::USHORT)CInt(*pValue).Value()); }
			GETVALUE { ((Simulation::CParticleSystem*)m_pObject)->ParticleSpread(CFloat(*pValue).Value()); }
		
			//Speed
			rReader.GetBlock();
			{
				GETVALUE { ((Simulation::CParticleSystem*)m_pObject)->ParticleSpeed(CFloat(*pValue).Value()); }
				GETVALUE { ((Simulation::CParticleSystem*)m_pObject)->ParticleSpeedVariation(CFloat(*pValue).Value()); }
				GETVALUE { ((Simulation::CParticleSystem*)m_pObject)->ParticleSpeedChange(CFloat(*pValue).Value()); }
			}

			//Size
			rReader.GetBlock();
			{
				GETVALUE { ((Simulation::CParticleSystem*)m_pObject)->ParticleSize(CFloat(*pValue).Value()); }
				GETVALUE { ((Simulation::CParticleSystem*)m_pObject)->ParticleSizeVariation(CFloat(*pValue).Value()); }
				GETVALUE { ((Simulation::CParticleSystem*)m_pObject)->ParticleSizeChange(CFloat(*pValue).Value()); }
			}

			//Rotation
			rReader.GetBlock();
			{
				GETVALUE { ((Simulation::CParticleSystem*)m_pObject)->ParticleRotation(CFloat(*pValue).Value()); }
				GETVALUE { ((Simulation::CParticleSystem*)m_pObject)->ParticleRotationVariation(CFloat(*pValue).Value()); }
				GETVALUE { ((Simulation::CParticleSystem*)m_pObject)->ParticleRotationChange(CFloat(*pValue).Value()); }
			}

			//Opacity
			rReader.GetBlock();
			{
				GETVALUE { ((Simulation::CParticleSystem*)m_pObject)->ParticleOpacity(CFloat(*pValue).Value()); }
				GETVALUE { ((Simulation::CParticleSystem*)m_pObject)->ParticleOpacityVariation(CFloat(*pValue).Value()); }
				GETVALUE { ((Simulation::CParticleSystem*)m_pObject)->ParticleOpacityChange(CFloat(*pValue).Value()); }
			}
		}

		((Simulation::CParticleSystem*)m_pObject)->Create();
		((Simulation::CParticleSystem*)m_pObject)->BoundsUpdate();
	}
}