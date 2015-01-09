#pragma once

#include "../SECore.h"

#include <SCFXML.h>

namespace Rendering
{
	enum CodePaths
	{
		CodePathOpenGL20DGC = 1,
		CodePathOpenGL20IGP,
	};

	class SECORE_API CRendererSettings
	{
	public:
		static bool Load(_IN SCFXML::CXMLDocument& rDocument);

	public:
		static SCF::ENUM CodePath();
		static void      CodePath(_IN SCF::ENUM eValue);

	public:
		static SCF::UINT MaxModels();
		static void      MaxModels(_IN SCF::UINT uiValue);

		static SCF::UINT MaxLights();
		static void      MaxLights(_IN SCF::UINT uiValue);

		static SCF::UINT MaxShadowMapResolution();
		static void      MaxShadowMapResolution(_IN SCF::UINT uiValue);

	private:
		CRendererSettings();
		~CRendererSettings();
	};
};