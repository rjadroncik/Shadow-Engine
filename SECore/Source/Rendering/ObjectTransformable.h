#pragma once

#include "ObjectBounded.h"

namespace Rendering
{
	class SECORE_API CObjectTransformable : public CObjectBounded
	{
	public:
		inline void Position      (_IN float fX, _IN float fY = 1, _IN float fZ = 1, _IN float w = 1)	_SET { m_Position[0] =  fX;	m_Position[1] =  fY; m_Position[2] =  fZ; m_Position[3] = w; }
		inline void PositionOffset(_IN float fX, _IN float fY,     _IN float fZ)						_SET { m_Position[0] += fX;	m_Position[1] += fY; m_Position[2] += fZ; }

		inline void Rotation      (_IN float fX, _IN float fY, _IN float fZ)							_SET { m_Rotation[0] =  fX;	m_Rotation[1] =  fY; m_Rotation[2] =  fZ; }
		inline void RotationOffset(_IN float fX, _IN float fY, _IN float fZ)							_SET { m_Rotation[0] += fX;	m_Rotation[1] += fY; m_Rotation[2] += fZ; }

		inline void	Position      (_IN Float3& Position)       _SET { CopyVector3((Float3&)m_Position, Position); m_Position[3] = 1;  }
		inline void	Position      (_IN Float4& Position)       _SET { CopyVector4(m_Position, Position); }
		inline void	PositionOffset(_IN Float3& PositionOffset) _SET { AddVectors3((Float3&)m_Position, (Float3&)m_Position, PositionOffset); }

		inline void	Rotation      (_IN Float3& Rotation)		 _SET { CopyVector3(m_Rotation, Rotation); }
		inline void	RotationOffset(_IN Float3& RotationOffset) _SET { AddVectors3(m_Rotation, m_Rotation, RotationOffset); }

		inline const Float4& Position() _GET { return m_Position; }
		inline const Float3& Rotation() _GET { return m_Rotation; }

	public:
		virtual void __fastcall OrbitAroundPoint(_IN float fX, _IN float fY, _IN float fZ, _IN float fRefX, _IN float fRefY, _IN float fRefZ);
		virtual void __fastcall OrbitAroundPoint(_IN Float3& rPoint, _IN Float3& rRotation);

	public:
		inline void RotationApply()  const { glRotated(m_Rotation[2], 0.0, 0.0, 1.0); glRotated(m_Rotation[1], 0.0, 1.0, 0.0); glRotated(m_Rotation[0], 1.0, 0.0, 0.0); } 
		inline void PosittionApply() const { glTranslated(m_Position[0], m_Position[1], m_Position[2]); } 
		
		inline void TransformationApply()        const { glTranslated(m_Position[0], m_Position[1], m_Position[2]); glRotated(m_Rotation[2], 0.0, 0.0, 1.0); glRotated(m_Rotation[1], 0.0, 1.0, 0.0); glRotated(m_Rotation[0], 1.0, 0.0, 0.0); } 
		inline void TransformationInverseApply() const { glRotated(-m_Rotation[0], 1.0, 0.0, 0.0); glRotated(-m_Rotation[1], 0.0, 1.0, 0.0); glRotated(-m_Rotation[2], 0.0, 0.0, 1.0); glTranslated(-m_Position[0], -m_Position[1], -m_Position[2]); } 

	public:
		void __fastcall InverseTransform(_OUT Float4& rResult, _IN Float4& rVector) const;

	protected:
		CObjectTransformable();
		virtual ~CObjectTransformable();

	protected:
		Float4 m_Position;
		Float3 m_Rotation;
	};
};
