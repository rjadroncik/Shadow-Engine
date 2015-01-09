#pragma once

#include "ProxyObjectScene.h"

#include "../Rendering/Scene.h"

#include "ProxyModel.h"
#include "ProxyLight.h"
#include "ProxyCamera.h"
#include "ProxyParticleSystem.h"

namespace Scripting
{
	class SECORE_API CProxyScene : public CProxy
	{
	public:
		static void Register();
		static CXMLObjectSerializable* New() { return new CProxyScene(); }

	public:
		SCF::ENUM ClassKey() _GET { return ClassProxyScene; }
		CString   AsString() _GET { return STRING("{ProxyScene}"); }

	public:
		CProxyScene();
		virtual ~CProxyScene();

	public:
		CVector* Cameras()         _GET { return (CVector*)&m_Cameras; }
		CVector* Lights()          _GET { return (CVector*)&m_Lights; }
		CVector* Models()          _GET { return (CVector*)&m_Models; }
		CVector* ParticleSystems() _GET { return (CVector*)&m_ParticleSystems; }

	public:
		void          CameraCurrentSet(_IN CProxyCamera* pCamera) _SET { m_pCameraCurrent = pCamera; }
		CProxyCamera* CameraCurrent()                             _GET { return (CProxyCamera*)m_pCameraCurrent; }

	public:
		Rendering::CScene& Value() { return *(Rendering::CScene*)m_pObject; }

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);
	
	protected:
		Rendering::CScene* m_pObject;

	protected:
		CVector m_Cameras;
		CVector m_Lights;
		CVector m_Models;
		CVector m_ParticleSystems;
		
	protected:
		const CProxyCamera* m_pCameraCurrent;
	};
};
