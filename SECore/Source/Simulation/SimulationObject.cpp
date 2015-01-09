#include "Simulation.h"
#include "SimulationObject.h"

using namespace Simulation;

Float3 CSimulationObject::m_TempVector3a;
Float3 CSimulationObject::m_TempVector3b;

float CSimulationObject::m_GravAcceleration = 980.0f / (CSimulation::CSettings::UpdateRate() * CSimulation::CSettings::UpdateRate());

CSimulationObject::CSimulationObject()
{
	m_Weight = 1;

	ZeroVector3(m_MassCenter);
	ZeroVector3(m_Velocity);
	ZeroVector3(m_Acceleration);

	m_AirResistance = 0.005f;

	m_Static = TRUE;
	m_Inert = TRUE;

	m_pPrimitive = NULL;
	m_PrimitiveCount = 0;
}

CSimulationObject::~CSimulationObject()
{
}

void CSimulationObject::ApplyForce(_IN Float3& Force, _IN _OPT Float3* Point)
{
	if (!Point)
	{
		DivideVector3(m_TempVector3a, Force, m_Weight);
		AddVectors3(m_Acceleration, m_Acceleration, m_TempVector3a);
	}
	else
	{
	
	}
}

void CSimulationObject::Update()
{
//Apply gravity
	m_Velocity[1] -= m_GravAcceleration;

//Apply acceleration
	AddVectors3(m_Velocity, m_Velocity, m_Acceleration);
	ScaleVector3(m_Velocity, m_Velocity, 1 - m_AirResistance);

//Apply velocity
	AddVectors3((Float3&)m_Position, (Float3&)m_Position, m_Velocity);
}