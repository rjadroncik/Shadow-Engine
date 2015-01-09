#include "ProxyElement.h"

using namespace Scripting;

using namespace SCFXML;
using namespace SCFScripting;
//using namespace SCFScriptingXML;

CProxyElement::CProxyElement()
{
	m_pObject = NULL;
}

CProxyElement::~CProxyElement()
{
	if (m_pObject) { RELEASE(*(m_pObject)); delete m_pObject; }
}

void CProxyElement::PropertiesAdd(_OUT CClass& rOutClass)
{
/*	CProperty* pProperty;
	
	pProperty = new CProperty(ClassFloat4);
	rOutClass.PropertyAdd(STRING("rect"), *pProperty);

	pProperty = new CProperty(ClassFloat2);
	rOutClass.PropertyAdd(STRING("sizeMin"), *pProperty);
	
	pProperty = new CProperty(ClassFloat2);
	rOutClass.PropertyAdd(STRING("sizeMax"), *pProperty);
	
	pProperty = new CProperty(ClassFloat2, 0, 0, 1, 0);
	rOutClass.PropertyAdd(STRING("sizeIdeal"), *pProperty);*/
}

void CProxyElement::Register()
{
/*	CClass* pClass = new CClass(ClassProxyObjectScene);
	{
		CProxyElement::PropertiesAdd(*pClass);
	}
	CRegistry::ClassRegister(STRING("GUIElement"), ClassProxyElement, *pClass);
*/
}

/*
	if (eName == s_Properties.uiRect)    { m_pObject->Rect   (((CFloat4&)rValue).Value()); return; }
	if (eName == s_Properties.uiSizeMin) { m_pObject->SizeMin(((CFloat2&)rValue).Value()); return; }
	if (eName == s_Properties.uiSizeMax) { m_pObject->SizeMax(((CFloat2&)rValue).Value()); return; }

	if (eName == s_Properties.uiRect)      { return new CFloat4(m_pObject->Rect()); }
	if (eName == s_Properties.uiSizeMin)   { return new CFloat2(m_pObject->SizeMin()); }
	if (eName == s_Properties.uiSizeMax)   { return new CFloat2(m_pObject->SizeMax()); }
	if (eName == s_Properties.uiSizeIdeal) { return new CFloat2(m_pObject->SizeIdeal()); }
*/

void CProxyElement::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
}

void CProxyElement::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CString* pValue = NULL;

	GETVALUE { m_pObject->Rect   (CFloat4(*pValue).Value()); }
	GETVALUE { m_pObject->SizeMin(CFloat2(*pValue).Value()); }
	GETVALUE { m_pObject->SizeMax(CFloat2(*pValue).Value()); }
}