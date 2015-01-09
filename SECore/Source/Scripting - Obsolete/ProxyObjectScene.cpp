#include "ProxyObjectScene.h"

using namespace Scripting;

using namespace SCFXML;
using namespace SCFScripting;
//using namespace SCFScriptingXML;

CProxyObjectScene::CProxyObjectScene()
{
	m_pObject = NULL;
}

CProxyObjectScene::~CProxyObjectScene()
{
	if (m_pObject) 
	{ 
		RELEASE(*(m_pObject));
		delete m_pObject;
	}
}

void CProxyObjectScene::Register()
{
	CClass* pClass = new CClass(CRegistry::Class(ClassProxy));
	{
		CProperty* pProperty;

		pProperty = new CProperty(SCFScripting::Float4, (PTR_PROPERTY_SETTER)&PositionSet, (PTR_PROPERTY_GETTER)&Position);
		pClass->PropertyAdd(STRING("position"), *pProperty);

		pProperty = new CProperty(SCFScripting::Float3, (PTR_PROPERTY_SETTER)&RotationSet, (PTR_PROPERTY_GETTER)&Rotation);
		pClass->PropertyAdd(STRING("rotation"), *pProperty);
	}
	CRegistry::ClassRegister(STRING("ObjectScene"), ClassProxyObjectScene, *pClass);
}

void CProxyObjectScene::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
	PUTVALUE("position", *Position());
	PUTVALUE("rotation", *Rotation());
}

void CProxyObjectScene::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CString* pValue = NULL;
	
	GETVALUE { PositionSet(CFloat4(*pValue)); }
	GETVALUE { RotationSet(CFloat3(*pValue)); }
}