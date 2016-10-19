#include "Texture.h"

using namespace SCFImaging;
using namespace Resources;

CTexture::CTexture()
{
	Construct(NULL, GL_TEXTURE_2D, FALSE);
}

CTexture::CTexture(_IN ENUM eTarget)
{
	Construct(NULL, eTarget, FALSE);
}

CTexture::CTexture(_IN CImage& rImage, _IN ENUM eTarget, _IN bool bMipmaps)
{
	Construct(&rImage, eTarget, bMipmaps);
}

CTexture::~CTexture()
{
	glDeleteTextures(1, &m_uiID);
}

void CTexture::Construct(_IN _OPT CImage* pImage, _IN ENUM eTarget, _IN bool bMipmaps)
{
	//Texture properties
	m_eCoordClamp = GL_REPEAT;
	m_uiMaxAnisotropy = 8;

	m_eTarget  = eTarget;
	m_bMipmaps = bMipmaps;

	//Texture properties
	if (bMipmaps)
	{
		m_eMinification  = GL_LINEAR_MIPMAP_LINEAR;
		m_eMagnification = GL_LINEAR;
	}
	else
	{
		m_eMinification  = GL_LINEAR;
		m_eMagnification = GL_LINEAR;
	}

	glGenTextures(1, &m_uiID);

	if (pImage)
	{
		switch (eTarget)
		{
		case GL_TEXTURE_2D: { LoadImage2D(*pImage); return; }
		}
	}

	m_Size[0] = 0;
	m_Size[1] = 0;

	m_eFormat = GL_RGB;
	m_eFormatInternal = GL_RGB;
	m_eDataType = GL_UNSIGNED_BYTE;
}

bool CTexture::LoadImage2D(_IN CImage& rImage)
{
	//Bind texture in order to initialize it
	glBindTexture(m_eTarget, m_uiID);

	//Load image & build mip-maps, if requested
	if (rImage.ClassKey() == ClassImageDDS)
	{
		CImageDDS* pImageDDS = (CImageDDS*)&rImage;

		if (!pImageDDS->MipmapCount() || !m_bMipmaps)
		{
			glCompressedTexImage2D(m_eTarget, 0, pImageDDS->GLFormat(), pImageDDS->Width(), pImageDDS->Width(), 0, pImageDDS->DataSize(), pImageDDS->Data());
		}
		else
		{
			for (UINT i = 0; i < pImageDDS->MipmapCount(); i++)
			{
				glCompressedTexImage2D(m_eTarget, i, pImageDDS->GLFormat(), pImageDDS->MipmapWidth(i), pImageDDS->MipmapWidth(i), 0, pImageDDS->MipmapDataSize(i), pImageDDS->MipmapData(i));
			}
		}
	}
	else
	{
		if (m_bMipmaps) { gluBuild2DMipmaps(m_eTarget,    rImage.Channels(), rImage.Width(), rImage.Height(),    rImage.GLFormat(), GL_UNSIGNED_BYTE, rImage.Data()); }
		else            { glTexImage2D     (m_eTarget, 0, rImage.Channels(), rImage.Width(), rImage.Height(), 0, rImage.GLFormat(), GL_UNSIGNED_BYTE, rImage.Data()); }
	}

	return TRUE;
}
  
bool CTexture::LoadImage3D(_IN BYTE* bpData, _IN UINT uiWidth, _IN UINT uiHeight, _IN UINT uiDepth, _IN GLenum eFormat)
{
	if (!bpData) { return FALSE; }

	//Setup member variables
	m_bMipmaps = FALSE;
	m_eTarget   = GL_TEXTURE_3D;
	m_eMinification  = GL_LINEAR;
	m_eMagnification  = GL_LINEAR;

	//Bind texture in order to initialize it
	glBindTexture(m_eTarget, m_uiID);

	//Store 3D image as texture
	glTexImage3D(m_eTarget, 0, eFormat, uiWidth, uiHeight, uiDepth, 0, eFormat, GL_UNSIGNED_BYTE, bpData);

	return TRUE;
}

void CTexture::UpdateProperties()
{
	glBindTexture(m_eTarget, m_uiID); 
	
	glTexParameteri(m_eTarget, GL_TEXTURE_MIN_FILTER, m_eMinification);
	glTexParameteri(m_eTarget, GL_TEXTURE_MAG_FILTER, m_eMagnification);

	glTexParameteri(m_eTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_uiMaxAnisotropy);

	glTexParameteri(m_eTarget, GL_TEXTURE_WRAP_S, m_eCoordClamp); 
	glTexParameteri(m_eTarget, GL_TEXTURE_WRAP_T, m_eCoordClamp);
}

void CTexture::UpdateData(_IN void* vpData)
{
	glBindTexture(m_eTarget, m_uiID); 

	glTexImage2D(m_eTarget, 0, m_eFormatInternal, m_Size[0], m_Size[1], 0, m_eFormat, m_eDataType, vpData);
}

#include "../Resources/DataPool.h"

void CTexture::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
}

void CTexture::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CString* pValue = NULL;

	GETVALUE { m_FileName = *pValue; }
	GETVALUE { m_eTarget  = SCFXML::CXMLEnumeration::Translate(STRING("TextureTargets"), *pValue)->Value(); }
	GETVALUE { m_bMipmaps = CBool(*pValue).Value(); }

	//Query file from data pool
	IStreamRead* pReadStream = CDataPool::FileOpen(m_FileName);
	if (pReadStream)
	{
		//Extract the extension to the file
		CString FileExtension;
		CDataPool::ParsePathFile(m_FileName, NULL, NULL, &FileExtension);

		CImage* pImage = CImage::Load(FileExtension, NULL, *pReadStream);
		if (pImage)
		{
			switch (m_eTarget)
			{
			case GL_TEXTURE_2D: { LoadImage2D(*pImage); break; }
			}
			delete pImage;
		}

		CDataPool::FileClose(pReadStream);
	}

	GETVALUE { Minification (SCFXML::CXMLEnumeration::Translate(STRING("TextureMinifications"),  *pValue)->Value()); }
	GETVALUE { Magnification(SCFXML::CXMLEnumeration::Translate(STRING("TextureMagnifications"), *pValue)->Value()); }
	GETVALUE { MaxAnisotropy(CInt(*pValue).Value()); }
	GETVALUE { CoordClamp   (SCFXML::CXMLEnumeration::Translate(STRING("TextureCoordClamps"),    *pValue)->Value()); }

	UpdateProperties();
}
