#include "RendererSettings.h"

using namespace SCFXML;
using namespace System;
using namespace Rendering;

ENUM Settings_eCodePath = CodePathOpenGL20IGP;

UINT Settings_uiMaxModels = 1024;
UINT Settings_uiMaxLights = 4;

UINT Settings_uiMaxShadowMapResolution = 2048;

inline ENUM CRendererSettings::CodePath()                     { return Settings_eCodePath; }
inline void      CRendererSettings::CodePath(_IN ENUM eValue) { Settings_eCodePath = eValue; }

inline UINT CRendererSettings::MaxModels()                      { return Settings_uiMaxModels; }
inline void      CRendererSettings::MaxModels(_IN UINT uiValue) { Settings_uiMaxModels = uiValue; }

inline UINT CRendererSettings::MaxLights()                      { return Settings_uiMaxLights; }
inline void      CRendererSettings::MaxLights(_IN UINT uiValue) { Settings_uiMaxLights = uiValue; }

inline UINT CRendererSettings::MaxShadowMapResolution()                      { return Settings_uiMaxShadowMapResolution; }
inline void      CRendererSettings::MaxShadowMapResolution(_IN UINT uiValue) { Settings_uiMaxShadowMapResolution = uiValue; }

bool CRendererSettings::Load(_IN CXMLDocument& rDocument)
{
	CXMLElement* pRenderer = rDocument.RootElement()->ChildNamed(STRING("renderer"));
	if (pRenderer)
	{
		CXMLAttribute* pCodePath = pRenderer->AttributeFirst();
		if (pCodePath)
		{
			CEnum* pCodePathEnum = SCFXML::CXMLEnumeration::Translate(STRING("CodePaths"), pCodePath->ValueString()); 
			if (pCodePathEnum) { Settings_eCodePath = pCodePathEnum->Value(); }
		}

		CXMLEnumerator Enumerator(*pRenderer);
		while (Enumerator.Next())
		{
			if (Enumerator.Current()->Name() == STRING("shadows"))
			{
				CInt MaxResolution(((CXMLElement*)Enumerator.Current())->AttributeNamed(STRING("maxResolution"))->ValueString());
				if (MaxResolution > 0)
				{
					Settings_uiMaxShadowMapResolution = MaxResolution.Value();
				}
			}
		}
	}
	else 
	{ 
		CEventLog::ErrorNew(STRING("Startup parameters missing!"));
		SCFError(ErrorSettingLoadFailed); 
		return FALSE;
	}

	return TRUE;
}