#pragma once

#include "../System/SEObject.h"
#include "../Resources/Texture.h"
#include "../Resources/GPUProgram.h"

namespace Rendering
{
	enum MaterialUsages
	{
		UsageSpot,
		UsageSpotShadow,
		UsageOmni,
		UsageOmniShadow,
		UsageDirect,
		UsageDirectShadow,

		UsageCount,
	};

	class SECORE_API CMaterialUsage : public System::CSEObject
	{
	public:
		System::Category Category() _GET { return System::Category::Materials; }

	public:
		CString XmlName() _GET { return STRING("MaterialUsage"); }
		CString ToString() _GET { return STRING("{MaterialUsage}"); }

	public:
		//Need to call this if we use GPU program outside the material scope
		static void LastReset();

	public:
		static void	__fastcall Initialize();
		static void __fastcall TexturesDisable();

	public:
		CMaterialUsage();
		virtual ~CMaterialUsage();

	public:
		void __fastcall Apply() const;

	public:
		//Texture bond functions
		void __fastcall  TextureAdd(_IN _REF Resources::CTexture& rTexture, _IN UINT uiSampler) _SET;
		void __fastcall  TexturesBind() const;

		inline UINT TextureCount() _GET { return m_uiTextureCount; }

	public:
		//GPU programs
		inline void                    Program(_IN _REF Resources::CGPUProgram& rProgram) _SET { m_pProgram = &rProgram; }
		inline Resources::CGPUProgram& Program()                                          _GET { return *(Resources::CGPUProgram*)m_pProgram; }

	public:
		inline void ShadowMapTargetFront(_IN int iTarget) _SET { m_iShadowMapTargets[0] = iTarget; }
		inline int  ShadowMapTargetFront()                _GET { return m_iShadowMapTargets[0]; }
	
		inline void ShadowMapTargetBack(_IN int iTarget) _SET { m_iShadowMapTargets[1] = iTarget; }
		inline int  ShadowMapTargetBack()                _GET { return m_iShadowMapTargets[1]; }

	protected:
		const Resources::CGPUProgram* m_pProgram;

		//List of all used textures
		struct STextureBond
		{
			const Resources::CTexture* pTexture; 
			UINT uiSampler; 

		}* m_pTextures;

		UINT m_uiTextureCount;

	protected:
		//Keeps track of the bind points for the shadow maps
		int m_iShadowMapTargets[2];
	};
};
