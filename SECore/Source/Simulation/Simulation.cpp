#include "Simulation.h"

#include "PhysicsSolver.h"

#include "../System/System.h"
#include "../Rendering/RendererDGC.h"

#include <SCFWinAPIUndef.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <SCFTimer.h>

using namespace SCFTimer;
using namespace Simulation;
using namespace System;

namespace Simulation
{
	struct SSimulationObjects 
	{
		CScene* pScene; 

	} static Simulation_Objects;

	struct SSimulationState 
	{
		struct STiming
		{
			struct SSettings
			{
				UINT uiUpdateRate;
				float fUpdateTime;

			} Settings;

			UINT uiUpdateCountTotal;

		} Timing;

		bool bRunning;

	} static Simulation_State;
};

inline CScene* CSimulation::Scene()               { return Simulation_Objects.pScene; }
inline void    CSimulation::Scene(CScene* pScene) { Simulation_Objects.pScene = pScene; }

void CSimulation::Pause()  { Simulation_State.bRunning = FALSE; }
void CSimulation::Resume() { Simulation_State.bRunning = TRUE; }

inline bool CSimulation::IsRunning() { return Simulation_State.bRunning; }

inline UINT CSimulation::UpdateCountTotal()   { return Simulation_State.Timing.uiUpdateCountTotal; }


inline void      CSimulation::CSettings::UpdateRate(UINT uiUpdateRate) { Simulation_State.Timing.Settings.uiUpdateRate = uiUpdateRate; }
inline UINT CSimulation::CSettings::UpdateRate()                       { return Simulation_State.Timing.Settings.uiUpdateRate; }

inline void  CSimulation::CSettings::UpdateTime(float fUpdateTime) { Simulation_State.Timing.Settings.fUpdateTime = fUpdateTime; }
inline float CSimulation::CSettings::UpdateTime()                  { return Simulation_State.Timing.Settings.fUpdateTime; }


void CSimulation::Start()
{
	CEventLog::EventNew(STRING("Simulation starting"));

	Simulation_State.bRunning = TRUE;

	Simulation_State.Timing.uiUpdateCountTotal = 0;

	Simulation_State.Timing.Settings.uiUpdateRate = 50;
	Simulation_State.Timing.Settings.fUpdateTime = 1.0f / Simulation_State.Timing.Settings.uiUpdateRate;
}

void CSimulation::Next()
{
	if (IsRunning())
	{
		if (Simulation_Objects.pScene) { Simulation_Objects.pScene->Update(); }

		Simulation_State.Timing.uiUpdateCountTotal++;
	}
}