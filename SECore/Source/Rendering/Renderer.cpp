#include "Renderer.h"

#include "../System/Statistics.h"
#include "../System/System.h"

using namespace Rendering;
using namespace Simulation;
using namespace Resources;

namespace Rendering 
{
	SCENE_RENDER CRenderer::Render = NULL;

	const CScene*           Renderer_pScene          = NULL;
	const CCamera*          Renderer_pCamera         = NULL;
	const CModel*           Renderer_pModel          = NULL;
	const CMaterialUsage*   Renderer_pMaterialUsage  = NULL;
	const CParticleSystem*  Renderer_pParticleSystem = NULL;
	const CLight*           Renderer_pLight          = NULL;

	const CModel** Renderer_ppModels       = NULL;
	const CModel** Renderer_ppModelsShadow = NULL;
	const CLight** Renderer_ppLights       = NULL;

	UINT Renderer_uiModelCount       = 0;
	UINT Renderer_uiModelShadowCount = 0;
	UINT Renderer_uiLightCount       = 0;

	//Levels of depth maps from highest to lowest resolution
	const CTexture* Renderer_paShadowMaps[LOD_LEVELS][2] = { NULL, NULL,  NULL, NULL,  NULL, NULL };

	const CGPUProgram* Renderer_pShadowMapProgram = NULL;
	UINT          Renderer_uiShadowMapBuffer = 0;

	struct SGPUProgramState
	{
		struct SLast
		{
			//These are used to eliminate redundant state changes
			ENUM uiaAttributes[32];
			UINT uiAttributesCount;

			GLhandleARB hProgram;

		} Last;

	} GPUProgram_State;
};

inline void CRenderer::GPUProgramUnbind() 
{ 
	if (GPUProgram_State.Last.hProgram != 0) 
	{ 
		glUseProgramObjectARB(0);
		GPUProgram_State.Last.hProgram = 0;
	} 
}

inline void CRenderer::GPUProgramBind(_IN CGPUProgram& rProgram)
{ 
	if (GPUProgram_State.Last.hProgram != rProgram.Handle()) 
	{
		glUseProgramObjectARB(rProgram.Handle());
		GPUProgram_State.Last.hProgram = rProgram.Handle();
	}
}

void __fastcall CRenderer::GPUProgramAttributesDisable()
{
	for (UINT i = 0; i < GPUProgram_State.Last.uiAttributesCount; i++) 
	{
		glDisableVertexAttribArrayARB(GPUProgram_State.Last.uiaAttributes[i]);
	}

	GPUProgram_State.Last.uiAttributesCount = 0;
}

void __fastcall CRenderer::AttributesBindMesh(_IN CGPUProgram& rProgram, _IN CMesh& rMesh)
{
	//Disable unneeded texture units
	for (UINT i = rProgram.AttributesCount(); i < GPUProgram_State.Last.uiAttributesCount; i++) 
	{
		glDisableVertexAttribArrayARB(GPUProgram_State.Last.uiaAttributes[i]);
	}

	for (UINT i = 0; i < rProgram.AttributesCount(); i++) 
	{
		for (UINT j = 0; j < rMesh.AttributesCount(); j++)
		{
			if (rMesh.Attribute(j)->eDataType == rProgram.Attribute(i)->eSource)
			{
				glEnableVertexAttribArrayARB(rProgram.Attribute(i)->uiTarget);
				glBindBufferARB(GL_ARRAY_BUFFER, rMesh.Attribute(j)->uiBufferID);
				glVertexAttribPointerARB(rProgram.Attribute(i)->uiTarget, rMesh.Attribute(j)->uiComponents, GL_FLOAT, FALSE, 0, NULL);
				break;
			}
		}

		//Mark the target of the texture for clearing purposes
		GPUProgram_State.Last.uiaAttributes[i] = rProgram.Attribute(i)->uiTarget;
	}

	GPUProgram_State.Last.uiAttributesCount = rProgram.AttributesCount();
}

void __fastcall CRenderer::AttributesBindParticleSystem(_IN CGPUProgram& rProgram, _IN CParticleSystem& rParticleSystem)
{
	//Disable unneeded texture units
	for (UINT i = rProgram.AttributesCount(); i < GPUProgram_State.Last.uiAttributesCount; i++) 
	{
		glDisableVertexAttribArrayARB(GPUProgram_State.Last.uiaAttributes[i]);
	}

	for (UINT i = 0; i < rProgram.AttributesCount(); i++) 
	{
		if (rProgram.Attribute(i)->eSource == AS_VERTEX_TEXCOORD0)
		{
			glEnableVertexAttribArrayARB(rProgram.Attribute(i)->uiTarget);
			glBindBufferARB(GL_ARRAY_BUFFER, 0);
			glVertexAttribPointerARB(rProgram.Attribute(i)->uiTarget, 4, GL_FLOAT, FALSE, 0, rParticleSystem.Properties());
			return;
		}

		//Mark the target of the texture for clearing purposes
		GPUProgram_State.Last.uiaAttributes[i] = rProgram.Attribute(i)->uiTarget;
	}

	GPUProgram_State.Last.uiAttributesCount = rProgram.AttributesCount();
}

bool __fastcall CRenderer::Initialize()
{
	static bool s_bInitalized = FALSE;
	if (!s_bInitalized)
	{
		Renderer_ppModels       = (const CModel**)CMemory::Aligned16Allocate(CRendererSettings::MaxModels() * sizeof(const CModel*));
		Renderer_ppModelsShadow = (const CModel**)CMemory::Aligned16Allocate(CRendererSettings::MaxModels() * sizeof(const CModel*));
		Renderer_ppLights       = (const CLight**)CMemory::Aligned16Allocate(CRendererSettings::MaxLights() * sizeof(const CLight*));

		//Prepare depth map textures & framebuffer
		{
			UINT uiShadowMapResolution = CRendererSettings::MaxShadowMapResolution();

			for (UINT i = 0; i < LOD_LEVELS; i++)
			{
				for (UINT j = 0; j < 2; j++)
				{
					CTexture* pTexture = new CTexture(GL_TEXTURE_2D);

					pTexture->Minification(GL_LINEAR);
					pTexture->Minification(GL_LINEAR);
					pTexture->CoordClamp  (GL_CLAMP);
					pTexture->MaxAnisotropy(1);

					pTexture->Size(uiShadowMapResolution, uiShadowMapResolution);
					pTexture->FormatInternal(GL_DEPTH_COMPONENT);
					pTexture->Format        (GL_DEPTH_COMPONENT);
					pTexture->DataType      (GL_FLOAT);

					pTexture->UpdateProperties();
					pTexture->UpdateData(NULL);

					glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

					Renderer_paShadowMaps[i][j] = pTexture;
				}
				uiShadowMapResolution >>= 1;
			}

			glGenFramebuffersEXT(1, &Renderer_uiShadowMapBuffer);
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, Renderer_uiShadowMapBuffer);
			{
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, Renderer_paShadowMaps[0][0]->GLID(), 0);

				glDrawBuffer(GL_NONE);
				glReadBuffer(GL_NONE);

				switch (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT))
				{                                     
				case GL_FRAMEBUFFER_COMPLETE_EXT: { break; }                             
				case GL_FRAMEBUFFER_UNSUPPORTED_EXT:                                        
				default: 
					{ 
						SCFError(ErrorHardwareUnsupported); 
						return FALSE;
					}                                          
				}
			}
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		}

		//Prepare depth map shader
		{
			CGPUProgram* pShadowMap = new CGPUProgram();

			pShadowMap->ShaderVertex  (STRING("\\Common\\Shaders\\ShadowMap.vs"));
			pShadowMap->ShaderFragment(STRING("\\Common\\Shaders\\ShadowMap.fs"));
			pShadowMap->CompileAndLink();

			Renderer_pShadowMapProgram = pShadowMap;
		}

		GPUProgram_State.Last.hProgram = NULL;
		GPUProgram_State.Last.uiAttributesCount = 0;

		return TRUE;
	}

	return FALSE;
}

inline const CTexture* CRenderer::ShadowMapFront(_IN UINT uiLOD) { return Renderer_paShadowMaps[uiLOD][0]; }
inline const CTexture* CRenderer::ShadowMapBack (_IN UINT uiLOD) { return Renderer_paShadowMaps[uiLOD][1]; }

void __fastcall CRenderer::ShadowMapBindFront()
{
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, ShadowMapFront(Renderer_pLight->ShadowMapLOD())->GLID());
}

void __fastcall CRenderer::ShadowMapBindBack()
{
	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, ShadowMapBack(Renderer_pLight->ShadowMapLOD())->GLID());
}


void __fastcall CRenderer::LightsUpdateState()
{
	for (UINT i = 0; i < Renderer_pScene->LightCount(); i++) 
	{ 
		register CLight& rLight = Renderer_pScene->Light(i);

		if (rLight.Enabled())
		{
			if (rLight.Changed()) { rLight.Update(); }

			rLight.VisibleToCamera(Renderer_pCamera->CheckVisibilityByBounds(rLight));
			if (rLight.VisibleToCamera())
			{
				rLight.DistanceToCamera(MeasureDistance3((Float3&)rLight.Position(), (Float3&)Renderer_pCamera->Position()));
			}
		}
	}
}

void __fastcall CRenderer::LightsChoose()
{
	Renderer_uiLightCount = 0;

	UINT uiMaxLights = __min(Renderer_pScene->LightCount(), CRendererSettings::MaxLights());

	for (UINT i = 0; i < uiMaxLights; i++)
	{
		if (Renderer_pScene->Light(i).VisibleToCamera())
		{
			Renderer_ppLights[Renderer_uiLightCount] = &Renderer_pScene->Light(i);
			Renderer_uiLightCount++;
		}
	}

	if (!Renderer_uiLightCount) { Renderer_ppLights[0] = &Renderer_pScene->Light(0); Renderer_uiLightCount = 1; }
}

int __fastcall CRenderer::QuickSortPartition(_INOUT const CObjectScene** ppObjects, _IN int iLeft, _IN int iRight, _IN int iPivotIndex)
{
	float fPivotValue = ppObjects[iPivotIndex]->DistanceToCamera();

	QuickSortSwap(ppObjects, iPivotIndex, iRight);

	int iStoreIndex = iLeft;

	for (int i = iLeft; i < iRight; i++)
	{
		if (ppObjects[i]->DistanceToCamera() <= fPivotValue)
		{
			QuickSortSwap(ppObjects, iStoreIndex, i);
			iStoreIndex++;
		}
	}

	if (ppObjects[iRight]->DistanceToCamera() <= ppObjects[iStoreIndex]->DistanceToCamera())
	{
		QuickSortSwap(ppObjects, iRight, iStoreIndex);
		return iStoreIndex;
	} 
	else { return iRight; }
}

void __fastcall CRenderer::QuickSort(_INOUT const CObjectScene** ppObjects, _IN int iLeft, _IN int iRight)
{
	if (iRight > iLeft)
	{
		int iPivotNewIndex = QuickSortPartition(ppObjects, iLeft, iRight, iLeft);
		QuickSort(ppObjects, iLeft, iPivotNewIndex - 1);
		QuickSort(ppObjects, iPivotNewIndex + 1, iRight);
	}
}

void __fastcall CRenderer::RenderListUpdate()
{
	Renderer_uiModelCount = 0;

	UINT uiMaxModels = __min(Renderer_pScene->ModelCount(), CRendererSettings::MaxModels());

	for (UINT i = 0; i < uiMaxModels; i++)
	{
		register CModel& rModel = Renderer_pScene->Model(i);

		rModel.VisibleToCamera(Renderer_pCamera->CheckVisibilityByBounds(rModel));
		
		if (rModel.VisibleToCamera())
		{
 			rModel.DistanceToCamera(MeasureDistance3((Float3&)rModel.Position(), (Float3&)Renderer_pCamera->Position()));

			Renderer_ppModels[Renderer_uiModelCount] = &rModel;
			Renderer_uiModelCount++;
		}
	}

	QuickSort((const CObjectScene**)Renderer_ppModels, 0, Renderer_uiModelCount - 1);
}

void __fastcall CRenderer::RenderListShadowUpdate()
{
	Renderer_uiModelShadowCount = 0;

	for (UINT i = 0; i < Renderer_pScene->ModelCount(); i++)
	{
		if (Renderer_pScene->Model(i).ShadowCasts())
		{
			Renderer_ppModelsShadow[Renderer_uiModelShadowCount] = &Renderer_pScene->Model(i);
			Renderer_uiModelShadowCount++;
		}
	}
}

void CRenderer::GUIRender()
{
	if (Renderer_pCamera && (System::CSystem::HandlerGUIRender() != NULL)) 
	{
		Renderer_pCamera->ApplyProjection2D();

		//Prepare OpenGL
		glColorMask(1, 1, 1, 0);
		glDepthMask(0);
		glEnable(GL_BLEND); 
		glBlendFunc(GL_ONE, GL_ONE);

		CMaterialUsage::TexturesDisable();
		GPUProgramAttributesDisable();
		GPUProgramUnbind();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glEnable(GL_TEXTURE_2D);
		glShadeModel(GL_FLAT); 
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_SCISSOR_TEST);
		{
			System::CSystem::HandlerGUIRender()();
		}
		glDisable(GL_SCISSOR_TEST);
		glEnable(GL_DEPTH_TEST);
		glShadeModel(GL_SMOOTH);
		glDisable(GL_TEXTURE_2D);
	}
}