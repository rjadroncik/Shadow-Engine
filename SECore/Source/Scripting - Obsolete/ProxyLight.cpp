#include "ProxyLight.h"
#include "../Rendering/Renderer20.h"

using namespace Scripting;

using namespace SCFXML;
using namespace SCFScripting;
//using namespace SCFScriptingXML;

CProxyLight::CProxyLight()
{
}

CProxyLight::~CProxyLight()
{
}

void CProxyLight::Register()
{
	CClass* pClass = new CClass(CRegistry::Class(ClassProxyObjectScene));
	{
		CProperty* pProperty;

		pProperty = new CProperty(SCFScripting::Float3, PROPERTY_SETTER(CProxyLight, SCFMathematics::Float3&)&Tint, PROPERTY_GETTER(CProxyLight, const SCFMathematics::Float3&)&Tint);
		pClass->PropertyAdd(STRING("tint"), *pProperty);

		pProperty = new CProperty(SCFScripting::Float, PROPERTY_SETTER(CProxyLight, float)&Ambient, PROPERTY_GETTER(CProxyLight, float)&Ambient);
		pClass->PropertyAdd(STRING("ambient"), *pProperty);
	
		pProperty = new CProperty(SCFScripting::Float, PROPERTY_SETTER(CProxyLight, float)&Diffuse, PROPERTY_GETTER(CProxyLight, float)&Diffuse);
		pClass->PropertyAdd(STRING("diffuse"), *pProperty);
		
		pProperty = new CProperty(SCFScripting::Float, PROPERTY_SETTER(CProxyLight, float)&Specular, PROPERTY_GETTER(CProxyLight, float)&Specular);
		pClass->PropertyAdd(STRING("specular"), *pProperty);
		
		pProperty = new CProperty(SCFScripting::Float, PROPERTY_SETTER(CProxyLight, float)&Range, PROPERTY_GETTER(CProxyLight, float)&Range);
		pClass->PropertyAdd(STRING("range"), *pProperty);
		
		pProperty = new CProperty(SCFScripting::Bool, PROPERTY_SETTER(CProxyLight, bool)&Shadows, PROPERTY_GETTER(CProxyLight, bool)&Shadows);
		pClass->PropertyAdd(STRING("shadows"), *pProperty);
	}
	CRegistry::ClassRegister(STRING("Light"), ClassProxyLight, *pClass);
}

void CProxyLight::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
	CProxyObjectScene::XMLSerialize(rWriter);
	
	PUTVALUENEW("tint",     Tint(),     CFloat3);
	PUTVALUENEW("ambient",  Ambient(),  CFloat);
	PUTVALUENEW("diffuse",  Diffuse(),  CFloat);
	PUTVALUENEW("specular", Specular(), CFloat);
	PUTVALUENEW("range",    Range(),    CFloat);
	PUTVALUENEW("shadows",  Shadows(),  CBool);

	Rendering::CLight* pLight = (Rendering::CLight*)m_pObject;

	if (pLight->Shadows())
	{
		PUTVALUENEW("shadowMapLOD", pLight->ShadowMapLOD(), CInt);
	}
}

void CProxyLight::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CProxyObjectScene::XMLDeserialize(rReader);

	CString* pValue = NULL;

	GETVALUE { Tint    (CFloat3(*pValue).Value()); }
	GETVALUE { Ambient (CFloat(*pValue).Value()); }
	GETVALUE { Diffuse (CFloat(*pValue).Value()); }
	GETVALUE { Specular(CFloat(*pValue).Value()); }
	GETVALUE { Range   (CFloat(*pValue).Value()); }

	GETVALUE 
	{
		Rendering::CLight* pLight = (Rendering::CLight*)m_pObject;

		pLight->Shadows(CBool(*pValue).Value());
		if (pLight->Shadows())
		{
			GETVALUE { pLight->ShadowMapLOD(CInt(*pValue).Value()); }
		}
	}
}