#include "ParticleSystem.h"
#include "PhysicsSolver.h"

#include "../System/Scripting.h"

#include <limits.h>
#include <xmmintrin.h>

using namespace SCF;
using namespace Simulation;
using namespace Rendering;
using namespace System;

Float3 ParticleSystem_TempVector3;

float*  ParticleSystem_fpRandomFloats1To1 = NULL;
SCF::USHORT* ParticleSystem_uspRandomUSHORT    = NULL;

SCF::BYTE ParticleSystem_ucCurrentFloat  = 0;
SCF::BYTE ParticleSystem_ucCurrentUSHORT = 0;

inline float       CParticleSystem::RandomFloat1To1() { ParticleSystem_ucCurrentFloat++;  return ParticleSystem_fpRandomFloats1To1[ParticleSystem_ucCurrentFloat]; }
inline SCF::USHORT CParticleSystem::RandomUSHORT()    { ParticleSystem_ucCurrentUSHORT++; return ParticleSystem_uspRandomUSHORT[ParticleSystem_ucCurrentUSHORT]; }

void CParticleSystem::Construct(_IN Rendering::CMaterial* pMaterial)
{
	if (!ParticleSystem_fpRandomFloats1To1)
	{
		ParticleSystem_fpRandomFloats1To1 = (float*)CMemory::Allocate(sizeof(float) * (UCHAR_MAX + 1));

		for (SCF::UINT i = 0; i < (UCHAR_MAX + 1); i++)
		{
			ParticleSystem_fpRandomFloats1To1[i] = SCFMathematics::RandomFloat1To1();
		}
	}

	if (!ParticleSystem_uspRandomUSHORT)
	{
		ParticleSystem_uspRandomUSHORT = (SCF::USHORT*)CMemory::Allocate(sizeof(SCF::USHORT) * (UCHAR_MAX + 1));

		for (SCF::UINT i = 0; i < (UCHAR_MAX + 1); i++)
		{
			ParticleSystem_uspRandomUSHORT[i] = (SCF::USHORT)(rand() % USHRT_MAX);
		}
	}

	m_usParticleCount = 0;

	m_uspTimesLeft = NULL;

	m_pPositions  = NULL;
	m_pVelocities = NULL;

	m_pProperties       = NULL;
	m_pPropertiesChange = NULL;

	SetVector3(m_EmitterBox, 100, 100, 100);

	m_usParticleCount = 0;
	m_usParticleLife  = 0;
	m_fParticleSpread = 0;

	m_Properties[3]          = 0;
	m_PropertiesVariation[3] = 0;
	m_PropertiesChange[3]    = 0;

	m_Properties[1]          = 0;
	m_PropertiesVariation[1] = 0;
	m_PropertiesChange[1]    = 0;

	m_Properties[0]          = 0;
	m_PropertiesVariation[0] = 0;
	m_PropertiesChange[0]    = 0;

	m_pMaterial = pMaterial;
	if (m_pMaterial) { ADDREF(*m_pMaterial); }

	m_ucUpdateCounter = 0;
}

CParticleSystem::CParticleSystem()
{
	Construct(NULL);
}

CParticleSystem::CParticleSystem(_IN _REF CMaterial& rMaterial)
{
	Construct(&rMaterial);
}

CParticleSystem::~CParticleSystem()
{
	CMemory::Aligned16Free(m_uspTimesLeft);

	CMemory::Aligned16Free(m_pPositions);
	CMemory::Aligned16Free(m_pVelocities);

	CMemory::Aligned16Free(m_pProperties);
	CMemory::Aligned16Free(m_pPropertiesChange);

	RELEASE(*m_pMaterial);
}

void CParticleSystem::ApplyForce(_IN Float3& rForce, _IN _OPT Float3* pPoint)
{
	for (SCF::USHORT i = 0; i < m_usParticleCount; i++)
	{
		//AddVectors3(m_pVelocities[i], m_pVelocities[i], rForce);
	}
}

void CParticleSystem::UpdatePositionsOnly()
{
	#ifdef _SSE
	{
		register __m128 a;
		register __m128 b;

		for (SCF::USHORT i = 0; i < m_usParticleCount; i++)
		{
			//Velocity - Load
			b = _mm_load_ps(m_pVelocities[i]);
			//Position - Load
			a = _mm_load_ps(m_pPositions[i]);
			//Position - Add - Velocity 
			a = _mm_add_ps(a, b);
			//Position - Store
			_mm_store_ps(m_pPositions[i], a);
		}
	}
	#else
	{
		for (SCF::USHORT i = 0; i < m_usParticleCount; i++)
		{
			AddVectors3((Float3&)m_pPositions[i], (Float3&)m_pPositions[i], (Float3&)m_pVelocities[i]);
		}
	}
	#endif
}

void CParticleSystem::UpdatePositionsAndProperties()
{
	for (SCF::USHORT i = 0; i < m_usParticleCount; i++)
	{
		if (!m_uspTimesLeft[i])
		{
			CreateNewParticle(i);
			continue;
		}

		m_uspTimesLeft[i]--;

		//Apply speed change
		/*NormalizeVector3(ParticleSystem_TempVector3, m_pVelocities[i]);
		ScaleVector3    (ParticleSystem_TempVector3, ParticleSystem_TempVector3, m_pPropertiesChange[i][3]);
		AddVectors3     (m_pVelocities[i], m_pVelocities[i], ParticleSystem_TempVector3);
		*/

		#ifdef _SSE
		{
			register __m128 a;
			register __m128 b;

			//Velocity change - Load
			a = _mm_load_ps1(&m_pPropertiesChange[i][3]);
			//Velocity - Load
			b = _mm_load_ps(m_pVelocities[i]);
			//Velocity - Scale 
			b = _mm_mul_ps(a, b);
			//Velocity - Store 
			_mm_store_ps(m_pVelocities[i], b);

			//Position - Load
			a = _mm_load_ps(m_pPositions[i]);
			//Position - Add - Velocity 
			a = _mm_add_ps(a, b);
			//Position - Store
			_mm_store_ps(m_pPositions[i], a);

			//Properties - Load
			a = _mm_load_ps(m_pProperties[i]);
			//Properties change - Load
			b = _mm_load_ps(m_pPropertiesChange[i]);
			//Properties - Add
			a = _mm_add_ps(a, b);
			//Properties - Store
			//_mm_stream_ps(m_pProperties[i], a);
			_mm_store_ps(m_pProperties[i], a);
		}
		#else
		{
			ScaleVector3((Float3&)m_pVelocities[i], (Float3&)m_pVelocities[i], m_pPropertiesChange[i][3]);
			AddVectors3 ((Float3&)m_pProperties[i], (Float3&)m_pProperties[i], (Float3&)m_pPropertiesChange[i]);
			AddVectors3 ((Float3&)m_pPositions[i],  (Float3&)m_pPositions[i],  (Float3&)m_pVelocities[i]);
		}
		#endif

		m_pProperties[i][3] = cosf(m_pProperties[i][2]);
	}
}

void CParticleSystem::Update()
{
	if (m_ucUpdateCounter < 1) 
	{ 
		m_ucUpdateCounter++; 
		UpdatePositionsOnly();
	}
	else
	{
		m_ucUpdateCounter = 0;
		UpdatePositionsAndProperties();
	}
}

void CParticleSystem::Create()
{
	m_uspTimesLeft = (SCF::USHORT*)CMemory::Aligned16Reallocate(m_uspTimesLeft, sizeof(SCF::USHORT) * m_usParticleCount);

	m_pPositions  = (Float4*)CMemory::Aligned16Reallocate(m_pPositions,  sizeof(Float4) * m_usParticleCount);
	m_pVelocities = (Float4*)CMemory::Aligned16Reallocate(m_pVelocities, sizeof(Float4) * m_usParticleCount);

	m_pProperties       = (Float4*)CMemory::Aligned16Reallocate(m_pProperties,       sizeof(Float4) * m_usParticleCount);
	m_pPropertiesChange = (Float4*)CMemory::Aligned16Reallocate(m_pPropertiesChange, sizeof(Float4) * m_usParticleCount);

	this->Reset();
}

void CParticleSystem::Reset()
{
	for (SCF::USHORT i = 0; i < m_usParticleCount; i++)
	{
		CreateNewParticle(i);
	}
}

void CParticleSystem::CreateNewParticle(_IN SCF::UINT uiIndex)
{
	m_uspTimesLeft[uiIndex] = CParticleSystem::RandomUSHORT() % m_usParticleLife;

	m_pPositions[uiIndex][0] = CParticleSystem::RandomFloat1To1() * m_EmitterBox[0] + m_Position[0];
	m_pPositions[uiIndex][1] = CParticleSystem::RandomFloat1To1() * m_EmitterBox[1] + m_Position[1];
	m_pPositions[uiIndex][2] = CParticleSystem::RandomFloat1To1() * m_EmitterBox[2] + m_Position[2];
	m_pPositions[uiIndex][3] = 1;

	m_pPropertiesChange[uiIndex][0] = m_PropertiesChange[0] * (1 + (m_PropertiesVariation[0] * CParticleSystem::RandomFloat1To1()));
	m_pPropertiesChange[uiIndex][1] = m_PropertiesChange[1] * (1 + (m_PropertiesVariation[1] * CParticleSystem::RandomFloat1To1()));
	m_pPropertiesChange[uiIndex][2] = m_PropertiesChange[2] * (1 + (m_PropertiesVariation[2] * CParticleSystem::RandomFloat1To1()));
	m_pPropertiesChange[uiIndex][3] = m_PropertiesChange[3] * (1 + (m_PropertiesVariation[3] * CParticleSystem::RandomFloat1To1()));

	m_pProperties[uiIndex][0] = m_Properties[0] * (1 + (m_PropertiesVariation[0] * CParticleSystem::RandomFloat1To1()));
	m_pProperties[uiIndex][1] = m_Properties[1] * (1 + (m_PropertiesVariation[1] * CParticleSystem::RandomFloat1To1()));
	m_pProperties[uiIndex][2] = m_Properties[2] * (1 + (m_PropertiesVariation[2] * CParticleSystem::RandomFloat1To1()));
	m_pProperties[uiIndex][3] = m_Properties[3] * (1 + (m_PropertiesVariation[3] * CParticleSystem::RandomFloat1To1()));

	SetVector4(m_pVelocities[uiIndex], 0, 0, m_pProperties[uiIndex][3], 0);
	RotateVector3((Float3&)m_pVelocities[uiIndex], (Float3&)m_pVelocities[uiIndex], m_fParticleSpread * CParticleSystem::RandomFloat1To1(), m_fParticleSpread * CParticleSystem::RandomFloat1To1(), 0);
	RotateVector3((Float3&)m_pVelocities[uiIndex], (Float3&)m_pVelocities[uiIndex], m_Rotation);
}

void __fastcall CParticleSystem::BoundsUpdate()
{
	m_fBoundsRadius = (m_Properties[3] + m_Properties[3] * m_PropertiesVariation[3]) * m_usParticleLife;
}

void CParticleSystem::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
	CObjectScene::XMLSerialize(rWriter);
}

void CParticleSystem::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CObjectScene::XMLDeserialize(rReader);

	CString* pValue = NULL;

	GETVALUE { m_pMaterial = (CMaterial*)CScripting::Object(*pValue); ADDREF(*m_pMaterial); }

	//Emitter
	rReader.GetBlock();
	GETVALUE { EmitterBox(CFloat3(*pValue).Value()); }

	//Particles
	rReader.GetBlock();
	{
		GETVALUE { ParticleCount((SCF::USHORT)CInt(*pValue).Value()); }
		GETVALUE { ParticleLife ((SCF::USHORT)CInt(*pValue).Value()); }
		GETVALUE { ParticleSpread          (CFloat(*pValue).Value()); }

		//Speed
		rReader.GetBlock();
		{
			GETVALUE { ParticleSpeed         (CFloat(*pValue).Value()); }
			GETVALUE { ParticleSpeedVariation(CFloat(*pValue).Value()); }
			GETVALUE { ParticleSpeedChange   (CFloat(*pValue).Value()); }
		}

		//Size
		rReader.GetBlock();
		{
			GETVALUE { ParticleSize         (CFloat(*pValue).Value()); }
			GETVALUE { ParticleSizeVariation(CFloat(*pValue).Value()); }
			GETVALUE { ParticleSizeChange   (CFloat(*pValue).Value()); }
		}

		//Rotation
		rReader.GetBlock();
		{
			GETVALUE { ParticleRotation         (CFloat(*pValue).Value()); }
			GETVALUE { ParticleRotationVariation(CFloat(*pValue).Value()); }
			GETVALUE { ParticleRotationChange   (CFloat(*pValue).Value()); }
		}

		//Opacity
		rReader.GetBlock();
		{
			GETVALUE { ParticleOpacity         (CFloat(*pValue).Value()); }
			GETVALUE { ParticleOpacityVariation(CFloat(*pValue).Value()); }
			GETVALUE { ParticleOpacityChange   (CFloat(*pValue).Value()); }
		}
	}

	Create();
	BoundsUpdate();
}
