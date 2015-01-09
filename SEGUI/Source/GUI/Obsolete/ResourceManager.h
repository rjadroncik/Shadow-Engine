#pragma once

#include "../GUI.h"
#include "Cache.h"
#include "Module.h"

namespace GUI
{
	class GUI_API CResourceManager
	{
	public:
		//Create or delete ALL objects that are defined in the specified files
		static bool ObjectsCreate(_IN CString& rFile);
		static bool ObjectsDelete(_IN CString& rFile);

		//Removes all objects that are NOT defined in the input file
		static bool ObjectsLeaveOnly(_IN CString& rFile);

		//Deletes all objects
		static void ObjectsDelete();

	public:
		static inline CModule* GUIModule(_IN CString& rName) { return (CModule*)s_GUIModules.NameToObject(rName); }

	private:
		static void BuildGUI       (_IN CXMLNode& rNode);
		static void BuildGUIModules(_IN CXMLNode& rNode);
		static void BuildGUIModule (_IN CXMLNode& rNode);

	private:
		static CDictionaryString s_GUIModules;

	private:
		CResourceManager();
		~CResourceManager();
	};
};