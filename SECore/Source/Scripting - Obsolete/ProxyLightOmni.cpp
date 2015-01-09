#include "ProxyLightOmni.h"

using namespace Scripting;

using namespace SCFXML;
using namespace SCFScripting;
//using namespace SCFScriptingXML;

CProxyLightOmni::CProxyLightOmni()
{
	m_pObject = new Rendering::CLightOmni();
	ADDREF(*(m_pObject));
}

CProxyLightOmni::~CProxyLightOmni()
{
}

void CProxyLightOmni::Register()
{
	CXMLObjectSerializable::ConstructorRegister(ClassProxyLightOmni, CProxyLightOmni::New);

	CClass* pClass = new CClass(CRegistry::Class(ClassProxyLight));
	{
	}
	CRegistry::ClassRegister(STRING("LightOmni"), ClassProxyLightOmni, *pClass);
}

void CProxyLightOmni::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
	CProxyLight::XMLSerialize(rWriter);
}

void CProxyLightOmni::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CProxyLight::XMLDeserialize(rReader);
}