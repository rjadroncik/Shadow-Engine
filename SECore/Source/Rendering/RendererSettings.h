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
		static ENUM CodePath();
		static void CodePath(_IN ENUM eValue);

	public:
		static UINT MaxModels();
		static void MaxModels(_IN UINT uiValue);

		static UINT MaxLights();
		static void MaxLights(_IN UINT uiValue);

		static UINT MaxShadowMapResolution();
		static void MaxShadowMapResolution(_IN UINT uiValue);

	private:
		CRendererSettings();
		~CRendererSettings();
	};
};