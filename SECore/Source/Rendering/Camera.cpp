#include "Camera.h"

#include "Material.h"

#include <xmmintrin.h>
#include <intrin.h>

using namespace Rendering;
using namespace System;

CCamera::CCamera()
{
	ZeroVector4(m_Viewport);
	ZeroVector4(m_Scissor);

	m_fFOV		= 45.0;
	m_fAspect	= 1.3333333333333f;

	m_fNear		= 10.0;
	m_fFar		= 14000.0;

	m_pTargetObject = NULL;
}

CCamera::~CCamera()
{
}

void __fastcall CCamera::MoveForward(_IN float Distance)
{
	RotateVector3(s_TempVector01, s_Forward, m_Rotation);
	ScaleVector3 (s_TempVector01, s_TempVector01, Distance);

	AddVectors3((Float3&)m_Position, (Float3&)m_Position, s_TempVector01);
}

void __fastcall CCamera::MoveRight(_IN float Distance)
{
	RotateVector3(s_TempVector01, s_Right, m_Rotation);
	ScaleVector3 (s_TempVector01, s_TempVector01, Distance);

	AddVectors3((Float3&)m_Position, (Float3&)m_Position, s_TempVector01);
}

void __fastcall CCamera::MoveUp(_IN float Distance)
{
	RotateVector3(s_TempVector01, s_Up, m_Rotation);
	ScaleVector3 (s_TempVector01, s_TempVector01, Distance);

	AddVectors3((Float3&)m_Position, (Float3&)m_Position, s_TempVector01);
}

void __fastcall CCamera::Viewport(_IN int iX, _IN int iY, _IN int iWidth, _IN int iHeight) _SET
{
	m_Viewport[0] = iX;
	m_Viewport[1] = iY;

	m_Viewport[2] = iWidth;
	m_Viewport[3] = iHeight;

	Viewport(m_Viewport);
}

void __fastcall CCamera::Viewport(_IN Int4& rViewport) _SET
{
	CopyVector4(m_Viewport, rViewport);

	if (!m_Viewport[3]) { m_Viewport[3] = 1; }
	m_fAspect = (float)m_Viewport[2] / (float)m_Viewport[3];
}

bool __fastcall CCamera::CheckVisibilityByBounds(_IN CObjectScene& rObject) _GET
{
	if (!this->BoundsOverlap(rObject)) { return FALSE; }

	if (rObject.BoundsBehindPlane(m_Planes[FP_FRONT]))  { return FALSE; }
	if (rObject.BoundsBehindPlane(m_Planes[FP_LEFT]))   { return FALSE; }
	if (rObject.BoundsBehindPlane(m_Planes[FP_RIGHT]))  { return FALSE; }
	if (rObject.BoundsBehindPlane(m_Planes[FP_BOTTOM])) { return FALSE; }
	if (rObject.BoundsBehindPlane(m_Planes[FP_TOP]))    { return FALSE; }
	if (rObject.BoundsBehindPlane(m_Planes[FP_BACK], rObject.BoundsRadius())) { return FALSE; }

	return TRUE;
}

void __fastcall CCamera::BoundsUpdate()
{
	m_fBoundsRadius = m_fFar;
}

void __fastcall CCamera::Render()
{
}
 
void __fastcall CCamera::ViewVolumeUpdate()
{
	//Update bounding volume
	BoundsUpdate();

	//Prepare rotated direction vectors
	RotateVector3(m_Forward, s_Forward, m_Rotation);
	RotateVector3(m_Up,      s_Up,      m_Rotation);
	RotateVector3(m_Right,   s_Right,   m_Rotation);
	
	//Front
	MakeVector3(s_TempVector01, m_Forward, m_fNear);
	AddVectors3(s_TempVector01, s_TempVector01, (Float3&)m_Position);

	CopyVector3((Float3&)m_Planes[FP_FRONT], m_Forward);
	m_Planes[FP_FRONT][3] = -MultiplyVectors3(s_TempVector01, (Float3&)m_Planes[FP_FRONT]);

	//Back
	MakeVector3(s_TempVector01, m_Forward, m_fFar);
	AddVectors3(s_TempVector01, s_TempVector01, (Float3&)m_Position);

	InvertVector3((Float3&)m_Planes[FP_BACK], m_Forward);
	m_Planes[FP_BACK][3] = -MultiplyVectors3(s_TempVector01, (Float3&)m_Planes[FP_BACK]);

	//Left
	RotateVector3((Float3&)m_Planes[FP_LEFT], s_Right, 0 , m_fFOV, 0);
	RotateVector3((Float3&)m_Planes[FP_LEFT], (Float3&)m_Planes[FP_LEFT], m_Rotation);
	m_Planes[FP_LEFT][3] = -MultiplyVectors3((Float3&)m_Position, (Float3&)m_Planes[FP_LEFT]);
	
	//Right
	InvertVector3((Float3&)m_Planes[FP_RIGHT], s_Right);
	RotateVector3((Float3&)m_Planes[FP_RIGHT], (Float3&)m_Planes[FP_RIGHT], 0 , -m_fFOV, 0);
	RotateVector3((Float3&)m_Planes[FP_RIGHT], (Float3&)m_Planes[FP_RIGHT], m_Rotation);
	m_Planes[FP_RIGHT][3] = -MultiplyVectors3((Float3&)m_Position, (Float3&)m_Planes[FP_RIGHT]);

	//Bottom
	RotateVector3((Float3&)m_Planes[FP_BOTTOM], s_Up, -m_fFOV / m_fAspect, 0, 0);
	RotateVector3((Float3&)m_Planes[FP_BOTTOM], (Float3&)m_Planes[FP_BOTTOM], m_Rotation);
	m_Planes[FP_BOTTOM][3] = -MultiplyVectors3((Float3&)m_Position, (Float3&)m_Planes[FP_BOTTOM]);

	//Top
	InvertVector3((Float3&)m_Planes[FP_TOP], s_Up);
	RotateVector3((Float3&)m_Planes[FP_TOP], (Float3&)m_Planes[FP_TOP], m_fFOV / m_fAspect, 0, 0);
	RotateVector3((Float3&)m_Planes[FP_TOP], (Float3&)m_Planes[FP_TOP], m_Rotation);
	m_Planes[FP_TOP][3] = -MultiplyVectors3((Float3&)m_Position, (Float3&)m_Planes[FP_TOP]);
}

void __fastcall CCamera::LookAt(_IN Float3& rPoint)
{
	Float3 Rotation  = { 0, 0, 0 };
	Float3 Direction = { 0, 0, 0 };

	MakeVector3(Direction, rPoint, *(Float3*)m_Position);
	NormalizeVector3(Direction, Direction);

	DirectionToRotation3N(Rotation, Direction);
	this->Rotation(Rotation[0], Rotation[1], m_Rotation[2]);
}

bool __fastcall CCamera::IsLookingAt(_IN CObjectScene& rObject) const
{
	register float fDistance = PointLineDistance(*(Float3*)rObject.Position(), *(Float3*)m_Position, m_Forward);

	if (fDistance < rObject.BoundsRadius()) { return TRUE; }
	else                                    { return FALSE; }
}

void __fastcall CCamera::ApplyProjection2D() const
{
	//Prepare renderer
	glViewport(m_Viewport[0], m_Viewport[1], m_Viewport[2], m_Viewport[3]);

	glMatrixMode(GL_PROJECTION);			
	glLoadIdentity();		

	gluOrtho2D(0, m_Viewport[2], 0, m_Viewport[3]);
	glTranslatef(0, (float)m_Viewport[3], 0);
	glScalef(1, -1, 0);
//	glTranslatef(0.5f, 0.5f, 0);

	glMatrixMode(GL_MODELVIEW);			
	glLoadIdentity();

	glDisable(GL_CLIP_PLANE0);
}

void __fastcall CCamera::Update()
{
	if (m_pTargetObject && m_pTargetObject->Changed()) { LookAt(*(Float3*)m_pTargetObject->Position()); }

	if (this->Changed()) { this->ViewVolumeUpdate(); }
}

void __fastcall CCamera::ApplyProjection3D() const
{
	//Prepare renderer
	glViewport(m_Viewport[0], m_Viewport[1], m_Viewport[2], m_Viewport[3]);

	glMatrixMode(GL_PROJECTION);	 
	glLoadIdentity();

	gluPerspective(m_fFOV * 2 / m_fAspect, m_fAspect, m_fNear, m_fFar);

	glRotated(-m_Rotation[0], 1.0, 0.0, 0.0); 
	glRotated(-m_Rotation[1], 0.0, 1.0, 0.0); 
	glRotated(-m_Rotation[2], 0.0, 0.0, 1.0); 

	glTranslated(-m_Position[0], -m_Position[1], -m_Position[2]); 

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	double Plane[4];
	Plane[0] = m_Planes[FP_BACK][0];
	Plane[1] = m_Planes[FP_BACK][1];
	Plane[2] = m_Planes[FP_BACK][2];
	Plane[3] = m_Planes[FP_BACK][3];

	glClipPlane(GL_CLIP_PLANE0, Plane);
	glEnable(GL_CLIP_PLANE0);
}

void CCamera::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
	CObjectScene::XMLSerialize(rWriter);

	PUTVALUENEW("fov",  FOV(),  CFloat);
	PUTVALUENEW("near", Near(), CFloat);
	PUTVALUENEW("far",  Far(),  CFloat);
}

void CCamera::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CObjectScene::XMLDeserialize(rReader);

	CString* pValue = NULL;

	GETVALUE { FOV (CFloat(*pValue).Value()); }
	GETVALUE { Near(CFloat(*pValue).Value()); }
	GETVALUE { Far (CFloat(*pValue).Value()); }
}
