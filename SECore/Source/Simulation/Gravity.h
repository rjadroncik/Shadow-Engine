#pragma once
#include "../Rendering/ObjectScene.h"	

namespace Simulation
{
	class SECORE_API CGravity : public Rendering::CObjectScene
	{
	public:
		System::Category Category() _GET { return System::Category::Forces; }

	public:
		CString XmlName() _GET { return STRING("Gravity"); }
		CString ToString() _GET { return STRING("{Gravity}"); }

	public:
		CGravity();
		virtual ~CGravity();

	    void ObjectsAffectedAdd   (_IN Rendering::CObjectScene& rObject);
		
		void ObjectsAffectedRemove(_IN Rendering::CObjectScene& rObject);
		void ObjectsAffectedRemove(_IN UINT uiIndex);

		UINT ObjectsAffectedCount() { return m_uiObjectsAffectedCount; }

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
		UINT                     m_uiObjectsAffectedCount;
	};
};