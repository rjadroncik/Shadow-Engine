#include "LightSpot.h"
#include "RendererDGC.h"

using namespace Rendering;

CLightSpot::CLightSpot()
{
	m_pTargetObject = NULL;

	m_Position[3] = 1.0;

	m_fRange  = 10000.0f;
	m_fCutoff = 60.0f;

	m_fScissorFactor = 2.0f;
	m_fScissorRange  = 0.0f;
}

CLightSpot::~CLightSpot()
{
}

void __fastcall CLightSpot::LookAt(_IN Float3& rPoint)
{
	Float3 Rotation  = { 0, 0, 0 };
	Float3 Direction = { 0, 0, 0 };

	MakeVector3(Direction, rPoint, *(Float3*)m_Position);

	NormalizeVector3(Direction, Direction);

	DirectionToRotation3N(Rotation, Direction);

	this->Rotation(Rotation[0], Rotation[1], m_Rotation[2]);
}

bool __fastcall CLightSpot::IsLookingAt(_IN CObjectScene& rObject) const
{
	register float fDistance = PointLineDistance(*(Float3*)rObject.Position(), *(Float3*)m_Position, m_Forward);

	if (fDistance < rObject.BoundsRadius()) { return TRUE; }

	return FALSE;
}

bool __fastcall CLightSpot::CheckVisibilityByBounds(_IN CObjectScene& rObject) _GET
{
	if (MeasureDistance3((Float3&)rObject.Position(), (Float3&)m_Position) < (m_fRange + rObject.BoundsRadius())) { return TRUE; }
	return FALSE;
}

bool __fastcall CLightSpot::CheckVisibilityByBounds(_IN Float3& rPosition, _IN float fRadius) _GET
{
	if (MeasureDistance3(rPosition, (Float3&)m_Position) < (m_fRange + fRadius)) { return TRUE; }
	return FALSE;
};


void __fastcall CLightSpot::BoundsUpdate()
{
	m_fBoundsRadius	= m_fRange;
}

void __fastcall CLightSpot::ApplyProjectionShadow() const
{
	glMatrixMode(GL_MODELVIEW);	 
	glLoadIdentity();

	//Bias & scale
	glTranslatef(0.5f, 0.5f, 0.5f);
	glScalef    (0.5f, 0.5f, 0.5f);

	//Perspective
	gluPerspective(m_fCutoff * 2, 1, m_fRange * 0.001f, m_fRange);

	//View
	glRotated(-m_Rotation[0], 1.0, 0.0, 0.0); 
	glRotated(-m_Rotation[1], 0.0, 1.0, 0.0); 
	glRotated(-m_Rotation[2], 0.0, 0.0, 1.0); 

	glTranslated(-m_Position[0], -m_Position[1], -m_Position[2]); 
}

void __fastcall CLightSpot::ApplyProjection3D() const
{
	//Prepare renderer
	glViewport(0, 0, CRendererDGC::ShadowMapFront(m_iShadowMapLOD)->Size()[1], CRendererDGC::ShadowMapFront(m_iShadowMapLOD)->Size()[1]);

	glMatrixMode(GL_PROJECTION);	 
	glLoadIdentity();

	gluPerspective(m_fCutoff * 2, 1, m_fRange * 0.001f, m_fRange);

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

void __fastcall CLightSpot::Update()
{
	if (m_pTargetObject) { LookAt(*(Float3*)m_pTargetObject->Position()); }

	if (this->Changed()) { this->ViewVolumeUpdate(); }
}

void __fastcall CLightSpot::ViewVolumeUpdate()
{
	//Update bounding volume
	BoundsUpdate();

	//Prepare rotated direction vectors
	RotateVector3(m_Forward, s_Forward, m_Rotation);
	RotateVector3(m_Up,      s_Up,      m_Rotation);
	RotateVector3(m_Right,   s_Right,   m_Rotation);

	//Front
	MakeVector3(s_TempVector01, m_Forward, m_fRange * 0.001f);
	AddVectors3(s_TempVector01, s_TempVector01, (Float3&)m_Position);

	CopyVector3((Float3&)m_Planes[FP_FRONT], m_Forward);
	m_Planes[FP_FRONT][3] = -MultiplyVectors3(s_TempVector01, (Float3&)m_Planes[FP_FRONT]);

	//Back
	MakeVector3(s_TempVector01, m_Forward, m_fRange);
	AddVectors3(s_TempVector01, s_TempVector01, (Float3&)m_Position);

	InvertVector3((Float3&)m_Planes[FP_BACK], m_Forward);
	m_Planes[FP_BACK][3] = -MultiplyVectors3(s_TempVector01, (Float3&)m_Planes[FP_BACK]);

	//Left
	RotateVector3((Float3&)m_Planes[FP_LEFT], s_Right, 0 , m_fCutoff, 0);
	RotateVector3((Float3&)m_Planes[FP_LEFT], (Float3&)m_Planes[FP_LEFT], m_Rotation);
	m_Planes[FP_LEFT][3] = -MultiplyVectors3((Float3&)m_Position, (Float3&)m_Planes[FP_LEFT]);

	//Right
	InvertVector3((Float3&)m_Planes[FP_RIGHT], s_Right);
	RotateVector3((Float3&)m_Planes[FP_RIGHT], (Float3&)m_Planes[FP_RIGHT], 0 , -m_fCutoff, 0);
	RotateVector3((Float3&)m_Planes[FP_RIGHT], (Float3&)m_Planes[FP_RIGHT], m_Rotation);
	m_Planes[FP_RIGHT][3] = -MultiplyVectors3((Float3&)m_Position, (Float3&)m_Planes[FP_RIGHT]);

	//Bottom
	RotateVector3((Float3&)m_Planes[FP_BOTTOM], s_Up, -m_fCutoff, 0, 0);
	RotateVector3((Float3&)m_Planes[FP_BOTTOM], (Float3&)m_Planes[FP_BOTTOM], m_Rotation);
	m_Planes[FP_BOTTOM][3] = -MultiplyVectors3((Float3&)m_Position, (Float3&)m_Planes[FP_BOTTOM]);

	//Top
	InvertVector3((Float3&)m_Planes[FP_TOP], s_Up);
	RotateVector3((Float3&)m_Planes[FP_TOP], (Float3&)m_Planes[FP_TOP], m_fCutoff, 0, 0);
	RotateVector3((Float3&)m_Planes[FP_TOP], (Float3&)m_Planes[FP_TOP], m_Rotation);
	m_Planes[FP_TOP][3] = -MultiplyVectors3((Float3&)m_Position, (Float3&)m_Planes[FP_TOP]);
}

void CLightSpot::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
	CLight::XMLSerialize(rWriter);

	PUTVALUENEW("cutoff", Cutoff(), CFloat);
}

void CLightSpot::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CLight::XMLDeserialize(rReader);

	CString* pValue = NULL;

	GETVALUE { Cutoff(CFloat(*pValue).Value()); }
}
