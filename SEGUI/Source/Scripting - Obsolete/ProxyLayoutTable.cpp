#include "ProxyLayoutTable.h"

#include "../../../SECore/Source/Resources/DataPool.h"

using namespace GUI;
using namespace Scripting;
using namespace Resources;

using namespace SCFXML;
using namespace SCFScripting;
//using namespace SCFScriptingXML;

CProxyLayoutTable::CProxyLayoutTable()
{
	m_pObject = NULL;
}

CProxyLayoutTable::~CProxyLayoutTable()
{
	if (m_pObject) { RELEASE(*(m_pObject)); delete m_pObject; }
}

void CProxyLayoutTable::Register()
{
	CXMLObjectSerializable::ConstructorRegister(ClassProxyLayoutTable, CProxyLayoutTable::New);
	
/*	CEnumeration* pEnumeration = new CEnumeration();
	{
		pEnumeration->ValueDefine(STRING("horizontal"),            LS_HORIZONTAL );
		pEnumeration->ValueDefine(STRING("horizontalFixedTop"),    LS_HORIZONTAL | LS_FIXED_1ST);
		pEnumeration->ValueDefine(STRING("horizontalFixedBottom"), LS_HORIZONTAL | LS_FIXED_2ND);
		pEnumeration->ValueDefine(STRING("vertical"),              LS_VERTICAL );
		pEnumeration->ValueDefine(STRING("verticalFixedTop"),      LS_VERTICAL | LS_FIXED_1ST);
		pEnumeration->ValueDefine(STRING("verticalFixedBottom"),   LS_VERTICAL | LS_FIXED_2ND);
	}
	CRegistry::EnumerationRegister(STRING("LayoutTableTypes"), *pEnumeration);
*/
	CClass* pClass = new CClass();
	{
/*		CProperty* pProperty;

		pProperty = new CProperty(ClassArrayFloat);
		pClass->PropertyAdd(STRING("position"), *pProperty);

		pProperty = new CProperty(ClassArrayFloat);
		pClass->PropertyAdd(STRING("rotation"), *pProperty);
*/	}
	CRegistry::ClassRegister(STRING("LayoutTable"), ClassProxyLayoutTable, *pClass);
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

void CProxyLayoutTable::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
}

void CProxyLayoutTable::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CString* pValue = NULL;

	m_pObject = new CLayoutTable();
	ADDREF(*(m_pObject));

	Int2 Size = { 0, 0 };
	GETVALUE { Size[0] = CInt(*pValue).Value(); }
	GETVALUE { Size[1] = CInt(*pValue).Value(); }

	m_pObject->TableSize(Size); 

	GETVALUE { m_pObject->CellPadding(CFloat(*pValue).Value()); }

	//Columns
	rReader.GetBlock();
	{
		SCF::UINT uiCount = 0;
		GETVALUE { uiCount = CInt(*pValue).Value(); }

		for (SCF::UINT i = 0; i < uiCount; i++)
		{
			rReader.GetBlock();
			//GETVALUE { ((CModule*)m_pObject)->ChildAdd(((CProxyWindow*)CScripting::Object(*pValue))->Value()); }
		}
	}

	//Rows
	rReader.GetBlock();
	{
		SCF::UINT uiCount = 0;
		GETVALUE { uiCount = CInt(*pValue).Value(); }

		for (SCF::UINT i = 0; i < uiCount; i++)
		{
			rReader.GetBlock();
			//GETVALUE { ((CModule*)m_pObject)->ChildAdd(((CProxyWindow*)CScripting::Object(*pValue))->Value()); }
		}
	}
}