#include "Scene.h"
#include "RendererDGC.h"

#include "../System/Scripting.h"

#include <SCFXML.h>

using namespace SCFXML;

using namespace Rendering;
using namespace Simulation;
using namespace System;

CScene::CScene() 
{
	m_uiShadowsFramesTillUpdate = 0;

	m_uiCameraCount         = 0;
	m_ppCameras             = NULL;
	m_pCameraCurrent        = NULL;

	m_uiLightCount          = 0;
	m_ppLights              = NULL;

	m_uiModelCount          = 0;
	m_ppModels	            = NULL;

	m_uiParticleSystemCount = 0;
	m_ppParticleSystems	    = NULL;
}

CScene::~CScene()
{
	CMemory::Free(m_ppCameras);
	CMemory::Free(m_ppLights);
	CMemory::Free(m_ppModels);
	CMemory::Free(m_ppParticleSystems);
}

void __fastcall CScene::Update()
{
	if (m_uiShadowsFramesTillUpdate > 0) { m_uiShadowsFramesTillUpdate--; }
 
	UpdateParticleSystems();

	static Float3 s_Origin = { 0, 0, 0 };
	static Float3 s_Angles = { 1, 0, 0 };

	if (this->LightCount() > 0)
	{
		SetVector3(s_Origin, 500, 0, 100);
		SetVector3(s_Angles, 0, 2.5f, 0);
		this->Light(0).OrbitAroundPoint(s_Origin, s_Angles);
		this->Light(0).Changed(TRUE);
	}

	if (this->LightCount() > 1)
	{
		SetVector3(s_Origin, 0, 0, 0);
		SetVector3(s_Angles, 0, 0.7f, 0);
		this->Light(1).OrbitAroundPoint(s_Origin, s_Angles);
		this->Light(1).Changed(TRUE);
	}

	if (this->LightCount() > 2)
	{
		SetVector3(s_Origin, -100, 0, -100);
		SetVector3(s_Angles, 0, 0.6f, 0);
		this->Light(2).OrbitAroundPoint(s_Origin, s_Angles);
		this->Light(2).Changed(TRUE);
	}
}

void __fastcall CScene::AllChangeStateReset()
{
	for (SCF::UINT i = 0; i < m_uiModelCount;          i++) { m_ppModels[i]->Changed(FALSE); }
	for (SCF::UINT i = 0; i < m_uiLightCount;          i++) { m_ppLights[i]->Changed(FALSE); }
	for (SCF::UINT i = 0; i < m_uiCameraCount;         i++) { m_ppCameras[i]->Changed(FALSE); }
	for (SCF::UINT i = 0; i < m_uiParticleSystemCount; i++) { m_ppParticleSystems[i]->Changed(FALSE); }
}

void __fastcall CScene::AllChangeStateInvalidate()
{
	for (SCF::UINT i = 0; i < m_uiModelCount;          i++) { m_ppModels[i]->Changed(TRUE); }
	for (SCF::UINT i = 0; i < m_uiLightCount;          i++) { m_ppLights[i]->Changed(TRUE); }
	for (SCF::UINT i = 0; i < m_uiCameraCount;         i++) { m_ppCameras[i]->Changed(TRUE); }
	for (SCF::UINT i = 0; i < m_uiParticleSystemCount; i++) { m_ppParticleSystems[i]->Changed(TRUE); }
}

bool __fastcall CScene::CameraSelect(_IN SCF::UINT i)
{
	if (i >= m_uiCameraCount) { return FALSE; }

	m_pCameraCurrent = m_ppCameras[i];
	return TRUE;
}

void __fastcall CScene::CameraSelectNext()
{
	for (SCF::UINT i = 0; i < m_uiCameraCount; i++)
	{
		if (m_ppCameras[i] == m_pCameraCurrent)
		{
			if (i == (m_uiCameraCount - 1))
			{
				m_pCameraCurrent = m_ppCameras[0];
				return;
			}
			else
			{
				m_pCameraCurrent = m_ppCameras[i + 1];
				return;
			}
		}
	}
}

void __fastcall CScene::AllRemoveCameras()
{
	m_uiCameraCount  = 0;
	m_pCameraCurrent = NULL;
}

void __fastcall CScene::AllRemoveLights()
{
	m_uiLightCount = 0;
}

void __fastcall CScene::AllRemoveModels()
{
	m_uiModelCount = 0;
}

void __fastcall CScene::AllRemoveParticleSystems()
{
	m_uiParticleSystemCount = 0;
}

void __fastcall CScene::Add(_IN _REF CCamera& rCamera) 
{ 
	m_uiCameraCount++;
	m_ppCameras = (CCamera**)CMemory::Reallocate(m_ppCameras, m_uiCameraCount * sizeof(CCamera*));
	m_ppCameras[m_uiCameraCount - 1] = (CCamera*)&rCamera;
}

void __fastcall CScene::Add(_IN _REF CLight& rLight)  
{ 
	m_uiLightCount++;
	m_ppLights = (CLight**)CMemory::Reallocate(m_ppLights, m_uiLightCount * sizeof(CLight*));
	m_ppLights[m_uiLightCount - 1] = (CLight*)&rLight; 
}

void __fastcall CScene::Add(_IN _REF CModel& rModel)
{ 
	m_uiModelCount++;
	m_ppModels = (CModel**)CMemory::Reallocate(m_ppModels, m_uiModelCount * sizeof(CModel*));
	m_ppModels[m_uiModelCount - 1] = (CModel*)&rModel;
}

void __fastcall CScene::Add(_IN _REF CParticleSystem& rParticleSystem)  
{ 
	m_uiParticleSystemCount++;
	m_ppParticleSystems = (CParticleSystem**)CMemory::Reallocate(m_ppParticleSystems, m_uiParticleSystemCount * sizeof(CParticleSystem*));
	m_ppParticleSystems[m_uiParticleSystemCount - 1] = (CParticleSystem*)&rParticleSystem; 
}

void __fastcall CScene::UpdateParticleSystems()
{
	if (!this->CameraCurrent()) { return; }

	for (SCF::UINT i = 0; i < m_uiParticleSystemCount; i++)
	{
		if (m_pCameraCurrent->CheckVisibilityByBounds(*m_ppParticleSystems[i]))
		{
			m_ppParticleSystems[i]->Update();
		}
	}
}

CObjectScene* __fastcall CScene::CameraCurrentLookingAt() _GET
{
	float fDistance = 0;

	CObjectScene* pFoundObject = NULL;

	for (SCF::UINT i = 0; i < m_uiModelCount; i++)
	{
		if (m_pCameraCurrent->IsLookingAt(*m_ppModels[i]))
		{
			if (pFoundObject)
			{
				float fNewDistance = MeasureDistance3(*(Float3*)m_pCameraCurrent->Position(), *(Float3*)m_ppModels[i]->Position());
				if (fNewDistance < fDistance)
				{
					pFoundObject = m_ppModels[i];
					fDistance = fNewDistance;
				}
			}
			else
			{
				pFoundObject = m_ppModels[i];
				fDistance = MeasureDistance3(*(Float3*)m_pCameraCurrent->Position(), *(Float3*)m_ppModels[i]->Position());
			}
		}
	}

	for (SCF::UINT i = 0; i < m_uiParticleSystemCount; i++)
	{
		if (m_pCameraCurrent->IsLookingAt(*m_ppParticleSystems[i]))
		{
			if (pFoundObject)
			{
				float fNewDistance = MeasureDistance3(*(Float3*)m_pCameraCurrent->Position(), *(Float3*)m_ppParticleSystems[i]->Position());
				if (fNewDistance < fDistance)
				{
					pFoundObject = m_ppParticleSystems[i];
					fDistance = fNewDistance;
				}
			}
			else
			{
				pFoundObject = m_ppParticleSystems[i];
				fDistance = MeasureDistance3(*(Float3*)m_pCameraCurrent->Position(), *(Float3*)m_ppParticleSystems[i]->Position());
			}
		}
	}
	return pFoundObject;
}

void CScene::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
	if (m_pCameraCurrent) { PUTVALUENEW("activeCamera", m_pCameraCurrent->ID(), CString); }
	else                  { PUTVALUENEW("activeCamera", STRING(""), CString); }

	rWriter.BlockStart(STRING("cameras"));
	PUTVALUENEW("count", m_uiCameraCount, CInt);

	for (SCF::UINT i = 0; i < m_uiCameraCount; i++)
	{
		rWriter.BlockStart(STRING("camera"));
		PUTVALUENEW("id", m_ppCameras[i]->ID(), CString);
		rWriter.BlockEnd();
	}
	rWriter.BlockEnd();

	rWriter.BlockStart(STRING("lights"));
	PUTVALUENEW("count", m_uiLightCount, CInt);

	for (SCF::UINT i = 0; i < m_uiLightCount; i++)
	{
		rWriter.BlockStart(STRING("light"));
		PUTVALUENEW("id", m_ppLights[i]->ID(), CString);
		rWriter.BlockEnd();
	}
	rWriter.BlockEnd();

	rWriter.BlockStart(STRING("models"));
	PUTVALUENEW("count", m_uiModelCount, CInt);

	for (SCF::UINT i = 0; i < m_uiModelCount; i++)
	{
		rWriter.BlockStart(STRING("model"));
		PUTVALUENEW("id", m_ppModels[i]->ID(), CString);
		rWriter.BlockEnd();
	}
	rWriter.BlockEnd();

	rWriter.BlockStart(STRING("particleSystems"));
	PUTVALUENEW("count", m_uiParticleSystemCount, CInt);

	for (SCF::UINT i = 0; i < m_uiParticleSystemCount; i++)
	{
		rWriter.BlockStart(STRING("particleSystem"));
		PUTVALUENEW("id", m_ppParticleSystems[i]->ID(), CString);
		rWriter.BlockEnd();
	}
	rWriter.BlockEnd();
}

void CScene::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CString* pValue = NULL;	
	CString* pActiveCamera = rReader.GetValue();

	//Models
	rReader.GetBlock();
	{
		SCF::UINT uiCount = 0;
		GETVALUE { uiCount = CInt(*pValue).Value(); }

		for (SCF::UINT i = 0; i < uiCount; i++)
		{
			rReader.GetBlock();
			GETVALUE { Add(*(CModel*)CScripting::Object(*pValue)); }
		}
	}

	//Lights
	rReader.GetBlock();
	{
		SCF::UINT uiCount = 0;
		GETVALUE { uiCount = CInt(*pValue).Value(); }

		for (SCF::UINT i = 0; i < uiCount; i++)
		{
			rReader.GetBlock();
			GETVALUE { Add(*(CLight*)CScripting::Object(*pValue)); }
		}
	}

	//Cameras
	rReader.GetBlock();
	{
		SCF::UINT uiCount = 0;
		GETVALUE { uiCount = CInt(*pValue).Value(); }

		for (SCF::UINT i = 0; i < uiCount; i++)
		{
			rReader.GetBlock();
			GETVALUE { Add(*(CCamera*)CScripting::Object(*pValue)); }
		}

		if (pActiveCamera)
		{
			CameraCurrent((CCamera*)CScripting::Object(*pActiveCamera));
		}
	}

	//ParticleSystems
	rReader.GetBlock();
	{
		SCF::UINT uiCount = 0;
		GETVALUE { uiCount = CInt(*pValue).Value(); }

		for (SCF::UINT i = 0; i < uiCount; i++)
		{
			rReader.GetBlock();
			GETVALUE { Add(*(CParticleSystem*)CScripting::Object(*pValue)); }
		}
	}
}