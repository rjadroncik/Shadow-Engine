#include "RTLoop.h"

#include "System.h"
#include "Statistics.h"

#include "../Simulation/Simulation.h"
#include "../Rendering/Renderer.h"

#include <SCFWinAPIUndef.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <SCFTimer.h>

using namespace SCFTimer;
using namespace System;
using namespace Simulation;
using namespace Rendering;

CRTLoop::SState CRTLoop::s_State = { FALSE,  0, 0, 0, 0,  0, 0, 0,  0 };			 
CRTLoop::SHandlers CRTLoop::s_Handlers = { NULL, NULL };			 

void CRTLoop::Start()
{
	CEventLog::EventNew(STRING("Real-time loop starting"));

	s_State.bRunning = TRUE;

	s_State.Timing.uiUpdateCount = 0;
	s_State.Timing.fLag = 0;
	s_State.Timing.fUpdateTime = 0;
	s_State.Timing.fRenderTime = 0;


	s_State.Statistics.uiUpdateCountPS = 0;
	s_State.Statistics.uiRenderCountPS = 0;

	s_State.Statistics.fUpdateTimePS = 0;
	s_State.Statistics.fRenderTimePS = 0;

	s_State.uiUpdatesSinceRender = 0;
}

//Perform simulation in a timed manner - fixed UPS, adapted RPS
void CRTLoop::Next()
{
	//We need to wait for the precise time
	if (GetTime02() < CSimulation::CSettings::UpdateTime()) { return; }

	//This timer measures the total time from start of frame processing
	ResetTime02();

	//Perform timed rendering
	ResetTime01();
	{
		Render(); 
		Rendering::CRenderer::GUIRender();
	}
	s_State.Timing.fRenderTime = GetTime01();
	s_State.Statistics.fRenderTimePS += s_State.Timing.fRenderTime;

	//Update the scene as many times as necessary & measure the needed time
	ResetTime01();
	{
		Update();	

		while (s_State.Timing.uiUpdateCount > 0) 
		{ 
			Update(); 
			s_State.Timing.uiUpdateCount--; 
		}
	}
	s_State.Timing.fUpdateTime = GetTime01();
	s_State.Statistics.fUpdateTimePS += s_State.Timing.fUpdateTime;

	CWindow::SwapBuffers(); 

	//Compensate lag by computing the number of consecutive scene
	//updates necessary to synchronize simulation time with real time 
	{
		float fFrameLag = GetTime02() - CSimulation::CSettings::UpdateTime();
		if (fFrameLag > 0) { s_State.Timing.fLag += fFrameLag; } 

		while (s_State.Timing.fLag > CSimulation::CSettings::UpdateTime())
		{ 
			s_State.Timing.fLag -= CSimulation::CSettings::UpdateTime(); 
			s_State.Timing.uiUpdateCount++; 
		}
	}

	//Compute real-time statistics once per second
	if (s_State.Statistics.uiUpdateCountPS >= CSimulation::CSettings::UpdateRate())
	{
		CStatistics::CRealTime::UPS(s_State.Statistics.uiUpdateCountPS / s_State.Statistics.fUpdateTimePS);
		CStatistics::CRealTime::RPS(s_State.Statistics.uiRenderCountPS / s_State.Statistics.fRenderTimePS);

		CStatistics::CRealTime::FPS(1 / (s_State.Statistics.fUpdateTimePS / s_State.Statistics.uiUpdateCountPS + s_State.Statistics.fRenderTimePS / s_State.Statistics.uiRenderCountPS));

		s_State.Statistics.uiUpdateCountPS = 0;
		s_State.Statistics.uiRenderCountPS = 0;

		s_State.Statistics.fRenderTimePS   = 0;
		s_State.Statistics.fUpdateTimePS   = 0;
	}

	//If we finished early, sleep till it's time for next frame
	if (GetTime02() < (CSimulation::CSettings::UpdateTime() * 0.8f)) 
	{
		SleepEx((UINT)((CSimulation::CSettings::UpdateTime() - GetTime02()) * 800), TRUE);
	}
}

void CRTLoop::Update()
{
	if (s_Handlers.pUpdateStart) { s_Handlers.pUpdateStart(); }

	CSimulation::Next();

	s_State.Statistics.uiUpdateCountPS++;
	s_State.uiUpdatesSinceRender++;

	if (s_Handlers.pUpdateEnd) { s_Handlers.pUpdateEnd(); }
}

void CRTLoop::Render()
{
	if (CSimulation::Scene()) { Rendering::CRenderer::Render(*CSimulation::Scene()); }

	s_State.Statistics.uiRenderCountPS++;
	s_State.uiUpdatesSinceRender = 0;

	//int x = 0;

	//for (UINT i = 0; i < 10000000; i++)
	//{
	//	x = __abs(x*x*x*x*x);
	//}
}