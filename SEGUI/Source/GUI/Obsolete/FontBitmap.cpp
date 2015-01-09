#include "FontBitmap.h"

using namespace GUI;

CFontBitmap* CFontBitmap::s_pFontCurrent = NULL;
Vector4i     CFontBitmap::s_Viewport     = { 0 };

CFontBitmap::CFontBitmap()
{
	m_uiListBase  = 0;
	m_uiCharCount = 0;

	m_iHeight  = 0;
	m_iAscent  = 0;
	m_iDescent = 0;

	m_ipCharWidths = NULL;
}

CFontBitmap::~CFontBitmap()
{
	//Free the display list
	glDeleteLists(m_uiListBase, m_uiCharCount);			
}

void* CFontBitmap::AsType(_IN ENUM eType) const
{
	switch (eType)
	{
	case ClassFontBitmap: { return (CFontBitmap*)this; }
	}

	return NULL;
}

void CFontBitmap::StringMeasure(_IN CString& rString, _OUT Vector2i& rOutSize) _GET
{
	register int iWidth = 0;

	for (UINT i = 0; i < rString.Length(); i++)
	{
		iWidth += m_ipCharWidths[rString[i]];
	}

	rOutSize[0] = iWidth;
	rOutSize[1] = m_iHeight;
}

bool CFontBitmap::Create(_IN CString rFontName, _IN UINT uiHeight, _IN HDC hDC, _IN UINT uiCount)	
{
	//Generate the lists for each glyph
	m_uiListBase = glGenLists(uiCount);	

	HFONT hFont = CreateFont(uiHeight, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, rFontName.Value()); 
	if (!hFont) { return FALSE; }

	HFONT hFontOld = (HFONT)SelectObject(hDC, hFont);

	//Create the font bitmaps, 2nd parameter - starting character, 3rd - ending character.   			
	if (!wglUseFontBitmaps(hDC, 0, uiCount - 1, m_uiListBase)) 
	{ 
		SelectObject(hDC, hFontOld); 
		return FALSE; 
	}

	m_csName       = rFontName;
	m_uiCharCount  = uiCount;
	CMemoryBlock::Reallocate((void**)&m_ipCharWidths, sizeof(int) * m_uiCharCount);

	GetCharWidth(hDC, 0, uiCount - 1, m_ipCharWidths);
 
	//Obtain font metrics
	TEXTMETRIC TextMetrics;
	GetTextMetrics(hDC, &TextMetrics);

	m_iHeight  = TextMetrics.tmHeight;
	m_iAscent  = TextMetrics.tmAscent;
	m_iDescent = TextMetrics.tmDescent;

	//Select old font, don't leave memory leaks
	SelectObject(hDC, hFontOld);	

	return TRUE;
}

void CFontBitmap::StringDraw(_IN CString& rString, _IN Vector2i& rPosition) const 
{ 
	glViewport(rPosition[0], s_Viewport[3] - (rPosition[1] + 2001), 0, 0);
	glRasterPos2i(0, 0); 

	glCallLists(rString.Length(), GL_UNSIGNED_SHORT, rString.Value());

	glViewport(s_Viewport[0], s_Viewport[1], s_Viewport[2], s_Viewport[3]);
}
void CFontBitmap::StringDraw(_IN CString& rString, _IN int iX, _IN int iY)  const
{ 
	glViewport(iX, s_Viewport[3] - (iY + 2001), 0, 0);
	glRasterPos2i(0, 0);

	glCallLists(rString.Length(), GL_UNSIGNED_SHORT, rString.Value());

	glViewport(s_Viewport[0], s_Viewport[1], s_Viewport[2], s_Viewport[3]);
}

void CFontBitmap::SelectInternal() const
{
	glListBase(m_uiListBase); 
	glGetIntegerv(GL_VIEWPORT, (int*)s_Viewport);

	s_pFontCurrent = (CFontBitmap*)this; 
}

