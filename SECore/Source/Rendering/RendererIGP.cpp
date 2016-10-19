#include "RendererIGP.h"

#include "Uniforms.h"

#include "../Simulation/Simulation.h"
#include "../System/Statistics.h"

using namespace Rendering;
using namespace System;
using namespace Resources;
using namespace Simulation;

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

	extern const CGPUProgram* Renderer_pShadowMapProgram;
	extern UINT          Renderer_uiShadowMapBuffer;
};

bool __fastcall CRendererIGP::Initialize()
{
	static bool s_bInitalized = FALSE;
	if (!s_bInitalized)
	{
		CEventLog::BlockNew(STRING("Initializing renderer for OpenGL 2.0 IGP codepath"));

		CRenderer::Initialize();
		CUniforms::Initialize();

		CRenderer::Render = CRendererIGP::Render;

		CEventLog::BlockClose();

		return TRUE;
	}

	return FALSE;
}

void CRendererIGP::VisibilityResolve()
{
	//Prepare OpenGL
	glColorMask(1, 1, 1, 1);
	glDepthMask(1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColorMask(0, 0, 0, 0);

	//Sort models based on distance, perform analytic visibility checks, etc.
	RenderListUpdate();
}

void CRendererIGP::LightDepthMapUpdate()
{
	//Prepare OpenGL
	glColorMask(0, 0, 0, 0);
	glDepthMask(1);
	glDisable(GL_BLEND);

	Renderer_pLight->ApplyProjectionShadow();
	glGetFloatv(GL_MODELVIEW_MATRIX, CUniforms::Light.DepthMapTransform[0]);

	Renderer_pLight->ApplyProjection3D();
	RenderListShadowUpdate();

	glEnable(GL_POLYGON_OFFSET_FILL);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, Renderer_uiShadowMapBuffer);
	glShadeModel(GL_FLAT);
	{
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, ShadowMapFront(Renderer_pLight->ShadowMapLOD())->GLID(), 0);
		glClear(GL_DEPTH_BUFFER_BIT);

		GPUProgramAttributesDisable();
		CMaterialUsage::TexturesDisable();
		GPUProgramBind(*Renderer_pShadowMapProgram);

		for (UINT i = 0; i < Renderer_uiModelShadowCount; i++)
		{
			glPushMatrix();
			{
				Renderer_ppModelsShadow[i]->TransformationApply();
				RenderMesh(Renderer_ppModelsShadow[i]->Mesh());
			}
			glPopMatrix();
		}
	}
	glShadeModel(GL_SMOOTH);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glDisable(GL_POLYGON_OFFSET_FILL);

	CMaterialUsage::LastReset();
}

void __fastcall CRendererIGP::Render(_IN CScene& rScene)
{
	if (!rScene.CameraCount() || !rScene.CameraCurrent()) { return; }

	//Gather objects
	Renderer_pScene  = &rScene;
	Renderer_pCamera = rScene.CameraCurrent();

	((CCamera*)Renderer_pCamera)->Update();

	LightsUpdateState();
	VisibilityResolve();
	LightsChoose();

	CStatistics::CRendering::Reset();
	for (UINT i = 0 ; i < Renderer_uiLightCount; i++)
	{
		//Shadow preparation
		Renderer_pLight = Renderer_ppLights[i];
		if (Renderer_pLight->Shadows()) 
		{
			LightDepthMapUpdate();
			CStatistics::CRendering::ProcessedShadows(CStatistics::CRendering::ProcessedShadows() + Renderer_uiModelShadowCount);

			ShadowMapBindFront();
			if (Renderer_pLight->ClassKey() == ClassLightOmni) { ShadowMapBindBack(); }
		}

		//Final lighting
		Renderer_pCamera->ApplyProjection3D();

		if (i == 0) { glDisable(GL_BLEND); }
		else        { glEnable (GL_BLEND); }

		RenderModels();
	}
	CStatistics::CRendering::ProcessedLights(Renderer_uiLightCount);

	RenderParticleSystems(rScene);

	//Update state
	if (rScene.ShadowsNeedUpdate()) { ((CScene&)rScene).ShadowsNeedUpdate(FALSE); }
	((CScene&)rScene).AllChangeStateReset();
}

void __fastcall CRendererIGP::RenderMesh(_IN Resources::CMesh& rMesh)
{
	//Bind buffers
	rMesh.BufferVertexBind();
	rMesh.BufferIndexBind();

	//Specify offsets into vertex buffer data storage
	glVertexPointer(3, GL_FLOAT, sizeof(Float3), NULL);
	glDrawElements(GL_TRIANGLES, rMesh.TriangleCount() * 3, GL_UNSIGNED_SHORT, NULL);
}

void __fastcall CRendererIGP::RenderModels()
{
	//Prepare OpenGL
	glColorMask(1, 1, 1, 0);
	glDepthMask(1);
	glBlendFunc(GL_ONE, GL_ONE);

	UINT uiObjectsProcessed = 0;

	//Render all models
	for (UINT i = 0; i < Renderer_pScene->ModelCount(); i++)
	{
		Renderer_pModel = &Renderer_pScene->Model(i);
		if (Renderer_pModel->VisibleToCamera()) 
		{
			glPushMatrix();
			{
				//Store material usage for later referencing
				Renderer_pMaterialUsage = Renderer_pModel->Material().Usage(*Renderer_pLight);

				if (Renderer_pMaterialUsage)
				{
					Renderer_pMaterialUsage->Apply();
					AttributesBindMesh(Renderer_pMaterialUsage->Program(), Renderer_pModel->Mesh());
				}

				Renderer_pModel->TransformationApply();
				CUniforms::BindModel();
				RenderMesh(Renderer_pModel->Mesh());
			}
			glPopMatrix();

			uiObjectsProcessed++;
		}
	}

	CStatistics::CRendering::ProcessedObjects(uiObjectsProcessed);
}

void __fastcall CRendererIGP::RenderParticleSystems(_IN CScene& rScene)
{
	UINT uiObjectsProcessed = 0;

	glDepthMask(0);
	glEnable(GL_POINT_SPRITE);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	{
		glBindBufferARB(GL_ARRAY_BUFFER, 0);
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);

		for (UINT i = 0; i < rScene.ParticleSystemCount(); i++)
		{
			if (rScene.CameraCurrent()->CheckVisibilityByBounds(rScene.ParticleSystem(i)))
			{
				RenderParticleSystem(rScene.ParticleSystem(i));
				uiObjectsProcessed++;
			}
		}
	}
	glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glDisable(GL_POINT_SPRITE);
	glDepthMask(1);

	CStatistics::CRendering::ProcessedObjects(CStatistics::CRendering::ProcessedObjects() + uiObjectsProcessed);
}

void __fastcall CRendererIGP::RenderParticleSystem(_IN Simulation::CParticleSystem& rParticleSystem)
{
	Renderer_pParticleSystem = (CParticleSystem*)&rParticleSystem;
	//Store material usage for later referencing
	Renderer_pMaterialUsage = Renderer_pParticleSystem->Material().Usage(*Renderer_pLight);

	if (Renderer_pParticleSystem->Material().Transparent())
	{
		glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendFunc(Renderer_pParticleSystem->Material().FactorSource(), Renderer_pParticleSystem->Material().FactorDestination());
	}
	else { glDisable(GL_BLEND); }

	if (Renderer_pMaterialUsage)
	{
		Renderer_pMaterialUsage->Apply();
		AttributesBindParticleSystem(Renderer_pMaterialUsage->Program(), rParticleSystem);
	}

	CUniforms::BindParticleSystem();

	glVertexPointer(3, GL_FLOAT, sizeof(Float4), rParticleSystem.Positions());
	glDrawArrays(GL_POINTS, 0, rParticleSystem.ParticleCount());
}