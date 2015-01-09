#include "ProxyCursor.h"

#include "../../../SECore/Source/Resources/DataPool.h"
#include "../../../SECore/Source/Scripting/Scripting.h"

using namespace Scripting;
using namespace Resources;

using namespace SCFXML;
using namespace SCFScripting;
//using namespace SCFScriptingXML;
using namespace SCFImaging;


CProxyCursor::CProxyCursor()
{
	m_pObject = NULL;
}

CProxyCursor::~CProxyCursor()
{
	if (m_pObject) { RELEASE(*(m_pObject)); delete m_pObject; }
}

void CProxyCursor::Register()
{
	CXMLObjectSerializable::ConstructorRegister(ClassProxyCursor, CProxyCursor::New);

	CClass* pClass = new CClass();
	{
/*		CProperty* pProperty;

		pProperty = new CProperty(ClassArrayFloat);
		pClass->PropertyAdd(STRING("position"), *pProperty);

		pProperty = new CProperty(ClassArrayFloat);
		pClass->PropertyAdd(STRING("rotation"), *pProperty);
*/	}
	CRegistry::ClassRegister(STRING("Cursor"), ClassProxyCursor, *pClass);

//	s_Properties.uiPosition = CRegistry::IDOfProperty(STRING("position"));
//	s_Properties.uiRotation = CRegistry::IDOfProperty(STRING("rotation"));
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

void CProxyCursor::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
}

void CProxyCursor::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CString* pValue = NULL;	

	GETVALUE { m_pObject = new Resources::CCursor(((CProxyTexture*)CScripting::Object(*pValue))->Value()); ADDREF(*(m_pObject)); }

	GETVALUE { m_pObject->Size   (CFloat2(*pValue).Value()); }
	GETVALUE { m_pObject->HotSpot(CFloat2(*pValue).Value()); }
}