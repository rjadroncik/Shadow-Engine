#pragma once
#include "Light.h"

namespace Rendering
{
	class SECORE_API CLightSpot : public CLight
	{
	public:
		CString XmlName() _GET { return STRING("LightSpot"); }
		CString ToString() _GET { return STRING("{LightSpot}"); }

	public:
		CLightSpot();
		virtual ~CLightSpot();

	public:
		inline void          TargetObject(_IN _REF CObjectScene* pObject) _SET { m_pTargetObject = (CObjectScene*)pObject; }
		inline CObjectScene* TargetObject()                               _GET { return m_pTargetObject; }

	public:
		void __fastcall LookAt(_IN Float3& rPoint) _SET;
		bool __fastcall IsLookingAt(_IN CObjectScene& rObject) _GET;

	public:
		void __fastcall BoundsUpdate();
		void __fastcall Update(); 

		void __fastcall Target      (_IN float fX, _IN float fY, _IN float fZ) _SET;
		void __fastcall TargetOffset(_IN float fX, _IN float fY, _IN float fZ) _SET;

		bool __fastcall CheckVisibilityByBounds(_IN CObjectScene& rObject) _GET;
		bool __fastcall CheckVisibilityByBounds(_IN Float3& rPosition, _IN float fRadius) _GET;

	public:
		void __fastcall ApplyProjection3D() const;
		void __fastcall ApplyProjectionShadow() const;
		void __fastcall ViewVolumeUpdate();

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	protected:
		CObjectScene* m_pTargetObject;
	};
};