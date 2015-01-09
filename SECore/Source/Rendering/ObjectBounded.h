#pragma once

#include "../SECore.h"
#include "../System/SEObject.h"

namespace Rendering
{
	class SECORE_API CObjectBounded : public System::CSEObject
	{
	public:
		virtual void __fastcall BoundsUpdate() = 0;
		void         __fastcall BoundsRender(_IN float fR, _IN float fG, _IN float fB) const;

	public:
		inline void  BoundsRadius(_IN float fRadius) _SET { m_fBoundsRadius = fRadius; }
		inline float BoundsRadius()                  _GET { return m_fBoundsRadius; }

	protected:
		CObjectBounded();
		virtual ~CObjectBounded();

	protected:
		float m_fBoundsRadius;

	protected:
		static Float3	s_TempVector01;
		static Float3	s_TempVector02;
	};
};
