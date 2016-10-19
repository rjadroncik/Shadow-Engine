#include "Statistics.h"

using namespace System;

UINT CStatistics::CRendering::s_uiProcessedObjects = 0;
UINT CStatistics::CRendering::s_uiProcessedLights  = 0;
UINT CStatistics::CRendering::s_uiProcessedShadows = 0;

float CStatistics::CRealTime::s_fUPS = 0;
float CStatistics::CRealTime::s_fRPS = 0;
float CStatistics::CRealTime::s_fFPS = 0;

void CStatistics::CRendering::Reset()
{
	//Reset statistics
	s_uiProcessedObjects = 0;
	s_uiProcessedLights  = 0;
	s_uiProcessedShadows = 0;
}
