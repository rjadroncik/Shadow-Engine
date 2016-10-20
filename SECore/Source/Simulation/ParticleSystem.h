#pragma once

#include "../Rendering/ObjectScene.h"
#include "../Rendering/Camera.h"
#include "../Rendering/Material.h"

#include "../Simulation/SimulationObject.h"

namespace Simulation
{
	class SECORE_API CParticleSystem : public Rendering::CObjectScene, public ISimulationObject
	{
	public:
		System::Category Category() _GET { return System::Category::ParticleSystems; }

	public:
		CString XmlName() _GET { return STRING("ParticleSystem"); }
		CString ToString() _GET { return STRING("{ParticleSystem}"); }

	public:
		CParticleSystem(_IN _REF Rendering::CMaterial& rMaterial);
		virtual ~CParticleSystem();

	public:
		inline const Rendering::CMaterial& Material() _GET { return *m_pMaterial; }

	public:
		void __fastcall ApplyForce(_IN Float3& rForce, _IN _OPT Float3* pPoint = NULL);

	public:
		void __fastcall Update();
		void __fastcall Create();
		void __fastcall Reset();

		void __fastcall BoundsUpdate();

	protected:
		void __fastcall UpdatePositionsOnly();
		void __fastcall UpdatePositionsAndProperties();

	public:
		inline Float4* Positions()  _GET { return m_pPositions; }
		inline Float4* Properties() _GET { return m_pProperties; }
												
	public:
		inline USHORT ParticleCount()                                _GET { return m_usParticleCount; }
		inline void        ParticleCount(_IN USHORT usParticleCount) _SET { m_usParticleCount = usParticleCount; }

		inline USHORT ParticleLife()                               _GET { return m_usParticleLife; }
		inline void        ParticleLife(_IN USHORT usParticleLife) _SET { m_usParticleLife = usParticleLife; }

		inline float ParticleSpread()                          _GET { return m_fParticleSpread; }
		inline void  ParticleSpread(_IN float fParticleSpread) _SET { m_fParticleSpread = fParticleSpread; }

	public:
		inline float ParticleOpacity()                           _GET { return m_Properties[0]; }
		inline void  ParticleOpacity(_IN float fParticleOpacity) _SET { m_Properties[0] = fParticleOpacity; }

		inline float ParticleOpacityVariation()                                    _GET { return m_PropertiesVariation[0]; }
		inline void  ParticleOpacityVariation(_IN float fParticleOpacityVariation) _SET { m_PropertiesVariation[0] = fParticleOpacityVariation; }

		inline float ParticleOpacityChange()                                 _GET { return m_PropertiesChange[0]; }
		inline void  ParticleOpacityChange(_IN float fParticleOpacityChange) _SET { m_PropertiesChange[0] = fParticleOpacityChange; }

	public:
		inline float ParticleSize()                        _GET { return m_Properties[1]; }
		inline void  ParticleSize(_IN float fParticleSize) _SET { m_Properties[1] = fParticleSize; }

		inline float ParticleSizeVariation()                                 _GET { return m_PropertiesVariation[1]; }
		inline void  ParticleSizeVariation(_IN float fParticleSizeVariation) _SET { m_PropertiesVariation[1] = fParticleSizeVariation; }

		inline float ParticleSizeChange()                              _GET { return m_PropertiesChange[1]; }
		inline void  ParticleSizeChange(_IN float fParticleSizeChange) _SET { m_PropertiesChange[1] = fParticleSizeChange; }

	public:
		inline float ParticleRotation()                            _GET { return m_Properties[2] * SCFRadToDeg; }
		inline void  ParticleRotation(_IN float fParticleRotation) _SET { m_Properties[2] = fParticleRotation * SCFDegToRad; }

		inline float ParticleRotationVariation()                                     _GET { return m_PropertiesVariation[2]; }
		inline void  ParticleRotationVariation(_IN float fParticleRotationVariation) _SET { m_PropertiesVariation[2] = fParticleRotationVariation; }

		inline float ParticleRotationChange()                                  _GET { return m_PropertiesChange[2] * SCFRadToDeg; }
		inline void  ParticleRotationChange(_IN float fParticleRotationChange) _SET { m_PropertiesChange[2] = fParticleRotationChange * SCFDegToRad; }

	public:
		inline float ParticleSpeed()                         _GET { return m_Properties[3]; }
		inline void  ParticleSpeed(_IN float fParticleSpeed) _SET { m_Properties[3] = fParticleSpeed; }

		inline float ParticleSpeedVariation()                                  _GET { return m_PropertiesVariation[3]; }
		inline void  ParticleSpeedVariation(_IN float fParticleSpeedVariation) _SET { m_PropertiesVariation[3] = fParticleSpeedVariation; }

		inline float ParticleSpeedChange()                               _GET { return m_PropertiesChange[3] - 1; }
		inline void  ParticleSpeedChange(_IN float fParticleSpeedChange) _SET { m_PropertiesChange[3] = 1 + fParticleSpeedChange; }

	public:
		inline const Float3& EmitterBox()                       _GET { return m_EmitterBox; }
		inline void          EmitterBox(_IN Float3& emmiterBox) _SET { CopyVector3(m_EmitterBox, emmiterBox); }

	protected:
		void __fastcall CreateNewParticle(_IN UINT uiIndex);

	protected:
		static float       RandomFloat1To1();
		static USHORT RandomUSHORT();

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	protected:
		void Construct(_IN Rendering::CMaterial* pMaterial);

	protected:
		BYTE m_ucUpdateCounter;

		Float3 m_EmitterBox;
		float  m_fParticleSpread;

		USHORT m_usParticleCount;
		USHORT m_usParticleLife;
		
		Float4 m_Properties;
		Float4 m_PropertiesVariation;
		Float4 m_PropertiesChange;

	protected:
		const Rendering::CMaterial* m_pMaterial;

	protected:
		//Per-particle information
		USHORT* m_uspTimesLeft;

		Float4* m_pPositions;
		Float4* m_pVelocities;

		Float4* m_pProperties;       //m_pProperties[x][0] - opacity, m_pProperties[x][1] - size, m_pProperties[x][2] - rotation, m_pProperties[x][3] - cos(rotation)
		Float4* m_pPropertiesChange; //m_pPropertiesChange[x][3] - speed

	private:
		CParticleSystem();
	};
};
