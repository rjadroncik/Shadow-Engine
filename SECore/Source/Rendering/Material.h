#pragma once

#include "Light.h"
#include "MaterialUsage.h"

namespace Rendering
{
	class SECORE_API CMaterial : public System::CSEObject
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassMaterial; }
		CString   ToString() _GET { return STRING("{Material}"); }

	public:
		CMaterial();
		virtual ~CMaterial();
	 
	public:
		const CMaterialUsage* __fastcall Usage(_IN CLight& rLight)   _GET;
		const CMaterialUsage* __fastcall Usage(_IN SCF::ENUM eUsage) _GET;
		void                  __fastcall Usage(_IN SCF::ENUM eUsage, _IN CMaterialUsage* pUsage) _SET;

	public:
		//Basic properties
		inline void	         Tint      (_IN float fR, _IN float fG, _IN float fB) _SET { m_Tint[0] = fR; m_Tint[1] = fG; m_Tint[2] = fB; }
		inline void          Tint      (_IN Float3& rTint)                        _SET { CopyVector3(m_Tint, rTint); }	
		inline void          TintOffset(_IN Float3& rTint)                        _SET { AddVectors3(m_Tint, m_Tint, rTint); }	
		inline const Float3& Tint()                                               _GET { return m_Tint; }

		inline void Ambient  (_IN float fValue) _SET { m_fAmbient   = fValue; }
		inline void Diffuse  (_IN float fValue) _SET { m_fDiffuse   = fValue; }
		inline void Specular (_IN float fValue) _SET { m_fSpecular  = fValue; }
		inline void Emission (_IN float fValue) _SET { m_fEmission  = fValue; }
		inline void Shininess(_IN float fValue) _SET { m_fShininess = fValue; }

		inline void AmbientOffset  (_IN float fOffset) _SET { m_fAmbient   += fOffset; }
		inline void DiffuseOffset  (_IN float fOffset) _SET { m_fDiffuse   += fOffset; }
		inline void SpecularOffset (_IN float fOffset) _SET { m_fSpecular  += fOffset; }
		inline void EmissionOffset (_IN float fOffset) _SET { m_fEmission  += fOffset; }
		inline void ShininessOffset(_IN float fOffset) _SET { m_fShininess += fOffset; }

		inline float Ambient()   _GET { return m_fAmbient; }
		inline float Diffuse()   _GET { return m_fDiffuse; }
		inline float Specular()  _GET { return m_fSpecular; }
		inline float Emission()  _GET { return m_fEmission; }
		inline float Shininess() _GET { return m_fShininess; }

	public:
		inline void Transparent(_IN bool bValue) _SET { m_bTransparent = bValue; }
		inline bool Transparent()                _GET { return m_bTransparent; }

		inline void      FactorSource(_IN SCF::ENUM eValue) _SET { m_eFactorSource = eValue; }
		inline SCF::ENUM FactorSource()                     _GET { return m_eFactorSource; }
	
		inline void      FactorDestination(_IN SCF::ENUM eValue) _SET { m_eFactorDestination = eValue; }
		inline SCF::ENUM FactorDestination()                     _GET { return m_eFactorDestination; }

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	protected:
		//Basic material properties
		Float3 m_Tint;

		float m_fAmbient;
		float m_fDiffuse;
		float m_fSpecular;

		float m_fEmission;
		float m_fShininess;

	protected:
		const CMaterialUsage* m_paUsages[UsageCount];

	protected:
		bool m_bTransparent;

		SCF::ENUM m_eFactorSource;
		SCF::ENUM m_eFactorDestination;
	};
};
