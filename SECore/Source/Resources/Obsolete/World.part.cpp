bool CManager::ObjectsCreate(_IN CString& rFile)
{
	CEventLog::BlockNew(STRING("Loading resources"));
	{
		//CDFFile* pFile = File(rFile);
		//if (pFile)
		//{
			CEventLog::EventNew(STRING("Loading: ") + rFile);

			CXMLDocumentFile Source(*(new CFile(rFile)));
			Source.ResolveIncludes();

			CXMLNode* pWorld = Source.Root()->ChildNamed(STRING("world"));
			if (pWorld) { BuildWorld (*pWorld); }
			
			//CXMLNode* pScenes = Source.Root()->ChildNamed(STRING("scenes"));
			//if (pScenes) { BuildScenes(*pScenes); }
		//}
	}
	CEventLog::BlockClose();

	return TRUE;
}

void CManager::BuildWorld(_IN CXMLNode& rNode)
{
	CXMLAttribute* pFile = rNode.AttributeNamed(STRING("file"));

	if (pFile) { CWorld::ObjectsCreate(pFile->ValueString()); }
}

/*void CManager::BuildScenes(_IN CXMLNode& rNode)
{
	CXMLEnumerator Enumerator(rNode);

	while (Enumerator.Next())
	{
		if (Enumerator.Current()->Name().AsString() == STRING("scene"))
		{
			BuildScene(*Enumerator.Current());
		}
	}
}*/

void CManager::BuildScene(_IN CXMLNode& rNode)
{
	//Skip unlabeled nodes
	if (!rNode.AttributeFirst()) { return; }

 	CScene* pScene = new CScene();
	s_Scenes.AtPut(rNode.IDString(), *pScene);

	CXMLAttribute* pFile = rNode.AttributeNamed(STRING("file"));

	if (pFile) { pScene->ObjectsAdd(pFile->ValueString()); }
}

void CManager::ObjectsDelete()
{
	DeleteScenes();

	CWorld::ObjectsDelete();
}

void CManager::DeleteScenes()
{
	CEventLog::BlockNew(STRING("Deleting all scenes"));
	{
		CEnumeratorDictionaryString SceneEnumerator(s_Scenes);
		while (SceneEnumerator.Next()) 
		{
			CEventLog::EventNew(STRING("Deleting: ") + s_Scenes.ObjectToName(*SceneEnumerator.Current()));
			SceneEnumerator.CurrentShallowDelete();
		}
		s_Scenes.AllRemove();
	}
	CEventLog::BlockClose();
}