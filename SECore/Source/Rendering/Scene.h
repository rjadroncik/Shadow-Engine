#pragma once
				
#include "Model.h"
#include "../Simulation/ParticleSystem.h"

namespace Resources { class SECORE_API CWorld; };
namespace Rendering
{
	class SECORE_API CModel;
	class SECORE_API CScene : public System::CSEObject
	{
	public:
		System::Category Category() _GET { return System::Category::Scenes; }

	public:
		CString XmlName() _GET { return STRING("Scene"); }
		CString ToString() _GET { return STRING("{Scene}"); }

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
		bool __fastcall CameraSelect(_IN UINT i) _SET;
		void __fastcall CameraSelectNext()       _SET;

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

		inline CCamera&                     Camera        (_IN UINT uiIndex) _GET { return *m_ppCameras        [uiIndex]; }
		inline CLight&                      Light         (_IN UINT uiIndex) _GET { return *m_ppLights         [uiIndex]; }
		inline CModel&                      Model         (_IN UINT uiIndex) _GET { return *m_ppModels         [uiIndex]; }
		inline Simulation::CParticleSystem& ParticleSystem(_IN UINT uiIndex) _GET { return *m_ppParticleSystems[uiIndex]; }

		inline UINT CameraCount()         _GET { return m_uiCameraCount; }
		inline UINT LightCount()          _GET { return m_uiLightCount; }
		inline UINT ModelCount()          _GET { return m_uiModelCount; }
		inline UINT ParticleSystemCount() _GET { return m_uiParticleSystemCount; }

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	private:
		//Arrays of all objects that build up a scene
		UINT m_uiCameraCount;
		CCamera** m_ppCameras;
 
		UINT m_uiLightCount;
		CLight**  m_ppLights;

		UINT m_uiModelCount;
		CModel**  m_ppModels;

		UINT                          m_uiParticleSystemCount;
		Simulation::CParticleSystem** m_ppParticleSystems;

	private:
		CCamera* m_pCameraCurrent;

	private:
		UINT m_uiShadowsFramesTillUpdate;
	 };
}
