#pragma once

#include "../SECore.h"
#include "../System/SEObject.h"

namespace Animation
{
	class SECORE_API CPath : public System::CSEObject
	{
	public:
		CPath();
		virtual ~CPath();

		Float3& PointAt(_IN float fRatio);

	protected:

		static Float3 s_Point;
	};
};
