#pragma once
#include "SimulationPrimitive.h"

namespace Simulation
{
	class SECORE_API CSimulationSphere : public CSimulationPrimitive
	{
	public:
		CSimulationSphere();
		~CSimulationSphere();

	protected:
		float m_Radius;
	};
};
