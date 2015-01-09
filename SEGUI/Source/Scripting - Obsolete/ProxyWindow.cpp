#include "ProxyWindow.h"

#include "../../../SECore/Source/Resources/DataPool.h"
#include "../../../SECore/Source/Scripting/Scripting.h"

#include "ProxyFont.h"
#include "../GUI/Layout.h"

using namespace GUI;
using namespace Scripting;
using namespace Resources;

using namespace SCFXML;
using namespace SCFScripting;
//using namespace SCFScriptingXML;
using namespace SCFImaging;

CProxyWindow::CProxyWindow()
{
}

CProxyWindow::~CProxyWindow()
{
}

void CProxyWindow::Register()
{
	CXMLObjectSerializable::ConstructorRegister(ClassProxyWindow, CProxyWindow::New);

	CClass* pClass = new CClass();
	{
/*		CProperty* pProperty;

		pProperty = new CProperty(ClassArrayFloat);
		pClass->PropertyAdd(STRING("position"), *pProperty);

		pProperty = new CProperty(ClassArrayFloat);
		pClass->PropertyAdd(STRING("rotation"), *pProperty);
*/	}
	CRegistry::ClassRegister(STRING("Window"), ClassProxyWindow, *pClass);
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

void CProxyWindow::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
}

void CProxyWindow::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	m_pObject = new CWindow();
	ADDREF(*(m_pObject));

	CProxyElement::XMLDeserialize(rReader);

	CString* pValue = NULL;

	GETVALUE { ((CWindow*)m_pObject)->Resizable(CBool(*pValue).Value()); }

	GETVALUE { ((CWindow*)m_pObject)->ButtonMin().Enabled  (CBool(*pValue).Value()); }
	GETVALUE { ((CWindow*)m_pObject)->ButtonMax().Enabled  (CBool(*pValue).Value()); }
	GETVALUE { ((CWindow*)m_pObject)->ButtonClose().Enabled(CBool(*pValue).Value()); }
	
	//Elements
	rReader.GetBlock();
	{
		SCF::UINT uiCount = 0;
		GETVALUE { uiCount = CInt(*pValue).Value(); }

		for (SCF::UINT i = 0; i < uiCount; i++)
		{
			rReader.GetBlock();
			GETVALUE { ((CWindow*)m_pObject)->ChildAdd((CWindowAbstract&)((CProxyElement*)CScripting::Object(*pValue))->Value()); }
		}
	}

	//Text
	rReader.GetBlock();
	{
		GETVALUE { ((CWindow*)m_pObject)->Text().Font(((CProxyFont*)CScripting::Object(*pValue))->Value()); }

		//Skip text alignment for the moment
		rReader.GetValue();

		GETVALUE { ((CWindow*)m_pObject)->Text().Color(CFloat4(*pValue).Value()); }
		
		//Text data
		rReader.GetBlock();
		((CWindow*)m_pObject)->Text().String(*rReader.GetBlockData());
	}

	//Layout
	rReader.GetBlock();
	GETVALUE { ((CWindow*)m_pObject)->Layout((CLayout*)&((CProxy*)CScripting::Object(*pValue))->Value()); }

	m_pObject->LayOut();
}