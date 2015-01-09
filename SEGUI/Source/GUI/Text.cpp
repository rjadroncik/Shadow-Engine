#include "Text.h"

#include "Element.h"

using namespace GUI;

CText::CText()
{
	m_pFont = NULL;

	ZeroVector2(m_Position);
	ZeroVector2(m_Size);

	SetVector4(m_Color, 255, 255, 255, 255);
}

CText::~CText()
{
}

void CText::Update()
{
	if (!m_pFont) { return; }
	m_pFont->StringMeasure(m_csString, m_Size);
}

void CText::Render()
{
	if (m_pFont)
	{
		glColor4fv(m_Color);

		m_pFont->Select();
		m_pFont->StringDraw(m_csString, m_Position);
	}
}

void CText::RangeToRect(_IN Int2& rRange, _OUT Float4& rOutRect)
{
	ZeroVector4(rOutRect); 
	if (!m_pFont) { return; }

	m_pFont->StringMeasure(CStringRange(m_csString, 0,         rRange[0]), (Float2&)rOutRect);
	m_pFont->StringMeasure(CStringRange(m_csString, rRange[0], rRange[1]), (Float2&)rOutRect[2]);

	rOutRect[0] += m_Position[0];
	rOutRect[1]  = m_Position[1] - m_pFont->Ascent() + m_pFont->Descent();
}

SCF::UINT CText::PositionToIndex(_IN Float2& rPosition)
{
	Float4 Rect = { rPosition[0], rPosition[1], 0, 0 };
	Int2 Range;

	RectToRange(Rect, Range);

	return Range[0];
}

bool CText::RectToRange(_IN Float4& rRect, _OUT Int2& rOutRange)
{
	ZeroVector2(rOutRange);
 	if (!m_pFont) { return FALSE; }

	//If the rectangle is entirely on the left side, set range to (0, 0) & return false
	if ((rRect[0]             < m_Position[0]) &&
	   ((rRect[0] + rRect[2]) < m_Position[0])) { return FALSE; }


	//If the rectangle is entirely on the right side, set range to (length, 0) & return false
	if ((rRect[0]             > (m_Position[0] + m_Size[0])) &&
	   ((rRect[0] + rRect[2]) > (m_Position[0] + m_Size[0]))) 
	{ 
		rOutRange[0] = m_csString.Length(); 
		return FALSE;
	}

	//Overlap defines the clamped (x, y) of the point within the text
 	Float2 Overlap = { __max(0, rRect[0] - m_Position[0]), (rRect[0] + rRect[2]) - m_Position[0] };
																								 
	Float2 Tmp;							

	bool bStartFound = FALSE;

	for (SCF::UINT i = 0; i < m_csString.Length(); i++)
	{
		//Measure the substring starting at the beginning and having the current length
		m_pFont->StringMeasure(CStringRange(m_csString, 0, i + 1), (Float2&)Tmp);

		if (!bStartFound)
		{
			if (Tmp[0] >= Overlap[0]) { rOutRange[0] = i; bStartFound = TRUE; }
		}

		if (bStartFound)
		{
			if (Tmp[0] >= Overlap[1]) { rOutRange[1] = i - rOutRange[0]; return TRUE; }
		}
	}

	rOutRange[1] = m_csString.Length() - rOutRange[0]; 

	//If the rectangle is entirely above or below the text return false to idicate this
	if (((rRect[1]             < (m_Position[1] - m_pFont->Ascent())) &&
		((rRect[1] + rRect[3]) < (m_Position[1] - m_pFont->Ascent()))) ||
		((rRect[1]             > (m_Position[1] + m_pFont->Descent())) &&
		((rRect[1] + rRect[3]) > (m_Position[1] + m_pFont->Descent())))) { return FALSE; }

	return TRUE;
}


