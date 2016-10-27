#include "Manager.h"

#include <SCFXML.h>

#include "DataPool.h"

#include "../Simulation/ParticleSystem.h"
#include "../System/Scripting.h"

using namespace SCFXML;

using namespace Resources;
using namespace System;

CDictionaryInt64<CCategory> Manager_Categories;
CDictionaryInt64<CNamespace> Manager_Namespaces;

CNamespace& __fastcall CManager::Namespace(_IN System::Namespace eNamespace)
{
	return *Manager_Namespaces.At(eNamespace);
}

void __fastcall CManager::NamespaceAdd(_IN _REF CNamespace& rNamespace)
{
	Manager_Namespaces.AtPut(rNamespace.Value(), rNamespace);
}

void __fastcall CManager::CategoryAdd(_IN _REF CCategory& rCategory)
{
	Manager_Categories.AtPut(rCategory.Value(), rCategory);
}

bool __fastcall CManager::Initialize()
{
	CNamespace& rResources  = CREATE_NAMESPACE(Resources);
	CNamespace& rRendering  = CREATE_NAMESPACE(Rendering);
	CNamespace& rSimulation = CREATE_NAMESPACE(Simulation);

	NamespaceAdd(rResources);
	NamespaceAdd(rRendering);
	NamespaceAdd(rSimulation);

	CategoryAdd(CREATE_CATEGORY(Textures,    rResources));
	CategoryAdd(CREATE_CATEGORY(GPUPrograms, rResources));
	CategoryAdd(CREATE_CATEGORY(Meshes,      rResources));

	CategoryAdd(CREATE_CATEGORY(Materials, rRendering));
	CategoryAdd(CREATE_CATEGORY(Models,    rRendering));
	CategoryAdd(CREATE_CATEGORY(Lights,    rRendering));
	CategoryAdd(CREATE_CATEGORY(Cameras,   rRendering));
	CategoryAdd(CREATE_CATEGORY(Scenes,    rRendering));

	CategoryAdd(CREATE_CATEGORY(ParticleSystems, rSimulation));

	return TRUE;
}

bool __fastcall CManager::ResolveIncludes(_INOUT CXMLDocument& rDocument, _IN CString& rPath)
{
	CXMLElement* pInclude = NULL;

	while ((pInclude = rDocument.RootElement()->DescendantsNamed(STRING("include"))) != 0)
	{
		CXMLAttribute* pFile = pInclude->AttributeNamed(STRING("file"));
		if (!pFile) { return FALSE; }

		IStreamRead* pReadStream = CDataPool::FileOpen(rPath + pFile->ValueString());
		if (pReadStream)
		{
			CStreamReadTextGeneric ReadStreamText(*pReadStream);
			CXMLDocument* pDocument = new CXMLDocument(ReadStreamText);

			if (pDocument)
			{
				pInclude->ReplaceWith(*pDocument->RootElementRetrieve());

				delete pInclude;
				delete pDocument;

// 				pInclude->ReplaceWith(*pDocument->RootElement());
// 				delete pInclude;
// 
// 				pDocument->Root(NULL);
// 				delete pDocument;
			}
			else { CDataPool::FileClose(pReadStream); return FALSE; }
		
			CDataPool::FileClose(pReadStream);
		}
		else { return FALSE; }
	}

	return TRUE;
}

bool __fastcall CManager::ObjectsLoad(_IN CString& rFile)
{
	//Query file from data pool
	IStreamRead* pReadStream = CDataPool::FileOpen(rFile);
	if (pReadStream)
	{
		CEventLog::BlockNew(STRING("Creating objects"));
		{
			CEventLog::EventNew(STRING("Loading: ") + rFile);

			//Extract the path to the file
			CString FilePath;
			CDataPool::ParsePathFile(rFile, &FilePath, NULL, NULL);

			CStreamReadTextGeneric ReadStreamText(*pReadStream);
			CXMLDocument Source(ReadStreamText);
		
			ResolveIncludes(Source, FilePath);

			_PENDING;
			{
				//For debugging only
				CStreamFileWrite FWS(STRING("ComposedSource.xml"));
				CStreamWriteTextUTF8 FWTS(FWS);

				Source.Write(FWTS);
			}

			CEnumeratorDictionaryInt64<CCategory> categories = CEnumeratorDictionaryInt64<CCategory>(Manager_Categories);
			while (categories.Next())
			{
				CXMLNode* pNode = Source.RootElement()->ChildNamed(categories.Current()->Name());
				if (pNode) 
				{ 
					ObjectsLoad(*pNode, *categories.Current());
				}
			}
		}
		CEventLog::BlockClose();

		CDataPool::FileClose(pReadStream);
		return TRUE;
	}

	return FALSE;
}

bool  __fastcall CManager::ObjectsSave(_IN CString& rDirectory)
{
	//CDirectory directory(rDirectory);
	//if (!directory.Exists()) { directory.Create(); }

	//for (UINT i = Manager_ObjectTypes.Size(); i > 0; i--)
	//{
	//	CString*   pCategory  = (CString*)  &((CAssociation<CString, CArrayInt>&)Manager_ObjectTypes[i - 1]).Key();
	//	CArrayInt* pClassKeys = (CArrayInt*)&((CAssociation<CString, CArrayInt>&)Manager_ObjectTypes[i - 1]).Value();
	//	CString*   pNamespace = Manager_ObjectTypeNamespaces.At(*pCategory);

	//	CFile* pFile = new CFile(rDirectory, *pCategory, STRING("xml"));
	//	if (pFile->Exists()) { pFile->Delete(); }

	//	CXMLDocumentFile Document(*pFile);
	//	CXMLStreamWrite Writer(Document, *pCategory, *pNamespace, STRING("ProjectShadow.") + *pNamespace);

	//	CXMLStreamWriteObject WriterObject(Document);
	//	ObjectsSave(WriterObject, *pClassKeys);

	//	Document.Write();
	//}

	return TRUE;
}

void __fastcall CManager::ObjectsLoad(_IN CXMLNode& rNode, _IN CCategory& rCategory)
{
	CEventLog::BlockNew(rCategory.Name());
	{
		CXMLStreamReadObject Reader(rNode, TRUE);

		CSEObject* pObject = NULL;
		CString* pID = NULL;

		while (Reader.Next(&pID))
		{
			CEventLog::BlockNew(*pID);

			pObject = (CSEObject*)Reader.Current();
			pObject->ID(*pID);

			CScripting::Object(*pID, *pObject);
			
			CEventLog::BlockClose();
		}
	}
	CEventLog::BlockClose();
}

void __fastcall CManager::ObjectsSave(_OUT CXMLStreamWriteObject& rStream, _IN CCategory& rCategory)
{
	CEnumeratorDictionaryString<CSEObject>* pEnumerator = CScripting::EnumarateObjects();

	while (pEnumerator->Next()) 
	{
		if (pEnumerator->Current()->Category() == rCategory.Value())
		{
			rStream.Next((CXMLObjectSerializable*)pEnumerator->Current());
			break;
		}
	}

	delete pEnumerator;
}

void __fastcall CManager::ObjectsDelete()
{
	CEventLog::BlockNew(STRING("Deleting objects"));
	{
		CEnumeratorDictionaryInt64<CCategory> categories = CEnumeratorDictionaryInt64<CCategory>(Manager_Categories);
		while (categories.Next())
		{
			ObjectsDelete(*categories.Current());
		}
	}
	CEventLog::BlockClose();
}

void __fastcall CManager::ObjectsDelete(_IN CCategory& rCategory)
{
	CEventLog::BlockNew(rCategory.Name());
	{
		CEnumeratorDictionaryString<CSEObject>* pEnumerator = CScripting::EnumarateObjects();

		while (pEnumerator->Next()) 
		{
			if (pEnumerator->Current()->Category() == rCategory.Value())
			{
				CEventLog::EventNew(pEnumerator->CurrentPath());
				pEnumerator->CurrentShallowDelete();
				break;
			}
		}

		delete pEnumerator;
	}
	CEventLog::BlockClose();
}

