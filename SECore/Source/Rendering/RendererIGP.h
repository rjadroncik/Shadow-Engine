#pragma once
#include "Renderer.h"

namespace Rendering
{
	class SECORE_API CRendererIGP : public CRenderer
	{
	public:
		static bool __fastcall Initialize();

	public:
		static void __fastcall Render               (_IN CScene& rScene);
		static void __fastcall RenderParticleSystems(_IN CScene& rScene);
		static void __fastcall RenderModels();

	protected:
		static void __fastcall VisibilityResolve();
		static void __fastcall LightDepthMapUpdate();

	protected:
		static void __fastcall RenderParticleSystem(_IN Simulation::CParticleSystem& rParticleSystem);
		static void __fastcall RenderMesh          (_IN Resources::CMesh& rMesh);

	private:
		CRendererIGP();
		~CRendererIGP();
	};
};
