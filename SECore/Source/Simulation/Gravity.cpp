#include "Gravity.h"

using namespace Rendering;
using namespace Simulation;

CGravity::CGravity()
{
	m_pObjectsAffected = NULL;
	m_uiObjectsAffectedCount = 0;
}

CGravity::~CGravity()
{
	CMemory::Free(m_pObjectsAffected);
}

void CGravity::ObjectsAffectedAdd(_IN CObjectScene& rObject)
{
	SCF_UNREFERENCED_PARAMETER(rObject);
}

void CGravity::ObjectsAffectedRemove(_IN CObjectScene& rObject)
{
	SCF_UNREFERENCED_PARAMETER(rObject);
}

void CGravity::ObjectsAffectedRemove(_IN SCF::UINT uiIndex)
{
	SCF_UNREFERENCED_PARAMETER(uiIndex);
}

void CGravity::ObjectsAffectedUpdate()
{

}
