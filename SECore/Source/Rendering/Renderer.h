#pragma once

#include "Scene.h"
#include "RendererSettings.h"

namespace Rendering
{
	typedef void (__fastcall *SCENE_RENDER)(_IN CScene&);
	typedef void (__fastcall *UNIFORM_BINDER)(_IN SCF::UINT uiTarget);

	class SECORE_API CRenderer
	{
	public:
		static SCENE_RENDER Render;

	public:
		static void GUIRender();

	public:
		static const Resources::CTexture* ShadowMapFront(_IN SCF::UINT uiLOD);
		static const Resources::CTexture* ShadowMapBack (_IN SCF::UINT uiLOD);

		static void __fastcall ShadowMapBindFront();
		static void __fastcall ShadowMapBindBack();

	public:
		static void __fastcall GPUProgramBind(_IN Resources::CGPUProgram& rProgram);
		static void __fastcall GPUProgramUnbind();
		static void __fastcall GPUProgramAttributesDisable();

	protected:
		static void __fastcall AttributesBindMesh          (_IN Resources::CGPUProgram& rProgram, _IN Resources::CMesh& rMesh);
		static void __fastcall AttributesBindParticleSystem(_IN Resources::CGPUProgram& rProgram, _IN Simulation::CParticleSystem& rParticleSystem);

	protected:
		//Prepares a list of meshes to be rendered based on distance etc..
		static void __fastcall RenderListUpdate();
		static void __fastcall RenderListShadowUpdate();

		//Chooses lights which should be used based on various metrics
		static void __fastcall LightsUpdateState();
		static void __fastcall LightsChoose();
	
	protected:
		static int  __fastcall QuickSortPartition(_INOUT const CObjectScene** ppObjects, _IN int iLeft, _IN int iRight, _IN int iPivotIndex);
		static void __fastcall QuickSort         (_INOUT const CObjectScene** ppObjects, _IN int iLeft, _IN int iRight);

		static inline void QuickSortSwap(_INOUT const CObjectScene** ppObjects, _IN int iFirst, _IN int iSecond) { register const CObjectScene* const pTemp = ppObjects[iFirst]; ppObjects[iFirst] = ppObjects[iSecond]; ppObjects[iSecond] = pTemp; }

	protected:
		static bool __fastcall Initialize();

	private:
		CRenderer();
		~CRenderer();
	};
};
