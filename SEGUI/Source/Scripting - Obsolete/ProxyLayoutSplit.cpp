#include "ProxyLayoutSplit.h"

#include "../../../SECore/Source/Resources/DataPool.h"

using namespace GUI;
using namespace Scripting;
using namespace Resources;

using namespace SCFXML;
using namespace SCFScripting;
//using namespace SCFScriptingXML;

CProxyLayoutSplit::CProxyLayoutSplit()
{
	m_pObject = NULL;
}

CProxyLayoutSplit::~CProxyLayoutSplit()
{
	if (m_pObject) { RELEASE(*(m_pObject)); delete m_pObject; }
}

void CProxyLayoutSplit::Register()
{
	CXMLObjectSerializable::ConstructorRegister(ClassProxyLayoutSplit, CProxyLayoutSplit::New);
	
	CEnumeration* pEnumeration = new CEnumeration();
	{
		pEnumeration->ValueDefine(STRING("horizontal"),            LS_HORIZONTAL );
		pEnumeration->ValueDefine(STRING("horizontalFixedTop"),    LS_HORIZONTAL | LS_FIXED_1ST);
		pEnumeration->ValueDefine(STRING("horizontalFixedBottom"), LS_HORIZONTAL | LS_FIXED_2ND);
		pEnumeration->ValueDefine(STRING("vertical"),              LS_VERTICAL );
		pEnumeration->ValueDefine(STRING("verticalFixedTop"),      LS_VERTICAL | LS_FIXED_1ST);
		pEnumeration->ValueDefine(STRING("verticalFixedBottom"),   LS_VERTICAL | LS_FIXED_2ND);
	}
	CRegistry::EnumerationRegister(STRING("LayoutSplitTypes"), *pEnumeration);

	CClass* pClass = new CClass();
	{
/*		CProperty* pProperty;

		pProperty = new CProperty(ClassArrayFloat);
		pClass->PropertyAdd(STRING("position"), *pProperty);

		pProperty = new CProperty(ClassArrayFloat);
		pClass->PropertyAdd(STRING("rotation"), *pProperty);
*/	}
	CRegistry::ClassRegister(STRING("LayoutSplit"), ClassProxyLayoutSplit, *pClass);
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

void CProxyLayoutSplit::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
}

void CProxyLayoutSplit::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CString* pValue = NULL;

	m_pObject = new CLayoutSplit();
	ADDREF(*(m_pObject));

	GETVALUE { m_pObject->SplitType(CRegistry::EnumerationTranslate(STRING("LayoutSplitTypes"), *pValue)->Value()); }
	GETVALUE { m_pObject->SplitValue  (CInt(*pValue).Value()); }
	GETVALUE { m_pObject->SplitPadding(CInt(*pValue).Value()); }
}