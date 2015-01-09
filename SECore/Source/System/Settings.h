#pragma once

#include "../SECore.h"

#include <SCFXML.h>

namespace System
{
	class SECORE_API CSettings
	{
	public:
		static bool Load(_IN CFile& rFile);

	public:
		class SECORE_API CDisplay
		{
		public:
			static bool Load(_IN SCFXML::CXMLDocument& rDocument);

		public:
			static SCF::UINT Width()                        { return s_uiWidth; }
			static void      Width(SCF::UINT uiWidth)       { s_uiWidth = uiWidth; }

			static SCF::UINT Height()                       { return s_uiHeight; }
			static void      Height(SCF::UINT uiHeight)     { s_uiHeight = uiHeight; }

			static SCF::UINT BitDepth()                     { return s_uiBitDepth; }

			static bool FullScreen() { return s_bFullScreen; }

		private:
			static SCF::UINT s_uiWidth;
			static SCF::UINT s_uiHeight;
			static SCF::UINT s_uiBitDepth;
			
			static bool s_bFullScreen;
		};

	public:
		class SECORE_API CResources
		{
		public:
			static bool Load(_IN SCFXML::CXMLDocument& rDocument);

		public:
#pragma warning(disable:4251)
			static const CVector<CString>& DatafileDirectories()       { return s_DatafileDirectories; }
			static const CVector<CString>& DatafileDirectoriesSource() { return s_DatafileDirectoriesSource; }
		
			static const CVector<CString>& DefinitionFiles() { return s_DefinitionFiles; }
#pragma warning(default:4251) 

			static const bool DatafilesUse() { return s_DatafilesUse; }

		private:
#pragma warning(disable:4251)
			static CVector<CString> s_DefinitionFiles;

			static CVector<CString> s_DatafileDirectories;
			static CVector<CString> s_DatafileDirectoriesSource;
#pragma warning(default:4251)

			static bool s_DatafilesUse;
		};

	public:
		class SECORE_API CUI
		{
		public:
			static bool Load(_IN SCFXML::CXMLDocument& rDocument);

		public:
			static const Float3& MouseSensitivityGUI()   { return s_Mouse.SensitivityGUI; }
			static const Float3& MouseSensitivity3DFPS() { return s_Mouse.Sensitivity3DFPS; }

		private:
			struct SMouse
			{
				Float3 SensitivityGUI;
				Float3 Sensitivity3DFPS;

			} static s_Mouse;
		};

	public:
		static const CString& StartupGUIModule() { return s_StartupGUIModule; }
		static const CString& StartupScene()     { return s_StartupScene; }

	private:
		static CString s_StartupGUIModule;
		static CString s_StartupScene;
		
	private:
		CSettings();
		~CSettings();
	};
};