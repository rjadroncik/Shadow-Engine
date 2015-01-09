#include "Font.h"

#include "../../../SECore/Source/Resources/DataPool.h"

#include <SCFImaging.h>
#include <SCFXML.h>

using namespace Resources;
using namespace SCFImaging;
using namespace SCFXML;
using namespace SCFBase;

CFont* Font_pCurrent = NULL;

CFont::CFont()
{
	m_uiListBase = 0;
	m_uiTexture  = 0;

	m_uiCharacterMax = 0;
	m_fpCharWidths = NULL;

	m_fHeight  = 16.0f;
	m_fAscent  = 0.0f;
	m_fDescent = 0.0f;
}

CFont::~CFont()
{
	//Free the display list
	glDeleteLists(m_uiListBase, m_uiCharacterMax + 1);

	glDeleteTextures(1, &m_uiTexture);

	CMemory::Free(m_fpCharWidths);
}

void __fastcall CFont::StringMeasure(_IN CString& rString, _OUT Float2& rOutSize) _GET
{
	register float fWidth = 0;

	for (SCF::UINT i = 0; i < rString.Length(); i++)
	{
		fWidth += m_fpCharWidths[rString[i]];
	}

	rOutSize[0] = fWidth;
	rOutSize[1] = m_fHeight;
}

void __fastcall CFont::StringDraw(_IN CString& rString, _IN Float2& rPosition) const 
{
	glBindTexture(GL_TEXTURE_2D, m_uiTexture);
	glPushMatrix();
	{
		glTranslatef(rPosition[0], rPosition[1], 0);
		glCallLists(rString.Length(), GL_UNSIGNED_SHORT, rString.Value());
	}
	glPopMatrix();
}

void __fastcall CFont::StringDraw(_IN CString& rString, _IN float fX, _IN float fY) const
{ 
	glBindTexture(GL_TEXTURE_2D, m_uiTexture);
	glPushMatrix();
	{
		glTranslatef(fX, fY, 0);
		glCallLists(rString.Length(), GL_UNSIGNED_SHORT, rString.Value());
	}
	glPopMatrix();
}

bool __fastcall CFont::CreateTexture(_IN SCFImaging::CImage& rImage)
{
	glGenTextures(1, &m_uiTexture);
	glBindTexture(GL_TEXTURE_2D, m_uiTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, rImage.Channels(), rImage.Width(), rImage.Height(), 0, rImage.GLFormat(), GL_UNSIGNED_BYTE, rImage.Data()); 

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);

	return TRUE;
}

bool __fastcall CFont::CreateDisplayList(_INOUT SCFXML::CXMLStreamRead& rStream)
{
	rStream.GetBlock();

	SCF::UINT uiGlyphCount = CInt(*rStream.GetValue()).Value();
	CFloat4 GlyphInsets(*rStream.GetValue());
	float fImageSize = CFloat(*rStream.GetValue()).Value();

	m_uiCharacterMax = CInt  (*rStream.GetValue()).Value();
	m_fHeight        = CFloat(*rStream.GetValue()).Value();
	
	//Prepare display list
	m_uiListBase = glGenLists(m_uiCharacterMax + 1);	
	m_fpCharWidths = (float*)CMemory::Allocate(sizeof(float) * (m_uiCharacterMax + 1));

	for (SCF::UINT i = 0; i < uiGlyphCount; i++)
	{
		rStream.GetBlock();

		//Glyph attributes
		int iChar = CInt(*rStream.GetValue()).Value();
	
		m_fpCharWidths[iChar] = 1.0f + CFloat(*rStream.GetValue()).Value();

		CFloat4 BoundingBox(*rStream.GetValue());

		//Process bounding box & create a display list for the current glyph
		m_fHeight = BoundingBox[3] - GlyphInsets[1] - GlyphInsets[3];

		_PENDING;
		m_fAscent = (float)(int)(0.9f * m_fHeight);
		m_fDescent = m_fHeight - m_fAscent;

		glNewList(m_uiListBase + iChar, GL_COMPILE);
		{
			if (iChar != ' ')
			{
				glBegin(GL_QUADS);
				{
					glTexCoord2f( BoundingBox[0]                   / fImageSize, 1.0f - (BoundingBox[1] + BoundingBox[3]) / fImageSize); glVertex2f(0.0f,           BoundingBox[3] - m_fAscent);
					glTexCoord2f((BoundingBox[0] + BoundingBox[2]) / fImageSize, 1.0f - (BoundingBox[1] + BoundingBox[3]) / fImageSize); glVertex2f(BoundingBox[2], BoundingBox[3] - m_fAscent);
					glTexCoord2f((BoundingBox[0] + BoundingBox[2]) / fImageSize, 1.0f -  BoundingBox[1]                   / fImageSize); glVertex2f(BoundingBox[2],                - m_fAscent);
					glTexCoord2f( BoundingBox[0]                   / fImageSize, 1.0f -  BoundingBox[1]                   / fImageSize); glVertex2f(0.0f,                          - m_fAscent);
				}
				glEnd();
			}
			else { m_fpCharWidths[iChar] = BoundingBox[2]; }

			glTranslatef((float)m_fpCharWidths[iChar], 0, 0);
		}
		glEndList();
	}

	return TRUE;
}

bool __fastcall CFont::Create(_INOUT CXMLStreamRead& rStream, _IN SCFImaging::CImage& rImage)
{
	CreateTexture(rImage);
	CreateDisplayList(rStream);

	return TRUE;
}

void __fastcall CFont::Select() const
{
	if (Font_pCurrent != this) 
	{ 
		glListBase(m_uiListBase);
	
		Font_pCurrent = (CFont*)this; 
	}
}

void CFont::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
}

void CFont::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CString* pFileXML   = rReader.GetValue();
	CString* pFileImage = rReader.GetValue();

	if (pFileXML && pFileImage) 
	{
		//Query files from data pool
		IStreamRead* pReadStreamXML   = Resources::CDataPool::FileOpen(*pFileXML);
		IStreamRead* pReadStreamImage = Resources::CDataPool::FileOpen(*pFileImage);

		if (pReadStreamXML && pReadStreamImage)
		{
			//Extract the extension to the file
			CString FileExtension;
			CDataPool::ParsePathFile(*pFileImage, NULL, NULL, &FileExtension);

			CImage* pImage = CImage::Load(FileExtension, NULL, *pReadStreamImage);
			if (pImage)
			{
				CStreamReadTextGeneric ReadStreamXMLText(*pReadStreamXML);
				CXMLDocument Document(ReadStreamXMLText);
				CXMLStreamRead XMLReadStream(Document);

				Create(XMLReadStream, *pImage);

				delete pImage;
			}
		}

		Resources::CDataPool::FileClose(pReadStreamXML);
		Resources::CDataPool::FileClose(pReadStreamImage);
	}
}

 