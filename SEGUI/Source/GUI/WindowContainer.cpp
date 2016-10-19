#include "WindowContainer.h"

#include "Label.h"

using namespace GUI;

CWindowContainer::CWindowContainer()
{
	m_pLayout = NULL;
}

CWindowContainer::~CWindowContainer()
{
}

void CWindowContainer::LayOut()
{
	if (m_pLayout)
	{
		Float4 LayoutRect = {0, 0, m_Rect[2], m_Rect[3] };

		m_pLayout->LayOut((CVector<CElement>&)m_Windows, LayoutRect, *(Float2*)&m_Rect[2]);
	}
	else
	{
		for (UINT i = 0; i < m_Windows.Size(); i++)
		{
			((CWindowAbstract&)m_Windows[i]).LayOut();
		}
	}
}

CWindowAbstract* CWindowContainer::MouseOver(_IN Float2& rMousePos, _OPT _OUT Float2* pOutLocalPosition) _GET
{
	if (!this->CWindowAbstract::MouseOver(rMousePos)) { return NULL; }

	Float2 LocalMousePos;
	SubtractVectors2(LocalMousePos, rMousePos, (Float2&)m_Rect);

	for (UINT i = 0; i < m_Windows.Size(); i++)
	{				   
		CWindowAbstract* pWindow = ((CWindowAbstract&)m_Windows[i]).MouseOver(LocalMousePos, pOutLocalPosition);

		if (pWindow) { return pWindow; }
	}					 

	if (pOutLocalPosition) { CopyVector2(*pOutLocalPosition, LocalMousePos); }

	return (CWindowAbstract*)this;
}

bool CWindowContainer::MouseLocalPos(_IN Float2& rMousePos, _IN CWindowAbstract& rWindow, _OUT Float2& rOutLocalPosition) _GET
{
	if (this->CWindowAbstract::MouseLocalPos(rMousePos, rWindow, rOutLocalPosition)) { return TRUE; }

	Float2 LocalMousePos;
	SubtractVectors2(LocalMousePos, rMousePos, (Float2&)m_Rect);

	for (UINT i = 0; i < m_Windows.Size(); i++)
	{				   
		if (((CWindowAbstract&)m_Windows[i]).MouseLocalPos(LocalMousePos, rWindow, rOutLocalPosition)) { return TRUE; }
	}					 

	return FALSE;
}


int CWindowContainer::ChildIndexOf(_IN CWindowAbstract& rChild) _GET
{
	for (UINT i = 0; i < m_Windows.Size(); i++)
	{
		if (&m_Windows[i] == &rChild) { return i; }
	}

	return -1;
}

void CWindowContainer::ChildAdd(_INOUT CWindowAbstract& rChild)
{
	if (this->ChildHas(rChild)) { return; }

	m_Windows.LastAdd(rChild);

	rChild.Parent(*this);
	rChild.OnWindowOpen();
}

void CWindowContainer::ChildRemove(_INOUT CWindowAbstract& rChild)
{
	if (!m_Windows.Size()) { return; }

	m_Windows.Remove(rChild);
}

void CWindowContainer::Render()
{
	PositionPush();
	{
		Translate(m_Rect[0], m_Rect[1]);
	
		this->RenderBackground();

 		for (UINT i = 0; i < m_Windows.Size(); i++)
		{
			((CWindowAbstract&)m_Windows[i]).Render();
		}

		this->OnWindowPaint();
	}
	PositionPop();
}

