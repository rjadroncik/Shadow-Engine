#include "ProxyModule.h"

#include "../../../SECore/Source/Resources/DataPool.h"
#include "../../../SECore/Source/Scripting/Scripting.h"

#include "ProxyWindow.h"
#include "ProxyCursor.h"

using namespace GUI;
using namespace Scripting;
using namespace Resources;

using namespace SCFXML;
using namespace SCFScripting;
//using namespace SCFScriptingXML;
using namespace SCFImaging;

CProxyModule::CProxyModule()
{
}

CProxyModule::~CProxyModule()
{
}

void CProxyModule::Register()
{
	CXMLObjectSerializable::ConstructorRegister(ClassProxyModule, CProxyModule::New);

	CEnumeration* pEnumeration = new CEnumeration();
	{
		pEnumeration->ValueDefine(STRING("Cursor.Arrow"), CursorArrow);
		pEnumeration->ValueDefine(STRING("Cursor.Beam"), CursorBeam);
		pEnumeration->ValueDefine(STRING("Cursor.Busy"), CursorBusy);
		pEnumeration->ValueDefine(STRING("Cursor.Cross"), CursorCross);
		pEnumeration->ValueDefine(STRING("Cursor.Hand"), CursorHand);
		pEnumeration->ValueDefine(STRING("Cursor.Help"), CursorHelp);
		pEnumeration->ValueDefine(STRING("Cursor.Move"), CursorMove);
		pEnumeration->ValueDefine(STRING("Cursor.No"), CursorNo);

		pEnumeration->ValueDefine(STRING("Cursor.Size1"), CursorSizeNESW);
		pEnumeration->ValueDefine(STRING("Cursor.Size2"), CursorSizeNWSE);
		pEnumeration->ValueDefine(STRING("Cursor.Size3"), CursorSizeWE);
		pEnumeration->ValueDefine(STRING("Cursor.Size4"), CursorSizeNS);

		pEnumeration->ValueDefine(STRING("Cursor.Wait"), CursorWait);
	}
	CRegistry::EnumerationRegister(STRING("CursorTypes"), *pEnumeration);

	CClass* pClass = new CClass();
	{
/*		CProperty* pProperty;

		pProperty = new CProperty(ClassArrayFloat);
		pClass->PropertyAdd(STRING("position"), *pProperty);

		pProperty = new CProperty(ClassArrayFloat);
		pClass->PropertyAdd(STRING("rotation"), *pProperty);
*/	}
	CRegistry::ClassRegister(STRING("GUIModule"), ClassProxyModule, *pClass);

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

void CProxyModule::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
}

void CProxyModule::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	m_pObject = new CModule();
	ADDREF(*(m_pObject));

	CString* pValue = NULL;	

	//Cursors
	rReader.GetBlock();
	{
		SCF::UINT uiCount = 0;
		GETVALUE { uiCount = CInt(*pValue).Value(); }

		for (SCF::UINT i = 0; i < uiCount; i++)
		{
			rReader.GetBlock();

			GETVALUE 
			{
				CCursor* pCursor = &((CProxyCursor*)CScripting::Object(*pValue))->Value();

				GETVALUE 
				{ 
					SCF::ENUM eCursorType = CRegistry::EnumerationTranslate(STRING("CursorTypes"), *pValue)->Value();

					((CModule*)m_pObject)->Cursor.Enlist(eCursorType, pCursor);
				}
			}
		}
	}

	//Windows
	rReader.GetBlock();
	{
		SCF::UINT uiCount = 0;
		GETVALUE { uiCount = CInt(*pValue).Value(); }

		for (SCF::UINT i = 0; i < uiCount; i++)
		{
			rReader.GetBlock();
			GETVALUE { ((CModule*)m_pObject)->ChildAdd(((CProxyWindow*)CScripting::Object(*pValue))->Value()); }
		}
	}

	((CModule*)m_pObject)->Cursor.Use(CursorArrow);
}