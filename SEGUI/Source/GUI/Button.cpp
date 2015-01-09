#include "Button.h"

using namespace GUI;

CButton::CButton()
{
}

CButton::~CButton()
{
}

#define BUTTON_HOT_COLOR_OFFSET 0.5f

void CButton::OnMouseEnter(_IN SEventMouse& rEvent) 
{
	if (!this->Enabled()) { return; }

	Float4 Color;
	Color[0] = this->ColorBackground()[0] + BUTTON_HOT_COLOR_OFFSET;
	Color[1] = this->ColorBackground()[1] + BUTTON_HOT_COLOR_OFFSET;
	Color[2] = this->ColorBackground()[2] + BUTTON_HOT_COLOR_OFFSET;
	Color[3] = this->ColorBackground()[3] + BUTTON_HOT_COLOR_OFFSET;

	this->ColorBackground(Color);

	if (m_fpOnMouseEnter) { m_fpOnMouseEnter(*this, rEvent); } 
}
void CButton::OnMouseLeave(_IN SEventMouse& rEvent) 
{
	if (!this->Enabled()) { return; }

	Float4 Color;
	Color[0] = this->ColorBackground()[0] - BUTTON_HOT_COLOR_OFFSET;
	Color[1] = this->ColorBackground()[1] - BUTTON_HOT_COLOR_OFFSET;
	Color[2] = this->ColorBackground()[2] - BUTTON_HOT_COLOR_OFFSET;
	Color[3] = this->ColorBackground()[3] - BUTTON_HOT_COLOR_OFFSET;

	this->ColorBackground(Color);

	if (m_fpOnMouseLeave) { m_fpOnMouseLeave(*this, rEvent); }
}
