#include "ProxyLayout.h"

using namespace Scripting;

CProxyLayout::SProperties CProxyLayout::s_Properties;

CProxyLayout::CProxyLayout()
{
	m_pObject = NULL;
}

CProxyLayout::~CProxyLayout()
{
	if (m_pObject) { m_pObject->Release(); delete m_pObject; }
}

void CProxyLayout::PropertiesAdd(_OUT CAccessibleClass& rOutClass)
{
/*	CAccessibleProperty* pProperty;
	
	pProperty = new CAccessibleProperty(ClassFloat4);
	rOutClass.AddProperty(STRING("rect"), *pProperty);

	pProperty = new CAccessibleProperty(ClassFloat2);
	rOutClass.AddProperty(STRING("sizeMin"), *pProperty);
	
	pProperty = new CAccessibleProperty(ClassFloat2);
	rOutClass.AddProperty(STRING("sizeMax"), *pProperty);
	
	pProperty = new CAccessibleProperty(ClassFloat2, 0, 0, 1, 0);
	rOutClass.AddProperty(STRING("sizeIdeal"), *pProperty);*/
}

void CProxyLayout::Register()
{
/*	CAccessibleClass* pClass = new CAccessibleClass(ClassProxyObjectScene);
	{
		CProxyLayout::PropertiesAdd(*pClass);
	}
	CAccessibleRegistry::ClassRegister(STRING("GUILayout"), ClassProxyLayout, *pClass);

	s_Properties.uiRect      = CAccessibleRegistry::IDOfProperty(STRING("rect"));
	s_Properties.uiSizeMin   = CAccessibleRegistry::IDOfProperty(STRING("sizeMin"));
	s_Properties.uiSizeMax   = CAccessibleRegistry::IDOfProperty(STRING("sizeMax"));
	s_Properties.uiSizeIdeal = CAccessibleRegistry::IDOfProperty(STRING("sizeIdeal"));
*/
}

void CProxyLayout::PropertySet(_IN ENUM eName, _IN CObject& rValue) _SET
{
/*	if (eName == s_Properties.uiRect)    { m_pObject->Rect   (((CFloat4&)rValue).Value()); return; }
	if (eName == s_Properties.uiSizeMin) { m_pObject->SizeMin(((CFloat2&)rValue).Value()); return; }
	if (eName == s_Properties.uiSizeMax) { m_pObject->SizeMax(((CFloat2&)rValue).Value()); return; }
*/}

CObject* CProxyLayout::PropertyGet(_IN ENUM eName) _GET
{
/*	if (eName == s_Properties.uiRect)      { return new CFloat4(m_pObject->Rect()); }
	if (eName == s_Properties.uiSizeMin)   { return new CFloat2(m_pObject->SizeMin()); }
	if (eName == s_Properties.uiSizeMax)   { return new CFloat2(m_pObject->SizeMax()); }
	if (eName == s_Properties.uiSizeIdeal) { return new CFloat2(m_pObject->SizeIdeal()); }
*/
	return NULL;
}

void CProxyLayout::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
}

void CProxyLayout::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
/*	CString* pValue = NULL;

	IFVALUE { m_pObject->Rect   (CFloat4(*pValue).Value()); }
	IFVALUE { m_pObject->SizeMin(CFloat2(*pValue).Value()); }
	IFVALUE { m_pObject->SizeMax(CFloat2(*pValue).Value()); }*/
}