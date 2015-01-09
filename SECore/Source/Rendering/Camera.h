#pragma once
#include "ObjectScene.h"

#include "Light.h"

namespace Rendering
{
#define LOD_LEVELS 3

	class SECORE_API CScene;
	class SECORE_API CCamera : public CObjectScene
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassCamera; }
		CString   ToString() _GET { return STRING("{Camera}"); }

	public:
		CCamera();
		virtual ~CCamera();

	public:
		inline void  FOV(_IN float fFOV)       _SET { m_fFOV = fFOV; }
		inline float FOV()                     _GET { return m_fFOV; }

		inline void  Near(_IN float fNear)     _SET { m_fNear = fNear; }
		inline float Near()                    _GET { return m_fNear; }

		inline void  Far(_IN float fFar)       _SET { m_fFar = fFar; }
		inline float Far()                     _GET { return m_fFar; }

		inline void  FarClip(_IN bool bClip)   _SET { if (bClip) { glEnable(GL_CLIP_PLANE0); } else { glDisable(GL_CLIP_PLANE0); } }
		inline bool  FarClip()                 _GET { return (glIsEnabled(GL_CLIP_PLANE0) == 1); }

	public:
		inline const Int4& Viewport()                                                        _GET { return m_Viewport; }
		void __fastcall    Viewport(_IN Int4& rViewport)                                     _SET;
		void __fastcall    Viewport(_IN int iX, _IN int iY, _IN int iWidth, _IN int iHeight) _SET;

		inline float         Aspect()                  _GET { return m_fAspect; }
		inline const Float4& FrustumPlane(SCF::BYTE i) _GET { return m_Planes[i]; }
		inline const Int4&   Scissor()                 _GET { return m_Scissor; }

	public:
		inline void          TargetObject(_IN _REF CObjectScene* pObject) _SET { m_pTargetObject = (CObjectScene*)pObject; }
		inline CObjectScene* TargetObject()                               _GET { return m_pTargetObject; }

	public:
		void __fastcall LookAt(_IN Float3& rPoint) _SET;
		bool __fastcall IsLookingAt(_IN CObjectScene& rObject) _GET;

	public:
		bool __fastcall CheckVisibilityByBounds(_IN CObjectScene& rObject) _GET;

	public:
		void __fastcall MoveRight  (_IN float Distance);
		void __fastcall MoveUp     (_IN float Distance);
		void __fastcall MoveForward(_IN float Distance);
	 
	public:
		inline const Float3& VectorForward() _GET { return m_Forward; }
		inline const Float3& VectorUp()      _GET { return m_Up; }
		inline const Float3& VectorRight()   _GET { return m_Right; }

	public:
		void __fastcall ApplyProjection3D() const;
		void __fastcall ApplyProjection2D() const;

	public:
		void __fastcall Update(); 
		void __fastcall BoundsUpdate();
		void __fastcall Render();

	private:
		void __fastcall ViewVolumeUpdate();

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	private:
		Float4 m_Planes[6];

	private:
		Float3 m_Forward;
		Float3 m_Up;
		Float3 m_Right;

	private:
		Int4 m_Viewport;
		Int4 m_Scissor;

	private:
		float m_fFOV;
		float m_fAspect;

		float m_fNear;
		float m_fFar;

	private:
		CObjectScene* m_pTargetObject;

	private:
		float m_faLODDistances[LOD_LEVELS];
	};
};