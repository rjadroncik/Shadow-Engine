#include "RendererDGC.h"

#include "Uniforms.h"

#include "../Simulation/Simulation.h"
#include "../System/Statistics.h"

using namespace Rendering;
using namespace System;
using namespace Resources;
using namespace Simulation;

namespace Rendering 
{
	UINT* RendererDGC_uipQueries = NULL;

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

bool __fastcall CRendererDGC::Initialize()
{
	static bool s_bInitalized = FALSE;
	if (!s_bInitalized)
	{
		CEventLog::BlockNew(STRING("Initializing renderer for OpenGL 2.0 DGC codepath"));

		CRenderer::Initialize();
		CUniforms::Initialize();

		CRenderer::Render = CRendererDGC::Render;

		//Prepare renderer for visibility queries
		{
			RendererDGC_uipQueries = (UINT*)CMemory::Aligned16Allocate(CRendererSettings::MaxModels() * sizeof(UINT));
			glGenQueriesARB(CRendererSettings::MaxModels(), RendererDGC_uipQueries);
		}

		CEventLog::BlockClose();

		return TRUE;
	}

	return FALSE;
}

void CRendererDGC::VisibilityResolve()
{
	//Prepare OpenGL
	glColorMask(1, 1, 1, 1);
	glDepthMask(1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColorMask(0, 0, 0, 0);
	glDisable(GL_BLEND);

	GPUProgramAttributesDisable();
	CMaterialUsage::TexturesDisable();
	GPUProgramBind(*Renderer_pShadowMapProgram);

	//Sort models based on distance, perform analytic visibility checks, etc.
	RenderListUpdate();

	Renderer_pCamera->ApplyProjection3D();

	//Query visibility information from OpenGL
	for (UINT i = 0; i < Renderer_uiModelCount; i++)
	{
		glBeginQueryARB(GL_SAMPLES_PASSED_ARB, RendererDGC_uipQueries[i]);
		glPushMatrix();
		{
			Renderer_ppModels[i]->TransformationApply();
			RenderMesh(Renderer_ppModels[i]->Mesh());
		}
		glPopMatrix();
		glEndQueryARB(GL_SAMPLES_PASSED_ARB);
	}

	//Finalize visibility determination
	for (UINT i = 0; i < Renderer_uiModelCount; i++)
	{
		static UINT RendererDGC_uiSamples;
		glGetQueryObjectuivARB(RendererDGC_uipQueries[i], GL_QUERY_RESULT_ARB, &RendererDGC_uiSamples);

		if (RendererDGC_uiSamples < 16) { ((CModel*)Renderer_ppModels[i])->VisibleToCamera(FALSE); }
	}

	CMaterialUsage::LastReset();
}

void CRendererDGC::LightDepthMapUpdate()
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

void __fastcall CRendererDGC::Render(_IN CScene& rScene)
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
		RenderModels();
	}
	CStatistics::CRendering::ProcessedLights(Renderer_uiLightCount);

	RenderParticleSystems(rScene);
	
	//Update state
	if (rScene.ShadowsNeedUpdate()) { ((CScene&)rScene).ShadowsNeedUpdate(FALSE); }
	((CScene&)rScene).AllChangeStateReset();
}

void __fastcall CRendererDGC::RenderMesh(_IN Resources::CMesh& rMesh)
{
	//Bind buffers
	rMesh.BufferVertexBind();
	rMesh.BufferIndexBind();

	//Specify offsets into vertex buffer data storage
	glVertexPointer(3, GL_FLOAT, sizeof(Float3), NULL);
	glDrawElements(GL_TRIANGLES, rMesh.TriangleCount() * 3, GL_UNSIGNED_SHORT, NULL);
}

void __fastcall CRendererDGC::RenderModels()
{
	//Prepare OpenGL
	glColorMask(1, 1, 1, 0);
	glDepthMask(0);
	glEnable(GL_BLEND); 
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

void __fastcall CRendererDGC::RenderParticleSystems(_IN CScene& rScene)
{
	UINT uiObjectsProcessed = 0;

	//MUST BE CALLED BEFORE ENABLING POINT SPRITES .. ROFL
	glActiveTexture(GL_TEXTURE0);
	glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
	glPointParameterfARB(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);

	glShadeModel(GL_FLAT);
	glDepthMask(0);

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0);

	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glEnable(GL_POINT_SPRITE);
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
	glDisable(GL_POINT_SPRITE);
	glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glDisable(GL_ALPHA_TEST);
	glDepthMask(1);
	glShadeModel(GL_SMOOTH);

	glActiveTexture(GL_TEXTURE0);
	glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_FALSE);

	CStatistics::CRendering::ProcessedObjects(CStatistics::CRendering::ProcessedObjects() + uiObjectsProcessed);
}

void __fastcall CRendererDGC::RenderParticleSystem(_IN Simulation::CParticleSystem& rParticleSystem)
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
