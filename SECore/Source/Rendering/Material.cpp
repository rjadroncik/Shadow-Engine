#include "Material.h"
#include "Light.h"

#include "../Resources/Texture.h"
#include "../System/OpenGL20.h"

#include <SCFMathematics.h>

using namespace SCFMathematics;
using namespace Rendering;
using namespace System;
using namespace Resources;

CMaterial::CMaterial()
{
	for (UINT i = 0 ; i < UsageCount; i++)
	{
		m_paUsages[i] = NULL;
	}

	//Material properties
	SetVector3(m_Tint, 1, 1, 1);

	m_fAmbient  = 0.0f;
	m_fDiffuse  = 1.0f;
	m_fSpecular = 1.0f;

	m_fEmission  = 0.0f;
	m_fShininess = 0.0f;

	m_bTransparent = FALSE;

	m_eFactorSource = GL_SRC_ALPHA;
	m_eFactorDestination = GL_ONE;
}

CMaterial::~CMaterial()
{
	for (UINT i = 0 ; i < UsageCount; i++)
	{
		if (m_paUsages[i]) { delete m_paUsages[i]; }
	}
}

const CMaterialUsage* __fastcall CMaterial::Usage(_IN CLight& rLight) _GET
{
	if (rLight.Shadows()) { return m_paUsages[rLight.ClassKey() - ClassLightSpot + 1]; }
	else                  { return m_paUsages[rLight.ClassKey() - ClassLightSpot]; }
}

const CMaterialUsage* __fastcall CMaterial::Usage(_IN ENUM eUsage) _GET
{
	return m_paUsages[eUsage];
}

void __fastcall CMaterial::Usage(_IN ENUM eUsage, _IN CMaterialUsage* pUsage) _SET
{
	m_paUsages[eUsage] = pUsage;
}

#include "../System/Scripting.h"

void CMaterial::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
}

void CMaterial::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CString* pValue = NULL;

	GETVALUE 
	{ 
		Transparent(CBool(*pValue).Value());
		if (Transparent())
		{
			GETVALUE 
			{
				CEnum* pFactor = SCFXML::CXMLEnumeration::Translate(STRING("BlendFactors"), *pValue); 
				if (pFactor) { FactorSource(pFactor->Value()); }
			}
			GETVALUE 
			{
				CEnum* pFactor = SCFXML::CXMLEnumeration::Translate(STRING("BlendFactors"), *pValue); 
				if (pFactor) { FactorDestination(pFactor->Value()); }
			}
		}	
	}

	GETVALUE { Tint     (CFloat3(*pValue).Value()); }
	GETVALUE { Ambient  (CFloat(*pValue).Value()); }
	GETVALUE { Diffuse  (CFloat(*pValue).Value()); }
	GETVALUE { Specular (CFloat(*pValue).Value()); }
	GETVALUE { Shininess(CFloat(*pValue).Value()); }

	//Usages
	rReader.GetBlock();
	{
		UINT uiCount = 0;
		GETVALUE { uiCount = CInt(*pValue).Value(); }

		for (UINT i = 0; i < uiCount; i++)
		{
			rReader.GetBlock();

			//Create a new usage for material
			CMaterialUsage* pUsage = new CMaterialUsage();

			GETVALUE 
			{ 
				//Assign new usage to material
				CEnum* pUsageType = SCFXML::CXMLEnumeration::Translate(STRING("MaterialUsages"), *pValue); 
				if (pUsageType) 
				{
					Usage(pUsageType->Value(), pUsage); 
				}
			}

			//Prepare usage
			{
				rReader.GetBlock();
				GETVALUE { pUsage->Program(*(CGPUProgram*)CScripting::Object(*pValue)); }

				//Textures
				rReader.GetBlock();
				{
					UINT uiCount = 0;
					GETVALUE { uiCount = CInt(*pValue).Value(); }

					for (UINT i = 0; i < uiCount; i++)
					{
						rReader.GetBlock();

						CString* pSource = rReader.GetValue();
						CString* pID     = rReader.GetValue();

						if (pSource && pID)
						{ 
							CEnum* pEnum = SCFXML::CXMLEnumeration::Translate(STRING("SourcesSamplers"), *pSource);
							if (pEnum) 
							{
								for (UINT j = 0; j < pUsage->Program().SamplersCount(); j++)
								{
									if (pUsage->Program().Sampler(j)->eSource == pEnum->Value()) 
									{
										CTexture* pTexture = (CTexture*)CScripting::Object(*pID);
										if (pTexture)
										{
											pUsage->TextureAdd(*pTexture, pUsage->Program().Sampler(j)->uiTarget); 
										}
									}
								}
							}
						}
					}
				}

				for (UINT i = 0; i < pUsage->Program().SamplersCount(); i++)
				{
					if (pUsage->Program().Sampler(i)->eSource == SS_LIGHT_DEPTHMAP0) { pUsage->ShadowMapTargetFront(pUsage->Program().Sampler(i)->uiTarget); }
					if (pUsage->Program().Sampler(i)->eSource == SS_LIGHT_DEPTHMAP1) { pUsage->ShadowMapTargetBack (pUsage->Program().Sampler(i)->uiTarget); }
				}
			}
		}
	}
}