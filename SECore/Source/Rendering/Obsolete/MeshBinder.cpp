#include "MeshBinder.h"

#include <xmmintrin.h>
#include <intrin.h>

using namespace Rendering;
using namespace Resources;

CMeshBinder::CMeshBinder(_IN CMesh& rMesh)
{
	m_pMaterial = NULL;
	m_pMesh     = &rMesh;
}

CMeshBinder::~CMeshBinder()
{
}

void* CMeshBinder::AsType(_IN ENUM eType) const
{
	switch (eType)
	{
	case ClassMeshBinder: { return (CMeshBinder*)this; }
	}

	return NULL;
}

void __fastcall CMeshBinder::MaterialAttributesBind() const
{
	CMaterial::ProgramAttributesDisable();

	register const CMaterial& rMaterial = *m_pMaterial;
	register const CMesh&     rMesh     = *m_pMesh;

	for (UINT i = 0; i < rMaterial.ProgramAttributesCount(); i++)
	{
		rMesh.BufferAttributeBind(rMaterial.ProgramAttributes(i)->uiSource, rMaterial.ProgramAttributes(i)->uiTarget);
	}
}

void __fastcall CMeshBinder::MaterialApply() const
{
	if (!m_pMaterial) { CMaterial::ApplyDefault(); return; }

	register const CMaterial& rMaterial = *m_pMaterial;

	//Apply material properties
	rMaterial.Apply();

	this->MaterialAttributesBind();
}

void __fastcall CMeshBinder::Material(_IN _REF CMaterial& rMaterial) _SET
{
	m_pMaterial = &rMaterial; 
	
	register const CMesh& rMesh = *m_pMesh;

	bool bNeedTN  = FALSE;

	for (UINT i = 0; i < rMaterial.ProgramAttributesCount(); i++)
	{
		if ((rMaterial.ProgramAttributes(i)->uiSource == VAS_MESH_VERTEX_TANGENT) ||
		    (rMaterial.ProgramAttributes(i)->uiSource == VAS_MESH_VERTEX_NORNAL))
		{
			bNeedTN = TRUE;
		}
	}

	if (bNeedTN) { ((CMesh&)rMesh).AttributeTNCompute(); }
}

float __fastcall CMeshBinder::BoundsRadiusCompute() _GET
{
	if (!m_pMesh) { return 0; }

	static Vector3f s_BoundsMin;
	static Vector3f s_BoundsMax;

	ZeroVector3(s_BoundsMin);
	ZeroVector3(s_BoundsMax);

	for (USHORT i = 0; i < m_pMesh->VertexCount(); i++)
	{
		register const Vector3f& rPosition = (Vector3f&)(m_pMesh->Vertices()[i]);

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
	return ((fRadius01 > fRadius02) ? fRadius01 : fRadius02);
}