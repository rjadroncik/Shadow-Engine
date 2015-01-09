#pragma once

#include "../Rendering/ObjectScene.h"
#include "SimulationPrimitive.h"

namespace Simulation
{
	class ISimulationObject
	{
	public:
		virtual void __fastcall ApplyForce(_IN Float3& rForce, _IN _OPT Float3* pPoint = NULL) = 0;
	};

	class SECORE_API CSimulationObject : public Rendering::CObjectScene, public ISimulationObject
	{
	public:
		CSimulationObject();
		virtual ~CSimulationObject();

		inline static void	SetGravAcceleration(float GravAcceleration)	{ m_GravAcceleration = GravAcceleration; }
		inline static float	GetGravAcceleration()							{ return m_GravAcceleration; }

		virtual	void		__fastcall ApplyForce(_IN Float3& rForce, _IN _OPT Float3* pPoint = NULL);
		virtual void		__fastcall Update();

		inline void			ZeroAcceleration()							{ ZeroVector3(m_Acceleration); } 
		inline void			SetAcceleration(Float3& Acceleration)	{ CopyVector3(m_Acceleration, Acceleration); }
		inline Float3&	GetAcceleration()							{ return m_Acceleration; }

		inline void			ZeroVelocity()						{ ZeroVector3(m_Velocity); } 
		inline void			SetVelocity(Float3& Velocity)	{ CopyVector3(m_Velocity, Velocity); }
		inline Float3&	GetVelocity()						{ return m_Velocity; }

		inline void			SetMassCenter(Float3& MassCenter)	{ CopyVector3(m_MassCenter, MassCenter); }
		inline Float3&	GetMassCenter()							{ return m_MassCenter; }

		inline void			SetStatic(bool Static)	{ m_Static = Static; }
		inline bool			GetStatic()					{ return m_Static; }
		
		inline bool			GetInert() { return m_Inert; }

	protected:

		static float m_GravAcceleration;

		static Float3	m_TempVector3a;
		static Float3	m_TempVector3b;

		float			m_Weight;

		Float3		m_MassCenter;
		Float3		m_Velocity;
		Float3		m_Acceleration;

		float			m_AirResistance;

		bool			m_Static;
		bool			m_Inert;

		CSimulationPrimitive* m_pPrimitive;
		SCF::UINT			      m_PrimitiveCount;
	};
};
