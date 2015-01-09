#include "ProxyEditField.h"

#include "../../../SECore/Source/Resources/DataPool.h"
#include "../../../SECore/Source/Scripting/Scripting.h"

#include "ProxyFont.h"

using namespace GUI;
using namespace Scripting;
using namespace Resources;

using namespace SCFXML;
using namespace SCFScripting;
//using namespace SCFScriptingXML;
using namespace SCFImaging;

CProxyEditField::CProxyEditField()
{
}

CProxyEditField::~CProxyEditField()
{
}

void CProxyEditField::Register()
{
	CXMLObjectSerializable::ConstructorRegister(ClassProxyEditField, CProxyEditField::New);

	CClass* pClass = new CClass();
	{
/*		CProperty* pProperty;

		pProperty = new CProperty(ClassArrayFloat);
		pClass->PropertyAdd(STRING("position"), *pProperty);

		pProperty = new CProperty(ClassArrayFloat);
		pClass->PropertyAdd(STRING("rotation"), *pProperty);
*/	}
	CRegistry::ClassRegister(STRING("EditField"), ClassProxyEditField, *pClass);
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

void CProxyEditField::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
}

void CProxyEditField::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	m_pObject = new CEditField();
	ADDREF(*(m_pObject));

	CString* pValue = NULL;

	//Text
	rReader.GetBlock();
	{
		GETVALUE { ((CEditField*)m_pObject)->Text().Font(((CProxyFont*)CScripting::Object(*pValue))->Value()); }

		GETVALUE { ((CEditField*)m_pObject)->TextAlignment(CRegistry::EnumerationTranslate(STRING("TextAlignments"), *pValue)->Value()); }

		GETVALUE { ((CEditField*)m_pObject)->Text().Color(CFloat4(*pValue).Value()); }
		
		//Text data
		rReader.GetBlock();
		((CEditField*)m_pObject)->Text().String(*rReader.GetBlockData());
	}

	m_pObject->LayOut();
}