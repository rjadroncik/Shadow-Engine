#include "ProxyScene.h"

#include "Scripting.h"

using namespace Scripting;

using namespace SCFXML;
using namespace SCFScripting;
//using namespace SCFScriptingXML;

CProxyScene::CProxyScene()
{
	m_pCameraCurrent = NULL;

	m_pObject = new Rendering::CScene();
	ADDREF(*(m_pObject));
}

CProxyScene::~CProxyScene()
{
	RELEASE(*(m_pObject));
	delete m_pObject;
}

void CProxyScene::Register()
{
	CXMLObjectSerializable::ConstructorRegister(ClassProxyScene, CProxyScene::New);

	CClass* pClass = new CClass(CRegistry::Class(ClassProxy));
	{
		CProperty* pProperty;

		pProperty = new CProperty(*CRegistry::Class(ClassProxyCamera), (PTR_PROPERTY_SETTER)&CameraCurrentSet, (PTR_PROPERTY_GETTER)&CameraCurrent);
		pClass->PropertyAdd(STRING("cameraCurrent"), *pProperty);

		pProperty = new CProperty(SCFScripting::Vector, (PTR_PROPERTY_SETTER)NULL, (PTR_PROPERTY_GETTER)&Cameras, 0);
		pClass->PropertyAdd(STRING("cameras"), *pProperty);

		pProperty = new CProperty(SCFScripting::Vector, (PTR_PROPERTY_SETTER)NULL, (PTR_PROPERTY_GETTER)&Lights, 0);
		pClass->PropertyAdd(STRING("lights"), *pProperty);

		pProperty = new CProperty(SCFScripting::Vector, (PTR_PROPERTY_SETTER)NULL, (PTR_PROPERTY_GETTER)&Models, 0);
		pClass->PropertyAdd(STRING("models"), *pProperty);

		pProperty = new CProperty(SCFScripting::Vector, (PTR_PROPERTY_SETTER)NULL, (PTR_PROPERTY_GETTER)&ParticleSystems, 0);
		pClass->PropertyAdd(STRING("particleSystems"), *pProperty);
	}
	CRegistry::ClassRegister(STRING("Scene"), ClassProxyScene, *pClass);
}

void CProxyScene::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
	if (m_pCameraCurrent) { PUTVALUE("activeCamera", *m_pCameraCurrent->ID()); }
	else                  { PUTVALUE("activeCamera", *(new STRING(""))); }

	rWriter.BlockStart(STRING("cameras"));
	PUTVALUE("count", *(new CInt(m_Cameras.Size())));

	for (SCF::UINT i = 0; i < m_Cameras.Size(); i++)
	{
		rWriter.BlockStart(STRING("camera"));
		PUTVALUE("id", *((CProxy&)m_Cameras[i]).ID());
		rWriter.BlockEnd();
	}
	rWriter.BlockEnd();

	rWriter.BlockStart(STRING("lights"));
	PUTVALUE("count", *(new CInt(m_Lights.Size())));

	for (SCF::UINT i = 0; i < m_Lights.Size(); i++)
	{
		rWriter.BlockStart(STRING("light"));
		PUTVALUE("id", *((CProxy&)m_Lights[i]).ID());
		rWriter.BlockEnd();
	}
	rWriter.BlockEnd();

	rWriter.BlockStart(STRING("models"));
	PUTVALUE("count", *(new CInt(m_Models.Size())));

	for (SCF::UINT i = 0; i < m_Models.Size(); i++)
	{
		rWriter.BlockStart(STRING("model"));
		PUTVALUE("id", *((CProxy&)m_Models[i]).ID());
		rWriter.BlockEnd();
	}
	rWriter.BlockEnd();

	rWriter.BlockStart(STRING("particleSystems"));
	PUTVALUE("count", *(new CInt(m_ParticleSystems.Size())));

	for (SCF::UINT i = 0; i < m_ParticleSystems.Size(); i++)
	{
		rWriter.BlockStart(STRING("particleSystem"));
		PUTVALUE("id", *((CProxy&)m_ParticleSystems[i]).ID());
		rWriter.BlockEnd();
	}
	rWriter.BlockEnd();
}

void CProxyScene::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CString* pValue = NULL;	
	CString* pActiveCamera = rReader.GetValue();

	//Models
	rReader.GetBlock();
	{
		SCF::UINT uiCount = 0;
		GETVALUE { uiCount = CInt(*pValue).Value(); }

		for (SCF::UINT i = 0; i < uiCount; i++)
		{
			rReader.GetBlock();
			GETVALUE 
			{
				CProxyModel* pProxy = (CProxyModel*)CScripting::Object(*pValue);
				if (pProxy)
				{
					m_Models.LastAdd(*pProxy);
					m_pObject->Add(pProxy->Value()); 
				}
			}
		}
	}

	//Lights
	rReader.GetBlock();
	{
		SCF::UINT uiCount = 0;
		GETVALUE { uiCount = CInt(*pValue).Value(); }

		for (SCF::UINT i = 0; i < uiCount; i++)
		{
			rReader.GetBlock();
			GETVALUE
			{
				CProxyLight* pProxy = (CProxyLight*)CScripting::Object(*pValue);
				if (pProxy)
				{
					m_Lights.LastAdd(*pProxy);
					m_pObject->Add(pProxy->Value()); 
				}
			}
		}
	}

	//Cameras
	rReader.GetBlock();
	{
		SCF::UINT uiCount = 0;
		GETVALUE { uiCount = CInt(*pValue).Value(); }

		for (SCF::UINT i = 0; i < uiCount; i++)
		{
			rReader.GetBlock();
			GETVALUE
			{
				CProxyCamera* pProxy = (CProxyCamera*)CScripting::Object(*pValue);
				if (pProxy)
				{
					m_Cameras.LastAdd(*pProxy);
					m_pObject->Add(pProxy->Value()); 
				}
			}
		}

		if (pActiveCamera)
		{
			CProxyCamera* pProxy = (CProxyCamera*)CScripting::Object(*pActiveCamera);
			if (pProxy)
			{
				m_pCameraCurrent = pProxy; 
				m_pObject->CameraCurrent(&(pProxy->Value())); 
			}
		}
	}

	//ParticleSystems
	rReader.GetBlock();
	{
		SCF::UINT uiCount = 0;
		GETVALUE { uiCount = CInt(*pValue).Value(); }

		for (SCF::UINT i = 0; i < uiCount; i++)
		{
			rReader.GetBlock();
			GETVALUE
			{
				CProxyParticleSystem* pProxy = (CProxyParticleSystem*)CScripting::Object(*pValue);
				if (pProxy)
				{
					m_ParticleSystems.LastAdd(*pProxy);
					m_pObject->Add(pProxy->Value()); 
				}
			}
		}
	}
}