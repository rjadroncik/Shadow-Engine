#pragma once

#include <SCFStandard.h>

#include "../Resources/World.h"
#include "../Rendering/Scene.h"

namespace Simulation
{
	class SECORE_API CSimulation
	{
	public:
		static CScene* Scene();
		static void    Scene(CScene* pScene);

	public:
		static void Start();
		static void Next();

		static void Pause();
		static void Resume();

		static bool IsRunning();

	public:
		static SCF::UINT UpdateCountTotal();

 		class SECORE_API CSettings
		{
		public:
			static void      UpdateRate(SCF::UINT uiUpdateRate);
			static SCF::UINT UpdateRate();

			static void  UpdateTime(float fUpdateTime);
			static float UpdateTime();
		};

	private:
		static void SceneUpdate();
		static void SceneRender();

	private:
		CSimulation();
		~CSimulation();
	};
};