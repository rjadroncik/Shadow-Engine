#include "ProxyTexture.h"

#include "../Resources/DataPool.h"

using namespace Scripting;
using namespace Resources;

using namespace SCFXML;
using namespace SCFScripting;
//using namespace SCFScriptingXML;
using namespace SCFImaging;

CProxyTexture::CProxyTexture()
{
	m_pObject = NULL;
}

CProxyTexture::~CProxyTexture()
{
	if (m_pObject) { RELEASE(*(m_pObject)); delete m_pObject; }
}

void CProxyTexture::Register()
{
	CXMLObjectSerializable::ConstructorRegister(ClassProxyTexture, CProxyTexture::New);

	CEnumeration* pEnumeration = new CEnumeration();
	{
		pEnumeration->ValueDefine(STRING("1D"),      GL_TEXTURE_1D);
		pEnumeration->ValueDefine(STRING("2D"),      GL_TEXTURE_2D);
		pEnumeration->ValueDefine(STRING("3D"),      GL_TEXTURE_3D);
		pEnumeration->ValueDefine(STRING("cubeMap"), GL_TEXTURE_CUBE_MAP);
	}
	CRegistry::EnumerationRegister(STRING("TextureTargets"), *pEnumeration);

	pEnumeration = new CEnumeration();
	{
		pEnumeration->ValueDefine(STRING("nearest"), GL_NEAREST);
		pEnumeration->ValueDefine(STRING("linear"),  GL_LINEAR);
		pEnumeration->ValueDefine(STRING("nearestMipmapNearest"), GL_NEAREST_MIPMAP_NEAREST);
		pEnumeration->ValueDefine(STRING("linearMipmapNearest"),  GL_LINEAR_MIPMAP_NEAREST);
		pEnumeration->ValueDefine(STRING("nearestMipmapLinear"),  GL_NEAREST_MIPMAP_LINEAR);
		pEnumeration->ValueDefine(STRING("linearMipmapLinear"),   GL_LINEAR_MIPMAP_LINEAR);
	}
	CRegistry::EnumerationRegister(STRING("TextureMinifications"), *pEnumeration);

	pEnumeration = new CEnumeration();
	{
		pEnumeration->ValueDefine(STRING("nearest"), GL_NEAREST);
		pEnumeration->ValueDefine(STRING("linear"),  GL_LINEAR);
	}
	CRegistry::EnumerationRegister(STRING("TextureMagnifications"), *pEnumeration);

	pEnumeration = new CEnumeration();
	{
		pEnumeration->ValueDefine(STRING("clamp"),         GL_CLAMP);
		pEnumeration->ValueDefine(STRING("clampToBorder"), GL_CLAMP_TO_BORDER);
		pEnumeration->ValueDefine(STRING("repeat"),        GL_REPEAT);
	}
	CRegistry::EnumerationRegister(STRING("TextureCoordClamps"), *pEnumeration);

	CClass* pClass = new CClass(CRegistry::Class(ClassProxy));
	{
/*		CProperty* pProperty;

		pProperty = new CProperty(STRING("TextureTargets"));
		pClass->PropertyAdd(STRING("target"), *pProperty);

		pProperty = new CProperty(ClassBool);
		pClass->PropertyAdd(STRING("mipmaps"), *pProperty);

		pProperty = new CProperty(STRING("TextureMinifications"));
		pClass->PropertyAdd(STRING("minification"), *pProperty);

		pProperty = new CProperty(STRING("TextureMagnifications"));
		pClass->PropertyAdd(STRING("magnification"), *pProperty);
		
		pProperty = new CProperty(ClassInt);
		pClass->PropertyAdd(STRING("maxAnisotropy"), *pProperty);
		
		pProperty = new CProperty(STRING("TextureCoordClamps"));
		pClass->PropertyAdd(STRING("coordClamp"), *pProperty);
*/	}
	CRegistry::ClassRegister(STRING("Texture"), ClassProxyTexture, *pClass);
}

void CProxyTexture::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
}

void CProxyTexture::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CString* pFileName = rReader.GetValue();

	SCF::ENUM eTarget = 0;
	bool bMipmaps = FALSE;

	CString* pValue = NULL;
	
	GETVALUE { eTarget = CRegistry::EnumerationTranslate(STRING("TextureTargets"), *pValue)->Value(); }
	GETVALUE { bMipmaps = CBool(*pValue).Value(); }

	//Query file from data pool
	IStreamRead* pReadStream = CDataPool::FileOpen(*pFileName);
	if (pReadStream)
	{
		//Extract the extension to the file
		CString FileExtension;
		CDataPool::ParsePathFile(*pFileName, NULL, NULL, &FileExtension);

		CPicture* pPicture = CPicture::Load(FileExtension, NULL, *pReadStream);
		if (pPicture)
		{
			m_pObject = new Resources::CTexture(*pPicture, eTarget, bMipmaps);
			ADDREF(*(m_pObject));

			delete pPicture;
		}

		CDataPool::FileClose(pReadStream);
	}
	else 
	{ 
		m_pObject = NULL;
		return;
	}

	GETVALUE { m_pObject->Minification (CRegistry::EnumerationTranslate(STRING("TextureMinifications"),  *pValue)->Value()); }
	GETVALUE { m_pObject->Magnification(CRegistry::EnumerationTranslate(STRING("TextureMagnifications"), *pValue)->Value()); }
	GETVALUE { m_pObject->MaxAnisotropy(CInt(*pValue).Value()); }
	GETVALUE { m_pObject->CoordClamp   (CRegistry::EnumerationTranslate(STRING("TextureCoordClamps"),    *pValue)->Value()); }

	m_pObject->UpdateProperties();
}