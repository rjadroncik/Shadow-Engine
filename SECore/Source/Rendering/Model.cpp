#include "Model.h"

#include "RendererDGC.h"
 
#include "../System/Scripting.h"

using namespace Rendering;
using namespace Resources;
using namespace System;

CModel::CModel()
{
	m_pMaterial = NULL;
	m_pMesh     = NULL;

	m_bShadowCasts = FALSE;
}

CModel::CModel(_IN _REF Resources::CMesh& rMesh, _IN _REF CMaterial& rMaterial)
{
	m_pMaterial = &rMaterial;
	m_pMesh     = &rMesh;

	m_bShadowCasts = FALSE;

	ADDREF(*m_pMaterial);
	ADDREF(*m_pMesh);

	((CMesh*)m_pMesh)->AttributeTNCompute();
}

CModel::~CModel()
{
	RELEASE(*m_pMaterial);
	RELEASE(*m_pMesh);
}

void __fastcall CModel::BoundsUpdate()
{
	static Float3 s_BoundsMin;
	static Float3 s_BoundsMax;

	ZeroVector3(s_BoundsMin);
	ZeroVector3(s_BoundsMax);

	for (USHORT i = 0; i < m_pMesh->VertexCount(); i++)
	{
		register const Float3& rPosition = (Float3&)(m_pMesh->Vertices()[i]);

		if (s_BoundsMin[0] > rPosition[0]) { s_BoundsMin[0] = rPosition[0]; }  
		if (s_BoundsMin[1] > rPosition[1]) { s_BoundsMin[1] = rPosition[1]; }  
		if (s_BoundsMin[2] > rPosition[2]) { s_BoundsMin[2] = rPosition[2]; }  

		if (s_BoundsMax[0] < rPosition[0]) { s_BoundsMax[0] = rPosition[0]; }  
		if (s_BoundsMax[1] < rPosition[1]) { s_BoundsMax[1] = rPosition[1]; }  
		if (s_BoundsMax[2] < rPosition[2]) { s_BoundsMax[2] = rPosition[2]; }  
	}

	float fRadius01 = MeasureVector3(s_BoundsMin);
	float fRadius02 = MeasureVector3(s_BoundsMax);

	//Return bounding sphere radius
	m_fBoundsRadius = ((fRadius01 > fRadius02) ? fRadius01 : fRadius02);
}

void CModel::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
	CObjectScene::XMLSerialize(rWriter);

	PUTVALUE_TOSTRING("mesh",     m_pMesh->ID(), CString);
	PUTVALUE_TOSTRING("material", m_pMaterial->ID(), CString);

	PUTVALUE_TOSTRING("shadowCasts", ShadowCasts(), CBool);
}

void CModel::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CObjectScene::XMLDeserialize(rReader);

	CString* pValue = NULL;

	GETVALUE { m_pMesh     = (CMesh*)    CScripting::Object(*pValue); ADDREF(*m_pMesh); }
	GETVALUE { m_pMaterial = (CMaterial*)CScripting::Object(*pValue); ADDREF(*m_pMaterial); }

	GETVALUE { ShadowCasts(CBool(*pValue).Value()); }

	((CMesh*)m_pMesh)->AttributeTNCompute();

	BoundsUpdate();
}