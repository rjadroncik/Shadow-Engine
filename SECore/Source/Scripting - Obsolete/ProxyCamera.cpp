#include "ProxyCamera.h"

using namespace Scripting;

using namespace SCFXML;
using namespace SCFScripting;
//using namespace SCFScriptingXML;

CProxyCamera::CProxyCamera()
{
	m_pObject = new Rendering::CCamera();
	ADDREF(*(m_pObject));
}

CProxyCamera::~CProxyCamera()
{
}

void CProxyCamera::Register()
{
	CXMLObjectSerializable::ConstructorRegister(ClassProxyCamera, CProxyCamera::New);

	CClass* pClass = new CClass(CRegistry::Class(ClassProxyObjectScene));
	{
/*		CProxyObjectScene::PropertiesAdd(*pClass);
	
		CProperty* pProperty;

		pProperty = new CProperty(ClassFloat);
		pClass->PropertyAdd(STRING("fov"), *pProperty);

		pProperty = new CProperty(ClassFloat);
		pClass->PropertyAdd(STRING("near"), *pProperty);
	
		pProperty = new CProperty(ClassFloat);
		pClass->PropertyAdd(STRING("far"), *pProperty);*/
	}
	CRegistry::ClassRegister(STRING("Camera"), ClassProxyCamera, *pClass);
}

void CProxyCamera::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
	CProxyObjectScene::XMLSerialize(rWriter);

	rWriter.PutValue(STRING("fov"),  *(new CFloat(((Rendering::CCamera*)m_pObject)->FOV())));
	rWriter.PutValue(STRING("near"), *(new CFloat(((Rendering::CCamera*)m_pObject)->Near())));
	rWriter.PutValue(STRING("far"),  *(new CFloat(((Rendering::CCamera*)m_pObject)->Far())));
}

void CProxyCamera::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CProxyObjectScene::XMLDeserialize(rReader);

	CString* pValue = NULL;
	
	GETVALUE { ((Rendering::CCamera*)m_pObject)->FOV (CFloat(*pValue).Value()); }
	GETVALUE { ((Rendering::CCamera*)m_pObject)->Near(CFloat(*pValue).Value()); }
	GETVALUE { ((Rendering::CCamera*)m_pObject)->Far (CFloat(*pValue).Value()); }
}