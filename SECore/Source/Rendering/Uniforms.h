#pragma once
#include "Renderer.h"

namespace Rendering
{
	typedef void (__fastcall *UNIFORM_BINDER) (_IN SCF::UINT uiTarget);

	class SECORE_API CUniforms
	{
	public:
		struct SLight 
		{
			float DepthMapTransform[2][16];

		} static Light;

	public:
		static bool __fastcall Initialize();

	public:
		static void __fastcall BindModelOnlyCamera();
		static void __fastcall BindModelOnlyLight();
		static void __fastcall BindModelOnlyMaterial();
		static void __fastcall BindModel();
		static void __fastcall BindParticleSystem();

		//Model specific uniforms
		static void __fastcall BindModelLightPositionLocal (_IN SCF::UINT uiTarget);
		static void __fastcall BindModelCameraPositionLocal(_IN SCF::UINT uiTarget);

		//Particle system specific uniforms

		//Common uniforms
		static void __fastcall BindLightDepthMapTransform0(_IN SCF::UINT uiTarget);

		static void __fastcall BindLightPosition  (_IN SCF::UINT uiTarget);
		static void __fastcall BindLightParameters(_IN SCF::UINT uiTarget);
		static void __fastcall BindLightTint      (_IN SCF::UINT uiTarget);
		static void __fastcall BindLightAmbDifSpe (_IN SCF::UINT uiTarget);

		static void __fastcall BindCameraPosition (_IN SCF::UINT uiTarget);
		static void __fastcall BindCameraViewport (_IN SCF::UINT uiTarget);

		static void __fastcall BindMaterialTint        (_IN SCF::UINT uiTarget);
		static void __fastcall BindMaterialAmbDifSpeEmi(_IN SCF::UINT uiTarget);
		static void __fastcall BindMaterialShininess   (_IN SCF::UINT uiTarget);
	};
};