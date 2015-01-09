#include "ObjectTransformable.h"

using namespace Rendering;

CObjectTransformable::CObjectTransformable()
{
	ZeroVector4(m_Position);
	ZeroVector3(m_Rotation);
}

CObjectTransformable::~CObjectTransformable()
{
}

void __fastcall CObjectTransformable::OrbitAroundPoint(_IN float fX, _IN float fY, _IN float fZ, _IN float fRefX, float _IN fRefY, float _IN fRefZ)
{
	RotationOffset(fRefX, fRefY, fRefZ);

	Float3 OrbitVector;

	OrbitVector[0] = m_Position[0] - fX;
	OrbitVector[1] = m_Position[1] - fY;
	OrbitVector[2] = m_Position[2] - fZ;

	RotateVector3(OrbitVector, OrbitVector, fRefX, fRefY, fRefZ);

	m_Position[0] = fX + OrbitVector[0];
	m_Position[1] = fY + OrbitVector[1];
	m_Position[2] = fZ + OrbitVector[2];
}

void __fastcall CObjectTransformable::OrbitAroundPoint(_IN Float3& rPoint, _IN Float3& rRotation)
{
	RotationOffset(rRotation);

	Float3 OrbitVector;

	SubtractVectors3(OrbitVector, (Float3&)m_Position,  rPoint);
	RotateVector3   (OrbitVector,            OrbitVector, rRotation);
	AddVectors3     ((Float3&)m_Position,  rPoint,      OrbitVector);
}

void __fastcall CObjectTransformable::InverseTransform(_OUT Float4& rResult, _IN Float4& rVector) const
{
	SubtractVectors3((Float3&)rResult, (Float3&)rVector, (Float3&)m_Position);
	RotateVector3   ((Float3&)rResult, (Float3&)rResult, 0, 0, -m_Rotation[2]);
	RotateVector3   ((Float3&)rResult, (Float3&)rResult, 0, -m_Rotation[1], 0);
	RotateVector3   ((Float3&)rResult, (Float3&)rResult, -m_Rotation[0], 0, 0);

	rResult[3] = 1;
}