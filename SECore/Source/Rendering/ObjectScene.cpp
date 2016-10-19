#include "ObjectScene.h"

using namespace Rendering;

const Float3 CObjectScene::s_Forward = { 0, 0, -1 };
const Float3 CObjectScene::s_Up      = { 0, 1,  0 };
const Float3 CObjectScene::s_Right   = { 1, 0,  0 };

CObjectScene::CObjectScene()
{
	m_bDynamic = TRUE;
	m_bChanged = TRUE;

	m_fDistanceToCamera  = 0;
	m_fDistanceToLight   = 0;

	m_bVisibleToCamera = FALSE;
	m_bVisibleToLight  = FALSE;
}

CObjectScene::~CObjectScene()
{
}

bool __fastcall CObjectScene::BoundsOverlap(_IN CObjectScene& rObject) _GET
{
	return (MeasureDistance3((Float3&)m_Position, (Float3&)rObject.Position()) < (m_fBoundsRadius + rObject.BoundsRadius()));
}

bool __fastcall CObjectScene::BoundsBehindPlane(_IN Float4& rPlane) _GET
{
	if ((PointPlaneDistance((Float3&)m_Position, rPlane) + m_fBoundsRadius) < 0) { return TRUE; }

	return FALSE;
}

bool __fastcall CObjectScene::BoundsBehindPlane(_IN Float4& rPlane, _IN float fTolerance) _GET
{
	if ((PointPlaneDistance((Float3&)m_Position, rPlane) + m_fBoundsRadius) < fTolerance) { return TRUE; }

	return FALSE;
}

void CObjectScene::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
	PUTVALUE_TOSTRING("position", Position(), CFloat4);
	PUTVALUE_TOSTRING("rotation", Rotation(), CFloat3);
}

void CObjectScene::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CString* pValue = NULL;

	GETVALUE { Position(CFloat4(*pValue).Value()); }
	GETVALUE { Rotation(CFloat3(*pValue).Value()); }
}