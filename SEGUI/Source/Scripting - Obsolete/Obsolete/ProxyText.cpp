#include "ProxyText.h"

#include "../../../Core/Source/Resources/DataPool.h"

using namespace GUI;
using namespace Scripting;
using namespace Resources;
using namespace SCFDatafileIO;
using namespace SCFXML;
using namespace SCFImaging;

CProxyText::SProperties CProxyText::s_Properties;

void* CProxyText::AsType(_IN ENUM eType) const
{
	switch (eType)
	{
	case ClassProxyText: { return (CProxyText*)this; }
	}

	return NULL;
}

CProxyText::CProxyText()
{
	m_pObject = NULL;
}

CProxyText::~CProxyText()
{
	if (m_pObject) { delete m_pObject; }
}

void CProxyText::Register()
{
	CObject::RegisterConstructor(ClassProxyText, CProxyText::New);

	CAccessibleClass* pClass = new CAccessibleClass(ClassProxyText);
	{
/*		CAccessibleProperty* pProperty;

		pProperty = new CAccessibleProperty(ClassArrayFloat);
		pClass->AddProperty(STRING("position"), *pProperty);

		pProperty = new CAccessibleProperty(ClassArrayFloat);
		pClass->AddProperty(STRING("rotation"), *pProperty);
*/	}
	CAccessibleRegistry::ClassRegister(STRING("Window"), ClassProxyText, *pClass);

//	s_Properties.uiPosition = CAccessibleRegistry::IDOfProperty(STRING("position"));
//	s_Properties.uiRotation = CAccessibleRegistry::IDOfProperty(STRING("rotation"));
}

void CProxyText::PropertySet(_IN ENUM eName, _IN CObject& rValue) _SET
{
/*	if (eName == s_Properties.uiRect)    { m_pObject->Rect   (((CFloat4&)rValue).Value()); return; }
	if (eName == s_Properties.uiSizeMin) { m_pObject->SizeMin(((CFloat2&)rValue).Value()); return; }
	if (eName == s_Properties.uiSizeMax) { m_pObject->SizeMax(((CFloat2&)rValue).Value()); return; }
*/}

CObject* CProxyText::PropertyGet(_IN ENUM eName) _GET
{
/*	if (eName == s_Properties.uiRect)      { return new CFloat4(m_pObject->Rect()); }
	if (eName == s_Properties.uiSizeMin)   { return new CFloat2(m_pObject->SizeMin()); }
	if (eName == s_Properties.uiSizeMax)   { return new CFloat2(m_pObject->SizeMax()); }
	if (eName == s_Properties.uiSizeIdeal) { return new CFloat2(m_pObject->SizeIdeal()); }
*/
	return NULL;
}

void CProxyText::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
}

void CProxyText::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CString* pFileXML   = rReader.GetValue();
	CString* pFileImage = rReader.GetValue();

	if (pFileXML && pFileImage) 
	{
		//Query files from data pool
		CDFFile* pDFFileXML   = Resources::CDataPool::File(*pFileXML);
		CDFFile* pDFFileImage = Resources::CDataPool::File(*pFileImage);
		
		if (pDFFileXML && pDFFileImage)
		{
			CDFStreamFileRead ReadStreamXML  (*pDFFileXML);
			CDFStreamFileRead ReadStreamImage(*pDFFileImage);

			CPicture* pPicture = CPicture::Load(pDFFileImage->Extension(), NULL, ReadStreamImage);
			if (pPicture)
			{
				CXMLDocument Document(ReadStreamXML);
				CXMLStreamRead XMLReadStream(Document);

				m_pObject = new CWindow();
				m_pObject->AddRef();

				m_pObject->Create(XMLReadStream, *pPicture);

				delete pPicture;
			}

			delete pDFFileXML;
			delete pDFFileImage;
		}
		else 
		{
			//Clean up in case of missing files
			if (pDFFileXML)   { delete pDFFileXML; }
			if (pDFFileImage) { delete pDFFileImage; }
		}
	}
}