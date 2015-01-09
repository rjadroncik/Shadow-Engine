#include "ProxyMaterial.h"
#include "Scripting.h"

#include "../Rendering/Renderer20.h"

using namespace Scripting;

using namespace SCFXML;
using namespace SCFScripting;
//using namespace SCFScriptingXML;

CProxyMaterial::CProxyMaterial()
{
	m_pObject = new Rendering::CMaterial();
	ADDREF(*(m_pObject));
}

CProxyMaterial::~CProxyMaterial()
{
	RELEASE(*(m_pObject));
	delete m_pObject;
}

void CProxyMaterial::Register()
{
	CXMLObjectSerializable::ConstructorRegister(ClassProxyMaterial, CProxyMaterial::New);

	CEnumeration* pEnumeration = new CEnumeration();
	{
		pEnumeration->ValueDefine(STRING("spot"),         Rendering::UsageSpot);
		pEnumeration->ValueDefine(STRING("spotShadow"),   Rendering::UsageSpotShadow);
		pEnumeration->ValueDefine(STRING("omni"),         Rendering::UsageOmni);
		pEnumeration->ValueDefine(STRING("omniShadow"),   Rendering::UsageOmniShadow);
		pEnumeration->ValueDefine(STRING("direct"),       Rendering::UsageDirect);
		pEnumeration->ValueDefine(STRING("directShadow"), Rendering::UsageDirectShadow);
	}
	CRegistry::EnumerationRegister(STRING("MaterialUsages"), *pEnumeration);

	pEnumeration = new CEnumeration();
	{
		pEnumeration->ValueDefine(STRING("zero"), GL_ZERO);
		pEnumeration->ValueDefine(STRING("one"),  GL_ONE);

		pEnumeration->ValueDefine(STRING("srcColor"),         GL_SRC_COLOR);
		pEnumeration->ValueDefine(STRING("oneMinusSrcColor"), GL_ONE_MINUS_SRC_COLOR);
		pEnumeration->ValueDefine(STRING("srcAlpha"),         GL_SRC_ALPHA);
		pEnumeration->ValueDefine(STRING("oneMinusSrcAlpha"), GL_ONE_MINUS_SRC_ALPHA);

		pEnumeration->ValueDefine(STRING("dstAlpha"),         GL_DST_ALPHA);
		pEnumeration->ValueDefine(STRING("oneMinusDstAlpha"), GL_ONE_MINUS_DST_ALPHA);
		pEnumeration->ValueDefine(STRING("dstColor"),         GL_DST_COLOR);
		pEnumeration->ValueDefine(STRING("oneMinusDstColor"), GL_ONE_MINUS_DST_COLOR);

		pEnumeration->ValueDefine(STRING("srcAlphaSaturate"), GL_SRC_ALPHA_SATURATE);
	}
	CRegistry::EnumerationRegister(STRING("BlendFactors"), *pEnumeration);

	CClass* pClass = new CClass(CRegistry::Class(ClassProxy));
	{
	}
	CRegistry::ClassRegister(STRING("Material"), ClassProxyMaterial, *pClass);
}

void CProxyMaterial::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
}

void CProxyMaterial::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CString* pValue = NULL;

	GETVALUE 
	{ 
		TransparentSet(*pValue);
		if (m_pObject->Transparent())
		{
			GETVALUE 
			{
				CEnum* pFactor = CRegistry::EnumerationTranslate(STRING("BlendFactors"), *pValue); 
				if (pFactor) { FactorSourceSet(*pFactor); }
			}
			GETVALUE 
			{
				CEnum* pFactor = CRegistry::EnumerationTranslate(STRING("BlendFactors"), *pValue); 
				if (pFactor) { FactorDestinationSet(*pFactor); }
			}
		}	
	}

	GETVALUE { TintSet     (*pValue); }
	GETVALUE { AmbientSet  (*pValue); }
	GETVALUE { DiffuseSet  (*pValue); }
	GETVALUE { SpecularSet (*pValue); }
	GETVALUE { ShininessSet(*pValue); }

	//Usages
	rReader.GetBlock();
	{
		SCF::UINT uiCount = 0;
		GETVALUE { uiCount = CInt(*pValue).Value(); }

		for (SCF::UINT i = 0; i < uiCount; i++)
		{
			rReader.GetBlock();
	
			//Create a new usage for material
			Rendering::CMaterialUsage* pUsage = new Rendering::CMaterialUsage();

			GETVALUE 
			{ 
				//Assign new usage to material
				CEnum* pUsageType = CRegistry::EnumerationTranslate(STRING("MaterialUsages"), *pValue); 
				if (pUsageType) 
				{
					m_pObject->Usage(pUsageType->Value(), pUsage); 
				}
			}

			//Prepare usage
			{
				rReader.GetBlock();
				GETVALUE { pUsage->Program(((CProxyGPUProgram*)CScripting::Object(*pValue))->Value()); }

				//Textures
				rReader.GetBlock();
				{
					SCF::UINT uiCount = 0;
					GETVALUE { uiCount = CInt(*pValue).Value(); }

					for (SCF::UINT i = 0; i < uiCount; i++)
					{
						rReader.GetBlock();

						CString* pSource = rReader.GetValue();
						CString* pID     = rReader.GetValue();

						if (pSource && pID)
						{ 
							CEnum* pEnum = CRegistry::EnumerationTranslate(STRING("SourcesSamplers"), *pSource);
							if (pEnum) 
							{
								for (SCF::UINT j = 0; j < pUsage->Program().SamplersCount(); j++)
								{
									if (pUsage->Program().Sampler(j)->eSource == pEnum->Value()) 
									{
										CProxyTexture* pTexture = (CProxyTexture*)CScripting::Object(*pID);
										if (pTexture)
										{
											pUsage->TextureAdd(pTexture->Value(), pUsage->Program().Sampler(j)->uiTarget); 
										}
									}
								}
							}
						}
					}
				}

				for (SCF::UINT i = 0; i < pUsage->Program().SamplersCount(); i++)
				{
					if (pUsage->Program().Sampler(i)->eSource == SS_LIGHT_DEPTHMAP0) { pUsage->ShadowMapTargetFront(pUsage->Program().Sampler(i)->uiTarget); }
					if (pUsage->Program().Sampler(i)->eSource == SS_LIGHT_DEPTHMAP1) { pUsage->ShadowMapTargetBack (pUsage->Program().Sampler(i)->uiTarget); }
				}
			}
		}
	}
}