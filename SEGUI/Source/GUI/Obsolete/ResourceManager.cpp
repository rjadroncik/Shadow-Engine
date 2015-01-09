#include "ResourceManager.h"

using namespace GUI;
using namespace System;

CDictionaryString CResourceManager::s_GUIModules;

bool CResourceManager::ObjectsCreate(_IN CString& rFile)
{
	CEventLog::BlockNew(CString(STRING("Loading resources")));
	{
		CEventLog::EventNew(CString(STRING("Loading: ")) + CFile(rFile).PathFull());

		CXMLDocumentFile Source(*(new CFile(rFile)));
		Source.ResolveIncludes();
		
		CXMLNode* pGUI        = Source.Root()->ChildNamed(CString(STRING("GUI")));
		CXMLNode* pGUIModules = Source.Root()->ChildNamed(CString(STRING("GUIModules")));

		if (pGUI)        { BuildGUI       (*pGUI); }
		if (pGUIModules) { BuildGUIModules(*pGUIModules); }
	}
	CEventLog::BlockClose();

	return TRUE;
}

void CResourceManager::BuildGUI(_IN CXMLNode& rNode)
{
	CXMLAttribute* pFile = rNode.AttributeNamed(CString(STRING("file")));

	if (pFile) { CCache::ObjectsCreate(pFile->ValueString()); }
}

void CResourceManager::BuildGUIModules(_IN CXMLNode& rNode)
{
	CXMLEnumerator Enumerator(rNode);

	while (Enumerator.Next())
	{
		if (Enumerator.Current()->Name().AsString() == CString(STRING("GUIModule")))
		{
			BuildGUIModule(*Enumerator.Current());
		}
	}
}

void CResourceManager::BuildGUIModule(_IN CXMLNode& rNode)
{
	//Skip unlabeled nodes
	if (!rNode.AttributeFirst()) { return; }

	CModule* pGUIModule = new CModule(rNode.IDString());
	s_GUIModules.AtPut(pGUIModule->IDString(), *pGUIModule);

	CXMLAttribute* pFile = rNode.AttributeNamed(CString(STRING("file")));

	if (pFile) { pGUIModule->ObjectsAdd(pFile->ValueString()); }
}

void CResourceManager::ObjectsDelete()
{
	CCache::ObjectsDelete();
}
