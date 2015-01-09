#pragma once

#include "ObjectTransformable.h"

namespace Rendering
{
	class SECORE_API CObjectScene : public CObjectTransformable
	{
	public:
		virtual ~CObjectScene();

	public:
		bool __fastcall BoundsOverlap    (_IN CObjectScene& rObject) _GET;
		bool __fastcall BoundsBehindPlane(_IN Float4& rPlane) _GET;
		bool __fastcall BoundsBehindPlane(_IN Float4& rPlane, _IN float fTolerance) _GET;

	public:
		inline void Changed(_IN bool bChanged) _SET { m_bChanged = bChanged; }
		inline bool Changed()                  _GET { return m_bChanged; }

		inline void Dynamic(_IN bool bDynamic) _SET { m_bDynamic = bDynamic; }
		inline bool Dynamic()                  _GET { return m_bDynamic; }

	public:
		inline float DistanceToCamera()                    _GET { return m_fDistanceToCamera; }
		inline void  DistanceToCamera(_IN float fDistance) _SET { m_fDistanceToCamera = fDistance; }

	public:
		inline float DistanceToLight()                    _GET { return m_fDistanceToLight; }
		inline void  DistanceToLight(_IN float fDistance) _SET { m_fDistanceToLight = fDistance; }

	public:
		inline void VisibleToCamera(_IN bool bVisible) _SET { m_bVisibleToCamera = bVisible; }
		inline bool VisibleToCamera()                  _GET { return m_bVisibleToCamera; }

	public:
		inline void VisibleToLight(_IN bool bVisible) _SET { m_bVisibleToLight = bVisible; }
		inline bool VisibleToLight()                  _GET { return m_bVisibleToLight; }

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	protected:
		bool m_bDynamic;
		bool m_bChanged;

	protected:
		bool  m_bVisibleToCamera;
		bool  m_bVisibleToLight;

		float m_fDistanceToCamera;
		float m_fDistanceToLight;

	protected:
		static const Float3 s_Forward;
		static const Float3 s_Up;
		static const Float3 s_Right;

	protected:
		CObjectScene();
	};
};
