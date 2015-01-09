#include "Container.h"

using namespace GUI;

CContainer::CContainer()
{
	m_ppElements   = NULL;
	m_uiElementCount = 0;
}

CContainer::~CContainer()
{
	CMemoryBlock::Free((void**)&m_ppElements);
} 

void CContainer::Add(_INOUT CElement& rElement)
{
	//Simple adding of an item at the last position of an array
	m_uiElementCount++;
	CMemoryBlock::Reallocate((void**)&m_ppElements, sizeof(CElement*) * m_uiElementCount);

	m_ppElements[m_uiElementCount - 1] = &rElement;
}

void CContainer::Remove(_INOUT CElement& rElement)
{
	if (!m_uiElementCount) { return; }

	m_uiElementCount--;

	//This cycle moves the child to be removed to the end of the sequence,
	//keeps order of other children unchanged
	for (UINT i = 0; i < m_uiElementCount; i++)
	{
		//If the child at the current index is the one we are looking for,
		//swap it with the next child, this way we will encounter it in the
		//next cycle & move it again until it is the last item
		if (m_ppElements[i] == &rElement)
		{
			m_ppElements[i]     = m_ppElements[i + 1]; 
			m_ppElements[i + 1] = &rElement;
		}
	}

	if (m_uiElementCount)
	{
		//Realloc the array of children, truncating the last child, which is now the one we wanted to remove
		CMemoryBlock::Reallocate((void**)&m_ppElements, sizeof(CElement*) * m_uiElementCount);
	}
	else
	{
		CMemoryBlock::Free((void**)&m_ppElements);
		m_ppElements = NULL;
	}
} 