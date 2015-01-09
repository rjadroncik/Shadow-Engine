#pragma once

#include "Scene.h"

namespace Rendering
{
	class SECORE_API CRendererDebug
	{
	public:

		class SECORE_API Mesh
		{
		public:
			void RenderTBNs(_IN Resources::CMesh& rMesh);
			void RenderSeam(_IN Resources::CMesh& rMesh);
		};

		class SECORE_API Scene
		{
		public:
			static inline void Bounds(_IN bool bRenderBounds)				{ s_Display.bBounds = bRenderBounds; }
			static inline bool Bounds()										{ return s_Display.bBounds; }

			static inline void Cameras(_IN bool bRenderCameras)				{ s_Display.bCameras = bRenderCameras; }
			static inline bool Cameras()									{ return s_Display.bCameras; }

			static inline void Lights(_IN bool bRenderLights)				{ s_Display.bLights = bRenderLights; }
			static inline bool Lights()										{ return s_Display.bLights; }

			static inline void TBNs(_IN bool bRenderTBNs)					{ s_Display.bTBNs = bRenderTBNs; }
			static inline bool TBNs()										{ return s_Display.bTBNs; }	

			static inline void ShadowCones(_IN bool bRenderShadowCones)		{ s_Display.bShadowCones = bRenderShadowCones; }
			static inline bool ShadowCones()								{ return s_Display.bShadowCones; }	

		public:
			//Data processing visualization
			static void RenderWireFrame        (_IN CScene& rScene, _IN bool bFillDepthBuffer);
			static void RenderShadowCones      (_IN CScene& rScene);
			static void RenderLights           (_IN CScene& rScene);
			static void RenderCameras          (_IN CScene& rScene);
			static void BoundsRender           (_IN CScene& rScene);
			static void RenderTBNs             (_IN CScene& rScene);
			static void RenderDataVisualisation(_IN CScene& rScene);
		
		private:
			struct SDisplay
			{
				//Various data visualization options
				bool bBounds;
				bool bShadowScissors;
				bool bCameras;
				bool bLights;
				bool bTBNs;
				bool bShadowCones;

			} static s_Display;
		};

	private:
		CRendererDebug();
		~CRendererDebug();
	};
};