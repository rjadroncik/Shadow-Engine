#include "ProxyModel.h"
#include "Scripting.h"

using namespace Scripting;

using namespace SCFXML;
using namespace SCFScripting;
//using namespace SCFScriptingXML;

CProxyModel::CProxyModel()
{
	m_pMesh     = NULL;
	m_pMaterial = NULL;
}

CProxyModel::~CProxyModel()
{
	if (m_pMesh)     { RELEASE(*m_pMesh); }
	if (m_pMaterial) { RELEASE(*m_pMaterial); }
}

void CProxyModel::Register()
{
	CXMLObjectSerializable::ConstructorRegister(ClassProxyModel, CProxyModel::New);

	CClass* pClass = new CClass(CRegistry::Class(ClassProxyObjectScene));
	{
		CProperty* pProperty;

		pProperty = new CProperty(SCFScripting::Bool, (PTR_PROPERTY_SETTER)&ShadowCastsSet, (PTR_PROPERTY_GETTER)&ShadowCasts);
		pClass->PropertyAdd(STRING("shadowCasts"), *pProperty);

		pProperty = new CProperty(*CRegistry::Class(ClassProxyMesh), (PTR_PROPERTY_SETTER)NULL, (PTR_PROPERTY_GETTER)&Mesh, 0);
		pClass->PropertyAdd(STRING("mesh"), *pProperty);

		pProperty = new CProperty(*CRegistry::Class(ClassProxyMaterial), (PTR_PROPERTY_SETTER)NULL, (PTR_PROPERTY_GETTER)&Material, 0);
		pClass->PropertyAdd(STRING("material"), *pProperty);

		CFunction* pFunction;

		pFunction = new CFunction((PTR_FUNCTION)&BoundsUpdate);
		pClass->MethodAdd(STRING("boundsUpdate"), *pFunction);
	}
	CRegistry::ClassRegister(STRING("Model"), ClassProxyModel, *pClass);
}

void CProxyModel::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
	CProxyObjectScene::XMLSerialize(rWriter);

	PUTVALUE("mesh",     *m_pMesh->ID());
	PUTVALUE("material", *m_pMaterial->ID());

	PUTVALUE("shadowCasts", *ShadowCasts());
}

void CProxyModel::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CString* pPosition = rReader.GetValue();
	CString* pRotation = rReader.GetValue();

	CString* pValue = NULL;

	GETVALUE { m_pMesh     = (CProxyMesh*)    CScripting::Object(*pValue); if (m_pMesh)     { ADDREF(*m_pMesh); } }
	GETVALUE { m_pMaterial = (CProxyMaterial*)CScripting::Object(*pValue); if (m_pMaterial) { ADDREF(*m_pMaterial); } }

	if (m_pMesh && m_pMaterial)
	{
		m_pObject = new Rendering::CModel(m_pMesh->Value(), m_pMaterial->Value());
		ADDREF(*(m_pObject));

		if (pPosition) { m_pObject->Position(CFloat4(*pPosition).Value()); }
		if (pRotation) { m_pObject->Rotation(CFloat3(*pRotation).Value()); }

		GETVALUE { ShadowCastsSet(*pValue); }

		((Rendering::CModel*)m_pObject)->BoundsUpdate();
	}
}