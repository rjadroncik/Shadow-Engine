#pragma once

#include "../SECore.h"

namespace Simulation
{
	class SECORE_API CSimulationPrimitive : public SCFBase::CObject
	{
	public:
		CSimulationPrimitive();
		~CSimulationPrimitive();

	//Basic Placement Functions
		inline virtual void	Position(float fX, float fY, float fZ)	{ m_Position[0] = fX;	m_Position[1] = fY;	m_Position[2] = fZ; }
		inline virtual void	PositionOffset(float fX, float fY, float fZ)	{ m_Position[0] += fX;	m_Position[1] += fY;	m_Position[2] += fZ; }

		inline virtual void	Position(Float3& Position)			{ CopyVector3(m_Position, Position); }
		inline virtual void	PositionOffset(Float3& PositionOffset)	{ AddVectors3(m_Position, m_Position, PositionOffset); }

		inline virtual void	Rotation(float fX, float fY, float fZ)	{ m_Rotation[0] = fX;	m_Rotation[1] = fY;	m_Rotation[2] = fZ; }
		inline virtual void	RotationOffset(float fX, float fY, float fZ)	{ m_Rotation[0] += fX;	m_Rotation[1] += fY;	m_Rotation[2] += fZ; }

		inline virtual void	Rotation(Float3& Rotation)			{ CopyVector3(m_Rotation, Rotation); }
		inline virtual void	RotationOffset(Float3& RotationOffset)	{ AddVectors3(m_Rotation, m_Rotation, RotationOffset); }

	protected:

		float		m_Friction;

		Float3	m_Position;
		Float3	m_Rotation;
	};
};
