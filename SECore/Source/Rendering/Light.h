#pragma once
#include "ObjectScene.h"
#include "../Resources/Texture.h"

namespace Rendering
{
	enum FrustumPlanes
	{
		FP_FRONT = 0,
		FP_BACK,
		FP_LEFT,
		FP_RIGHT,
		FP_BOTTOM,
		FP_TOP,
	};

	class SECORE_API CLight : public CObjectScene
	{
	public:
		CLight();
		virtual ~CLight();

	public:
		inline void Shadows(_IN bool bCastShadows) _SET { m_bCastShadows = bCastShadows; }
		inline bool Shadows()                      _GET { return m_bCastShadows; }

		inline void ShadowsUpdate(_IN bool bShadowsUpdate) _SET { m_bShadowsUpdate = bShadowsUpdate; }
		inline bool ShadowsUpdate()                        _GET { return m_bShadowsUpdate; }
		
		inline void Enabled(_IN bool bEnabled) _SET { m_bEnabled = bEnabled; }
		inline bool Enabled()                  _GET { return m_bEnabled; }

	public:
		inline void  Cutoff(_IN float fCutoff) _SET { m_fCutoff = fCutoff; }
		inline float Cutoff()                  _GET { return m_fCutoff; }
		
		inline void  Range(_IN float fRange) _SET { m_fRange = fRange; m_fScissorRange = m_fRange * m_fScissorFactor; BoundsUpdate(); }
		inline float Range()                 _GET { return m_fRange; }
		inline float RangeScissor()          _GET { return m_fScissorRange; }

	public:
		inline void	         Tint      (_IN float fR, _IN float fG, _IN float fB) _SET { m_Tint[0] = fR; m_Tint[1] = fG; m_Tint[2] = fB; }
		inline void          Tint      (_IN Float3& rTint)                        _SET { CopyVector3(m_Tint, rTint); }	
		inline void          TintOffset(_IN Float3& rTint)                        _SET { AddVectors3(m_Tint, m_Tint, rTint); }	
		inline const Float3& Tint()                                               _GET { return m_Tint; }

		inline void	Ambient (_IN float fValue) _SET { m_fAmbient  = fValue; }
		inline void	Diffuse (_IN float fValue) _SET { m_fDiffuse  = fValue; }
		inline void	Specular(_IN float fValue) _SET { m_fSpecular = fValue; }

		inline void	AmbientOffset (_IN float fOffset) _SET { m_fAmbient  += fOffset; }
		inline void	DiffuseOffset (_IN float fOffset) _SET { m_fDiffuse  += fOffset; }
		inline void	SpecularOffset(_IN float fOffset) _SET { m_fSpecular += fOffset; }

		inline float Ambient()  _GET { return m_fAmbient; }
		inline float Diffuse()  _GET { return m_fDiffuse; }
		inline float Specular() _GET { return m_fSpecular; }

	public:
		int  ShadowMapLOD()                      _GET{ return m_iShadowMapLOD; }
		void ShadowMapLOD(_IN int iShadowMapLOD) _SET{ m_iShadowMapLOD = iShadowMapLOD; }

	public:
		void __fastcall Use() const;

	public:
		virtual void __fastcall Update() = 0; 
		virtual void __fastcall BoundsUpdate() = 0;

		virtual bool __fastcall CheckVisibilityByBounds(_IN CObjectScene& rObject) _GET = 0;
		virtual bool __fastcall CheckVisibilityByBounds(_IN Float3& rPosition, _IN float fRadius) _GET = 0;

	public:
		virtual void __fastcall ApplyProjection3D()     const = 0;
		virtual void __fastcall ApplyProjectionShadow() const = 0;

	private:
		virtual void __fastcall ViewVolumeUpdate() = 0;

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	protected:
		float m_fRange;
		float m_fCutoff;

		float m_fScissorRange;
		float m_fScissorFactor;

	protected:
		Float3 m_Tint;

		float m_fAmbient;
		float m_fDiffuse;
		float m_fSpecular;

	protected:
		int m_iShadowMapLOD;

		bool m_bCastShadows;
		bool m_bEnabled;
		bool m_bShadowsUpdate;

	protected:
		Float4 m_Planes[6];

	protected:
		Float3 m_Forward;
		Float3 m_Up;
		Float3 m_Right;
	};
};