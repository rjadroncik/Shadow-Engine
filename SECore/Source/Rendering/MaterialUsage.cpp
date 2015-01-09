#include "MaterialUsage.h"
#include "Light.h"

#include "../Resources/Texture.h"
#include "../System/OpenGL20.h"
#include "Renderer.h"

#include <SCFMathematics.h>

using namespace SCFMathematics;
using namespace Rendering;
using namespace System;
using namespace Resources;

namespace Rendering
{
	struct SMaterialUsageState
	{
		struct SLast
		{
			//These are used to eliminate redundant state changes
			SCF::ENUM eaTextureTargets[32];
			SCF::UINT uiTexturesCount;

			const CMaterialUsage* pMaterialUsage;

		} Last;

	} static MaterialUsage_State;

	const CGPUProgram* MaterialUsage_pProgramDefault;
};

inline void CMaterialUsage::LastReset() { MaterialUsage_State.Last.pMaterialUsage = NULL; }

CMaterialUsage::CMaterialUsage()
{
	m_pTextures = NULL;
	m_uiTextureCount = 0;

	m_pProgram = MaterialUsage_pProgramDefault;

	m_iShadowMapTargets[0] = -1;
	m_iShadowMapTargets[1] = -1;
}

CMaterialUsage::~CMaterialUsage()
{
	CMemory::Free(m_pTextures);
}

void __fastcall CMaterialUsage::Initialize()
{
	MaterialUsage_State.Last.pMaterialUsage = NULL;
	MaterialUsage_State.Last.uiTexturesCount = 0;

	CEventLog::BlockNew(STRING("Initializing default material"));

	{
		CGPUProgram* pDefault = new CGPUProgram();

		pDefault->ShaderVertex  (STRING("\\Common\\Shaders\\Default.vs"));
		pDefault->ShaderFragment(STRING("\\Common\\Shaders\\Default.fs"));
		pDefault->CompileAndLink();

		MaterialUsage_pProgramDefault = pDefault;
	}

	CEventLog::BlockClose();
}

void __fastcall CMaterialUsage::TexturesDisable()
{
	for (SCF::UINT i = 0; i < MaterialUsage_State.Last.uiTexturesCount; i++) 
	{
		glActiveTexture(GL_TEXTURE2 + i);
		glDisable(MaterialUsage_State.Last.eaTextureTargets[i]);
	}

	MaterialUsage_State.Last.uiTexturesCount = 0;
}

void __fastcall CMaterialUsage::TexturesBind() const
{
	if (m_iShadowMapTargets[0] != -1) { glUniform1iARB(m_iShadowMapTargets[0], 0);}
	else                              { glActiveTexture(GL_TEXTURE0); glDisable(GL_TEXTURE_2D); }
	if (m_iShadowMapTargets[1] != -1) { glUniform1iARB(m_iShadowMapTargets[1], 1);}
	else                              { glActiveTexture(GL_TEXTURE1); glDisable(GL_TEXTURE_2D); }

	//Disable unneeded texture units
	for (SCF::UINT i = m_uiTextureCount; i < MaterialUsage_State.Last.uiTexturesCount; i++) 
	{
		glActiveTexture(GL_TEXTURE2 + i);
		glDisable(MaterialUsage_State.Last.eaTextureTargets[i]);
	}

	for (SCF::UINT i = 0; i < m_uiTextureCount; i++) 
	{
		register const CTexture* const pTexture = m_pTextures[i].pTexture;

		glActiveTexture(GL_TEXTURE2 + i);
		glEnable(pTexture->Target());
		glBindTexture(pTexture->Target(), pTexture->GLID());

		//Set the value of the specified sampler uniform
		glUniform1iARB(m_pTextures[i].uiSampler, 2 + i);

		//Mark the target of the texture for clearing purposes
		MaterialUsage_State.Last.eaTextureTargets[i] = pTexture->Target();
	}

	MaterialUsage_State.Last.uiTexturesCount = m_uiTextureCount;
}

void __fastcall CMaterialUsage::TextureAdd(_IN _REF Resources::CTexture& rTexture, _IN SCF::UINT uiSampler) _SET
{
	m_uiTextureCount++;
	m_pTextures = (STextureBond*)CMemory::Reallocate(m_pTextures, sizeof(STextureBond) * m_uiTextureCount);

	m_pTextures[m_uiTextureCount - 1].pTexture  = &rTexture;
	m_pTextures[m_uiTextureCount - 1].uiSampler = uiSampler;
}

void __fastcall CMaterialUsage::Apply() const
{
	if (MaterialUsage_State.Last.pMaterialUsage != this)
	{
		CRenderer::GPUProgramBind(*m_pProgram);
		TexturesBind();
		MaterialUsage_State.Last.pMaterialUsage = this;
	}
}