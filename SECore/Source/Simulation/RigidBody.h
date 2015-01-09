#pragma once
#include "SimulationObject.h"

namespace Simulation
{
	class SECORE_API CRigidBody : public CSimulationObject
	{
	public:
		CRigidBody();
		virtual ~CRigidBody();

	protected:

		static Float3	m_TempVector3a;
		static Float3	m_TempVector3b;
	};
};