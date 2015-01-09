#include "Cursor.h"

#include "../../../SECore/Source/System/Scripting.h"

using namespace Resources;
using namespace System;

CCursor::CCursor()
{
	m_pTexture = NULL;

	m_Size[0] = 32;
	m_Size[1] = 32;

	m_HotSpot[0] = 0;
	m_HotSpot[1] = 0;
}

CCursor::CCursor(_IN CTexture& rTexture)
{
	m_pTexture = &rTexture;
	ADDREF(*(m_pTexture));

	m_Size[0] = 32;
	m_Size[1] = 32;

	m_HotSpot[0] = 0;
	m_HotSpot[1] = 0;
}

CCursor::~CCursor()
{
	RELEASE(*(m_pTexture));
}

void CCursor::RenderAt(_IN Float2& rPosition) const
{
	//glEnable(GL_TEXTURE_2D);
	//glEnable(GL_BLEND);
	{
		glBindTexture(GL_TEXTURE_2D, m_pTexture->GLID());
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1, 1, 1, 1);

		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, 0);
			glVertex2f(rPosition[0] - m_HotSpot[0],             rPosition[1] - m_HotSpot[1] + m_Size[1]);
			glTexCoord2f(1, 0);
			glVertex2f(rPosition[0] - m_HotSpot[0] + m_Size[0], rPosition[1] - m_HotSpot[1] + m_Size[1]);
			glTexCoord2f(1, 1);
			glVertex2f(rPosition[0] - m_HotSpot[0] + m_Size[0], rPosition[1] - m_HotSpot[1]);
			glTexCoord2f(0, 1);
			glVertex2f(rPosition[0] - m_HotSpot[0],             rPosition[1] - m_HotSpot[1]);
		}
		glEnd();
	}
	//glDisable(GL_BLEND);
	//glDisable(GL_TEXTURE_2D);
}

void CCursor::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
}

void CCursor::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CString* pValue = NULL;	

	GETVALUE { m_pTexture = (CTexture*)CScripting::Object(*pValue); ADDREF(*m_pTexture); }

	GETVALUE { Size   (CFloat2(*pValue).Value()); }
	GETVALUE { HotSpot(CFloat2(*pValue).Value()); }
}