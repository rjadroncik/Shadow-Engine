#include "ProxyFont.h"

#include "../../../SECore/Source/Resources/DataPool.h"

using namespace GUI;
using namespace Scripting;
using namespace Resources;

using namespace SCFXML;
using namespace SCFScripting;
//using namespace SCFScriptingXML;
using namespace SCFImaging;

CProxyFont::CProxyFont()
{
	m_pObject = NULL;
}

CProxyFont::~CProxyFont()
{
	if (m_pObject) { RELEASE(*(m_pObject)); delete m_pObject; }
}

void CProxyFont::Register()
{
	CXMLObjectSerializable::ConstructorRegister(ClassProxyFont, CProxyFont::New);

	CClass* pClass = new CClass();
	{
/*		CProperty* pProperty;

		pProperty = new CProperty(ClassArrayFloat);
		pClass->PropertyAdd(STRING("position"), *pProperty);

		pProperty = new CProperty(ClassArrayFloat);
		pClass->PropertyAdd(STRING("rotation"), *pProperty);
*/	}
	CRegistry::ClassRegister(STRING("Font"), ClassProxyFont, *pClass);
}

/*	if (eName == s_Properties.uiRect)    { m_pObject->Rect   (((CFloat4&)rValue).Value()); return; }
	if (eName == s_Properties.uiSizeMin) { m_pObject->SizeMin(((CFloat2&)rValue).Value()); return; }
	if (eName == s_Properties.uiSizeMax) { m_pObject->SizeMax(((CFloat2&)rValue).Value()); return; }
*/

/*	if (eName == s_Properties.uiRect)      { return new CFloat4(m_pObject->Rect()); }
	if (eName == s_Properties.uiSizeMin)   { return new CFloat2(m_pObject->SizeMin()); }
	if (eName == s_Properties.uiSizeMax)   { return new CFloat2(m_pObject->SizeMax()); }
	if (eName == s_Properties.uiSizeIdeal) { return new CFloat2(m_pObject->SizeIdeal()); }
*/

void CProxyFont::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
}

void CProxyFont::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
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

			CPicture* pPicture = CPicture::Load(FileExtension, NULL, *pReadStreamImage);
			if (pPicture)
			{
				CStreamReadTextGeneric ReadStreamXMLText(*pReadStreamXML);
				CXMLDocument Document(ReadStreamXMLText);
				CXMLStreamRead XMLReadStream(Document);

				m_pObject = new CFont();
				ADDREF(*(m_pObject));

				m_pObject->Create(XMLReadStream, *pPicture);

				delete pPicture;
			}
		}

		Resources::CDataPool::FileClose(pReadStreamXML);
		Resources::CDataPool::FileClose(pReadStreamImage);
	}
}