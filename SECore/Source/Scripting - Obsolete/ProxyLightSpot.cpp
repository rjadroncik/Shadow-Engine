#include "ProxyLightSpot.h"

using namespace Scripting;

using namespace SCFXML;
using namespace SCFScripting;
//using namespace SCFScriptingXML;

CProxyLightSpot::CProxyLightSpot()
{
	m_pObject = new Rendering::CLightSpot();
	ADDREF(*(m_pObject));
}

CProxyLightSpot::~CProxyLightSpot()
{
}

void CProxyLightSpot::Register()
{
	CXMLObjectSerializable::ConstructorRegister(ClassProxyLightSpot, CProxyLightSpot::New);

	CClass* pClass = new CClass(CRegistry::Class(ClassProxyLight));
	{
		CProperty* pProperty;

		pProperty = new CProperty(SCFScripting::Float, (PTR_PROPERTY_SETTER)&CutoffSet, (PTR_PROPERTY_GETTER)&Cutoff);
		pClass->PropertyAdd(STRING("cutoff"), *pProperty);
	}
	CRegistry::ClassRegister(STRING("LightSpot"), ClassProxyLightSpot, *pClass);
}

void CProxyLightSpot::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
	CProxyLight::XMLSerialize(rWriter);

	PUTVALUE("cutoff", *Cutoff());
}

void CProxyLightSpot::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CProxyLight::XMLDeserialize(rReader);

	CString* pValue = NULL;
	Rendering::CLight* pLight = (Rendering::CLight*)m_pObject;

	GETVALUE { pLight->Cutoff(CFloat(*pValue).Value()); }
}