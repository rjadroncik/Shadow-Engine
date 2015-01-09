#include "Proxy.h"

using namespace Scripting;

using namespace SCFXML;
using namespace SCFScripting;
//using namespace SCFScriptingXML;

void CProxy::Register()
{
	CClass* pClass = new CClass(CRegistry::Class(ClassProxy));
	{
		CProperty* pProperty = new CProperty(String, (PTR_PROPERTY_SETTER)&IDSet, (PTR_PROPERTY_GETTER)&ID); 
		pClass->PropertyAdd(STRING("id"), *pProperty);
	}
	CRegistry::ClassRegister(STRING("SE.Proxy"), ClassProxy, *pClass);
}