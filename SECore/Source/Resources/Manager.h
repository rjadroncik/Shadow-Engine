#pragma once

#include "../SECore.h"
#include "../System/SEObject.h"
#include "../Rendering/Scene.h"
#include "World.h"

#include <SCFXML.h>

namespace Resources
{
	class SECORE_API CManager
	{
	public:
		//Initializations
		static bool __fastcall Initialize();

	public:
		static System::CNamespace& __fastcall Namespace(_IN System::Namespace eNamespace);
		
		static void __fastcall NamespaceAdd(_IN _REF System::CNamespace& rNamespace);
		static void __fastcall CategoryAdd(_IN _REF System::CCategory& rCategory);

	public:
		//Load objects that are defined in the specified file OR save existing objects
		static bool __fastcall ObjectsLoad(_IN CString& rFile);
		static bool __fastcall ObjectsSave(_IN CString& rDirectory);

		//Delete objects that are OR are not defined in the input file
		static bool __fastcall ObjectsDelete      (_IN CString& rFile);
		static bool __fastcall ObjectsDeleteAllBut(_IN CString& rFile);

		//Deletes all objects
		static void __fastcall ObjectsDelete();

	private:
		static void __fastcall ObjectsLoad(_IN CXMLNode& rNode, _IN System::CCategory& rCategory);
		static void __fastcall ObjectsSave(_OUT CXMLStreamWriteObject& rStream, _IN System::CCategory& rCategory);

		static void __fastcall ObjectsDelete(_IN System::CCategory& rCategory);

	private:
		static bool __fastcall ResolveIncludes(_INOUT CXMLDocument& rDocument, _IN CString& rPath);
	
	private:
		CManager();
		~CManager();
	};
};