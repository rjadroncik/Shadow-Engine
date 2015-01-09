#include "ObjectBounded.h"

#include "Material.h"

using namespace Rendering;

Float3 CObjectBounded::s_TempVector01 = { 0, 0, 0 };
Float3 CObjectBounded::s_TempVector02 = { 0, 0, 0 };

CObjectBounded::CObjectBounded()
{
	m_fBoundsRadius = 0;
}

CObjectBounded::~CObjectBounded()
{
}

void __fastcall CObjectBounded::BoundsRender(_IN float fR, _IN float fG, _IN float fB) const
{
//Prepare Renderer
	//CMaterialUsage::ApplyDefault();
	glColor3d(fR, fG, fB);

	_PENDING; //CHelper::RenderSphereWF(0, 0, 0, 0, 0, 0, m_fBoundsRadius);
}