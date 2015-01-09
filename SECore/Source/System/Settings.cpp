#include "Settings.h"
#include "../Rendering/RendererSettings.h"

using namespace SCFXML;
using namespace System;

SCF::UINT CSettings::CDisplay::s_uiWidth = 640;
SCF::UINT CSettings::CDisplay::s_uiHeight = 480;
SCF::UINT CSettings::CDisplay::s_uiBitDepth = 32;

bool CSettings::CDisplay::s_bFullScreen = FALSE;

CVector<CString> CSettings::CResources::s_DefinitionFiles;

CVector<CString> CSettings::CResources::s_DatafileDirectories;
CVector<CString> CSettings::CResources::s_DatafileDirectoriesSource;

bool CSettings::CResources::s_DatafilesUse = FALSE;

CString CSettings::s_StartupGUIModule;
CString CSettings::s_StartupScene;

CSettings::CUI::SMouse CSettings::CUI::s_Mouse = { 0.3f, 0.3f, 0.3f, 0.03f, 0.03f, 0.03f};

bool CSettings::Load(_IN CFile& rFile)
{
	CEventLog::BlockNew(STRING("Loading settings"));
	{
		CEventLog::EventNew(STRING("Loading: ") + rFile.PathFull());

		CXMLDocumentFile Source(rFile, FALSE);
		Source.ResolveIncludes();

		CXMLElement* pStartup = Source.RootElement()->ChildNamed(STRING("startup"));
		if (pStartup)
		{
			s_StartupGUIModule = pStartup->ChildNamed(STRING("GUIModule"))->IDString();
			s_StartupScene     = pStartup->ChildNamed(STRING("scene")    )->IDString();
		}
		else 
		{ 
			CEventLog::ErrorNew(STRING("Startup parameters missing!"));
			SCFError(ErrorSettingLoadFailed); 
			return FALSE;
		}
	
		CSettings::CDisplay::Load(Source);
		CSettings::CResources::Load(Source);
		CSettings::CUI::Load(Source);
		Rendering::CRendererSettings::Load(Source);

		if (!CResources::DatafileDirectories().Size() || !CResources::DefinitionFiles().Size() || !s_StartupGUIModule.Length() || !s_StartupScene.Length())
		{ 
			CEventLog::ErrorNew(STRING("Startup parameters invalid!"));
			SCFError(ErrorSettingLoadFailed);
			return FALSE;
		}
	}
	CEventLog::BlockClose();

	return TRUE;
}


bool CSettings::CDisplay::Load(_IN CXMLDocument& rDocument)
{
	CXMLElement* pDisplay = rDocument.RootElement()->ChildNamed(STRING("display"));
	if (pDisplay)
	{
		s_uiWidth    = CInt(pDisplay->AttributeNamed(STRING("width")   )->ValueString()).Value();
		s_uiHeight   = CInt(pDisplay->AttributeNamed(STRING("height")  )->ValueString()).Value();
		s_uiBitDepth = CInt(pDisplay->AttributeNamed(STRING("bitDepth"))->ValueString()).Value();
		
		s_bFullScreen = CBool(pDisplay->AttributeNamed(STRING("fullScreen"))->ValueString()).Value();
	}

	return TRUE;
}


bool CSettings::CUI::Load(_IN CXMLDocument& rDocument)
{
	SCF_UNREFERENCED_PARAMETER(rDocument);

	return TRUE;
}

bool CSettings::CResources::Load(_IN CXMLDocument& rDocument)
{
	CXMLElement* pDatafileDirectories = rDocument.RootElement()->ChildNamed(STRING("datafileDirectories"));
	if (pDatafileDirectories)
	{
		CXMLEnumerator Enumerator(*pDatafileDirectories);
		while (Enumerator.Next())
		{
			if (Enumerator.Current()->Name() == STRING("datafileDirectory"))
			{
				CString* pPath = new CString(((CXMLElement*)Enumerator.Current())->AttributeNamed(STRING("directory"))->ValueString(), (bool)TRUE);
				if (pPath)
				{
					s_DatafileDirectories.LastAdd(*pPath);
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

	CXMLNode* pDatafileDirectoriesSource = rDocument.RootElement()->ChildNamed(STRING("datafileDirectoriesSource"));
	if (pDatafileDirectoriesSource)
	{
		CXMLEnumerator Enumerator(*pDatafileDirectoriesSource);
		while (Enumerator.Next())
		{
			if (Enumerator.Current()->Name() == STRING("datafileDirectorySource"))
			{
				CString* pPath = new CString(((CXMLElement*)Enumerator.Current())->AttributeNamed(STRING("directory"))->ValueString(), (bool)TRUE);
				if (pPath)
				{
					s_DatafileDirectoriesSource.LastAdd(*pPath);
				}
			}
		}
	}
	else if (!s_DatafilesUse)
	{
		CEventLog::ErrorNew(STRING("Startup parameters missing!"));
		SCFError(ErrorSettingLoadFailed); 
		return FALSE;
	}

	CXMLNode* pResourceFiles = rDocument.RootElement()->ChildNamed(STRING("resourceFiles"));
	if (pResourceFiles)
	{
		CXMLEnumerator Enumerator(*pResourceFiles);
		while (Enumerator.Next())
		{
			if (Enumerator.Current()->Name() == STRING("resourceFile"))
			{
				CString* pPath = new CString(((CXMLElement*)Enumerator.Current())->AttributeNamed(STRING("file"))->ValueString(), (bool)TRUE);
				if (pPath)
				{
					s_DefinitionFiles.LastAdd(*pPath);
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


