#pragma once

#include "../SECore.h"

namespace Rendering { class SECORE_API CRenderer; class SECORE_API CRendererDGC; }
namespace System
{
	class SECORE_API CRTLoop;
	class SECORE_API CStatistics
	{
	public:
		class SECORE_API CRendering
		{
			friend class CRenderer;
			friend class CRendererDGC;
			friend class CRendererIGP;

		public:
			static inline SCF::UINT ProcessedObjects() { return s_uiProcessedObjects; }
			static inline SCF::UINT ProcessedLights()  { return s_uiProcessedLights; }
			static inline SCF::UINT ProcessedShadows() { return s_uiProcessedShadows; }

		private:
			static void Reset();

		private:
			static inline void ProcessedObjects(_IN SCF::UINT uiValue) { s_uiProcessedObjects = uiValue; }
			static inline void ProcessedLights (_IN SCF::UINT uiValue) { s_uiProcessedLights  = uiValue; }
			static inline void ProcessedShadows(_IN SCF::UINT uiValue) { s_uiProcessedShadows = uiValue; }

		private:
			static SCF::UINT s_uiProcessedObjects;
			static SCF::UINT s_uiProcessedLights;
			static SCF::UINT s_uiProcessedShadows;
		};

	public:
		class SECORE_API CRealTime
		{
			friend class CRTLoop;

		public:
			static inline float UPS() { return s_fUPS; }
			static inline float RPS() { return s_fRPS; }
			static inline float FPS() { return s_fFPS; }

		private:
			static inline void UPS(_IN float fValue) { s_fUPS = fValue; }
			static inline void RPS(_IN float fValue) { s_fRPS = fValue; }
			static inline void FPS(_IN float fValue) { s_fFPS = fValue; }

		private:
			static float s_fUPS;
			static float s_fRPS;
			static float s_fFPS;
		};

	private:
		CStatistics();
		~CStatistics();
	};
};
