#pragma once
#include "../Rendering/ObjectScene.h"	

namespace Simulation
{
	class SECORE_API CGravity : public Rendering::CObjectScene
	{
	public:
		CGravity();
		virtual ~CGravity();

		void ObjectsAffectedAdd   (_IN Rendering::CObjectScene& rObject);
		
		void ObjectsAffectedRemove(_IN Rendering::CObjectScene& rObject);
		void ObjectsAffectedRemove(_IN SCF::UINT uiIndex);

		SCF::UINT ObjectsAffectedCount() { return m_uiObjectsAffectedCount; }

		void ObjectsAffectedUpdate();

	protected:

		//Strength corresponds to: G * m 
		//Where: G - gravitation constant
		//       m - mass of the object causing the gravitation field
		float m_fStrength;

		union SProperties
		{
			struct SRadial
			{
				Float3 m_Position;
			};

			struct SPlanal
			{
				Float4 m_Plane;
			};
		
		} m_Properties;

		Rendering::CObjectScene* m_pObjectsAffected;
		SCF::UINT                     m_uiObjectsAffectedCount;
	};
};