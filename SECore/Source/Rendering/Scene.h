#pragma once
				
#include "Model.h"
#include "Model.h"
#include "../Simulation/ParticleSystem.h"

namespace Resources { class SECORE_API CWorld; };
namespace Rendering
{
	class SECORE_API CModel;
	class SECORE_API CScene : public System::CSEObject
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassScene; }
		CString   ToString() _GET { return STRING("{Scene}"); }

	public:
		CScene();
		virtual ~CScene();

	public:
		inline bool ShadowsNeedUpdate()                 _GET { return (m_uiShadowsFramesTillUpdate == 0); }
		inline void ShadowsNeedUpdate(_IN bool bUpdate) _SET { if (bUpdate) { m_uiShadowsFramesTillUpdate = 0; } else { m_uiShadowsFramesTillUpdate = 2; } }

	public:
		void __fastcall AllChangeStateReset();
		void __fastcall AllChangeStateInvalidate();

		void __fastcall Update();
		void __fastcall UpdateParticleSystems();

	public:
		bool __fastcall CameraSelect(_IN SCF::UINT i) _SET;
		void __fastcall CameraSelectNext()            _SET;

		inline CCamera*	CameraCurrent()                     _GET { return m_pCameraCurrent; }
		inline void     CameraCurrent(_IN CCamera* pCamera) _SET { m_pCameraCurrent = (CCamera*)pCamera; }
		
		CObjectScene* __fastcall CameraCurrentLookingAt() _GET;

	public:
		void __fastcall Add(_IN _REF CCamera&                     rCamera);
		void __fastcall Add(_IN _REF CLight&                      rLight);
		void __fastcall Add(_IN _REF CModel&                      rModel);
		void __fastcall Add(_IN _REF Simulation::CParticleSystem& rParticleSystem);

		void __fastcall AllRemoveCameras();
		void __fastcall AllRemoveLights();
		void __fastcall AllRemoveModels();
		void __fastcall AllRemoveParticleSystems();

		inline CCamera&                     Camera        (_IN SCF::UINT uiIndex) _GET { return *m_ppCameras        [uiIndex]; }
		inline CLight&                      Light         (_IN SCF::UINT uiIndex) _GET { return *m_ppLights         [uiIndex]; }
		inline CModel&                      Model         (_IN SCF::UINT uiIndex) _GET { return *m_ppModels         [uiIndex]; }
		inline Simulation::CParticleSystem& ParticleSystem(_IN SCF::UINT uiIndex) _GET { return *m_ppParticleSystems[uiIndex]; }

		inline SCF::UINT CameraCount()         _GET { return m_uiCameraCount; }
		inline SCF::UINT LightCount()          _GET { return m_uiLightCount; }
		inline SCF::UINT ModelCount()          _GET { return m_uiModelCount; }
		inline SCF::UINT ParticleSystemCount() _GET { return m_uiParticleSystemCount; }

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	private:
		//Arrays of all objects that build up a scene
		SCF::UINT m_uiCameraCount;
		CCamera** m_ppCameras;
 
		SCF::UINT m_uiLightCount;
		CLight**  m_ppLights;

		SCF::UINT m_uiModelCount;
		CModel**  m_ppModels;

		SCF::UINT                     m_uiParticleSystemCount;
		Simulation::CParticleSystem** m_ppParticleSystems;

	private:
		CCamera* m_pCameraCurrent;

	private:
		SCF::UINT m_uiShadowsFramesTillUpdate;
	 };
}
