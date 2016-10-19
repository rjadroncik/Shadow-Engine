#include "Uniforms.h"

using namespace Rendering;
using namespace System;
using namespace Resources;
using namespace Simulation;

UNIFORM_BINDER Uniforms_aBindersModel         [US_COUNT];
UNIFORM_BINDER Uniforms_aBindersParticleSystem[US_COUNT];

CUniforms::SLight CUniforms::Light;

namespace Rendering 
{
	extern const CScene*           Renderer_pScene;
	extern const CCamera*          Renderer_pCamera;
	extern const CModel*           Renderer_pModel;
	extern const CMaterialUsage*   Renderer_pMaterialUsage;
	extern const CParticleSystem*  Renderer_pParticleSystem;
	extern const CLight*           Renderer_pLight;

	extern const CModel** Renderer_ppModels;
	extern const CModel** Renderer_ppModelsShadow;
	extern const CLight** Renderer_ppLights;

	extern UINT Renderer_uiModelCount;
	extern UINT Renderer_uiModelShadowCount;
	extern UINT Renderer_uiLightCount;
};

void __fastcall CUniforms::BindLightDepthMapTransform0(_IN UINT uiTarget)
{
	glUniformMatrix4fvARB(uiTarget, 1, FALSE, Light.DepthMapTransform[0]);
}

void __fastcall CUniforms::BindLightPosition(_IN UINT uiTarget)
{
	glUniform3fvARB(uiTarget, 1, Renderer_pLight->Position());
}

void __fastcall CUniforms::BindLightParameters(_IN UINT uiTarget)
{
	static Float4 LightParams;
	LightParams[0] = Renderer_pLight->Range();
	LightParams[1] = LightParams[0];
	LightParams[2] = LightParams[0];
	LightParams[3] = LightParams[0];

	glUniform4fvARB(uiTarget, 1, LightParams);
}

void __fastcall CUniforms::BindLightTint(_IN UINT uiTarget)
{
	glUniform3fvARB(uiTarget, 1, Renderer_pLight->Tint());
}

void __fastcall CUniforms::BindLightAmbDifSpe(_IN UINT uiTarget)
{
	static Float3 AmbDifSpe;
	AmbDifSpe[0] = Renderer_pLight->Ambient();
	AmbDifSpe[1] = Renderer_pLight->Diffuse();
	AmbDifSpe[2] = Renderer_pLight->Specular();

	glUniform3fvARB(uiTarget, 1, AmbDifSpe);
}

void __fastcall CUniforms::BindCameraPosition(_IN UINT uiTarget)
{
	glUniform3fvARB(uiTarget, 1, Renderer_pCamera->Position());
}

void __fastcall CUniforms::BindCameraViewport(_IN UINT uiTarget)
{
	Float4 Viewport;

	Viewport[0] = (float)(Renderer_pCamera->Viewport())[0];
	Viewport[1] = (float)(Renderer_pCamera->Viewport())[1];
	Viewport[2] = (float)(Renderer_pCamera->Viewport())[2];
	Viewport[3] = (float)(Renderer_pCamera->Viewport())[3];

	glUniform4fvARB(uiTarget, 1, Viewport);
}

void __fastcall CUniforms::BindMaterialTint(_IN UINT uiTarget)
{
	glUniform3fvARB(uiTarget, 1, Renderer_pModel->Material().Tint());
}

void __fastcall CUniforms::BindMaterialAmbDifSpeEmi(_IN UINT uiTarget)
{
	static Float4 AmbDifSpeEmi;
	AmbDifSpeEmi[0] = Renderer_pModel->Material().Ambient();
	AmbDifSpeEmi[1] = Renderer_pModel->Material().Diffuse();
	AmbDifSpeEmi[2] = Renderer_pModel->Material().Specular();
	AmbDifSpeEmi[3] = Renderer_pModel->Material().Emission();

	glUniform4fvARB(uiTarget, 1, AmbDifSpeEmi);
}

void __fastcall CUniforms::BindMaterialShininess(_IN UINT uiTarget)
{
	glUniform1fARB(uiTarget, Renderer_pModel->Material().Shininess());
}

void __fastcall CUniforms::BindModelLightPositionLocal(_IN UINT uiTarget)
{
	static Float4 s_LightPositionLocal;

	Renderer_pModel->InverseTransform(s_LightPositionLocal, Renderer_pLight->Position()); 
	glUniform3fvARB(uiTarget, 1, s_LightPositionLocal);
}

void __fastcall CUniforms::BindModelCameraPositionLocal(_IN UINT uiTarget)
{
	static Float4 s_CameraPositionLocal;

	Renderer_pModel->InverseTransform(s_CameraPositionLocal, Renderer_pCamera->Position());
	glUniform3fvARB(uiTarget, 1, s_CameraPositionLocal);
}


void __fastcall CUniforms::BindModelOnlyCamera()
{
	const CGPUProgram* pProgram = &Renderer_pMaterialUsage->Program();

	for (UINT i = 0; i < pProgram->UniformsCount(); i++)
	{
		if ((pProgram->Uniform(i)->eSource > CAMERA_PARAMS_START) &&
			(pProgram->Uniform(i)->eSource < CAMERA_PARAMS_END))
		{
			Uniforms_aBindersModel[pProgram->Uniform(i)->eSource](pProgram->Uniform(i)->uiTarget);
		}
	}
}
void __fastcall CUniforms::BindModelOnlyLight()
{
	const CGPUProgram* pProgram = &Renderer_pMaterialUsage->Program();

	for (UINT i = 0; i < pProgram->UniformsCount(); i++)
	{
		if ((pProgram->Uniform(i)->eSource > LIGHT_PARAMS_START) &&
			(pProgram->Uniform(i)->eSource < LIGHT_PARAMS_END))
		{
			Uniforms_aBindersModel[pProgram->Uniform(i)->eSource](pProgram->Uniform(i)->uiTarget);
		}
	}
}
void __fastcall CUniforms::BindModelOnlyMaterial()
{
	const CGPUProgram* pProgram = &Renderer_pMaterialUsage->Program();

	for (UINT i = 0; i < pProgram->UniformsCount(); i++)
	{
		if ((pProgram->Uniform(i)->eSource > MATERIAL_PARAMS_START) &&
			(pProgram->Uniform(i)->eSource < MATERIAL_PARAMS_END))
		{
			Uniforms_aBindersModel[pProgram->Uniform(i)->eSource](pProgram->Uniform(i)->uiTarget);
		}
	}
}

void __fastcall CUniforms::BindModel()
{
	const CGPUProgram* pProgram = &Renderer_pMaterialUsage->Program();

	for (UINT i = 0; i < pProgram->UniformsCount(); i++)
	{
		Uniforms_aBindersModel[pProgram->Uniform(i)->eSource](pProgram->Uniform(i)->uiTarget);
	}
}

void __fastcall CUniforms::BindParticleSystem()
{
	const CGPUProgram* pProgram = &Renderer_pMaterialUsage->Program();

	for (UINT i = 0; i < pProgram->UniformsCount(); i++)
	{
		Uniforms_aBindersParticleSystem[pProgram->Uniform(i)->eSource](pProgram->Uniform(i)->uiTarget);
	}
}

bool __fastcall CUniforms::Initialize()
{
	static bool s_bInitalized = FALSE;
	if (!s_bInitalized)
	{
		CEventLog::BlockNew(STRING("Preparing uniforms binders"));

		Uniforms_aBindersModel[US_LIGHT_POSITION]       = BindLightPosition;
		Uniforms_aBindersModel[US_LIGHT_POSITION_LOCAL] = BindModelLightPositionLocal;
		Uniforms_aBindersModel[US_LIGHT_PARAMETERS]     = BindLightParameters;
		Uniforms_aBindersModel[US_LIGHT_TINT]           = BindLightTint;
		Uniforms_aBindersModel[US_LIGHT_AMB_DIF_SPE]    = BindLightAmbDifSpe;

		Uniforms_aBindersModel[US_LIGHT_MATRIX_DEPTHMAP_TRANSFORM0] = BindLightDepthMapTransform0;
		Uniforms_aBindersModel[US_LIGHT_MATRIX_DEPTHMAP_TRANSFORM0] = BindLightDepthMapTransform0;
		Uniforms_aBindersModel[US_LIGHT_MATRIX_DEPTHMAP_TRANSFORM1] = BindLightDepthMapTransform0;

		Uniforms_aBindersModel[US_CAMERA_POSITION]       = BindCameraPosition;
		Uniforms_aBindersModel[US_CAMERA_POSITION_LOCAL] = BindModelCameraPositionLocal;
		Uniforms_aBindersModel[US_CAMERA_VIEWPORT]       = BindCameraViewport;

		Uniforms_aBindersModel[US_MATERIAL_TINT]            = BindMaterialTint;
		Uniforms_aBindersModel[US_MATERIAL_AMB_DIF_SPE_EMI] = BindMaterialAmbDifSpeEmi;
		Uniforms_aBindersModel[US_MATERIAL_SHININESS]       = BindMaterialShininess;

		Uniforms_aBindersParticleSystem[US_LIGHT_POSITION]       = BindLightPosition;
		Uniforms_aBindersParticleSystem[US_LIGHT_POSITION_LOCAL] = NULL;
		Uniforms_aBindersParticleSystem[US_LIGHT_PARAMETERS]     = BindLightParameters;
		Uniforms_aBindersParticleSystem[US_LIGHT_TINT]           = BindLightTint;
		Uniforms_aBindersParticleSystem[US_LIGHT_AMB_DIF_SPE]    = BindLightAmbDifSpe;

		Uniforms_aBindersParticleSystem[US_CAMERA_POSITION]       = BindCameraPosition;
		Uniforms_aBindersParticleSystem[US_CAMERA_POSITION_LOCAL] = NULL;
		Uniforms_aBindersParticleSystem[US_CAMERA_VIEWPORT]       = BindCameraViewport;

		Uniforms_aBindersParticleSystem[US_MATERIAL_TINT]            = BindMaterialTint;
		Uniforms_aBindersParticleSystem[US_MATERIAL_AMB_DIF_SPE_EMI] = BindMaterialAmbDifSpeEmi;
		Uniforms_aBindersParticleSystem[US_MATERIAL_SHININESS]       = BindMaterialShininess;

		CEventLog::BlockClose();

		return TRUE;
	}

	return FALSE;
}