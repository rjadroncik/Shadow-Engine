#pragma once
#include "Light.h"

namespace Rendering
{
	class SECORE_API CLightOmni : public CLight
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassLightOmni; }
		CString   ToString() _GET { return STRING("{LightOmni}"); }

	public:
		CLightOmni();
		virtual ~CLightOmni();

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
	};
};