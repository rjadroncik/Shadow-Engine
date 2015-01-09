#include "Manager.h"

#include <SCFXML.h>

#include "DataPool.h"

#include "../Simulation/ParticleSystem.h"
#include "../System/Scripting.h"

using namespace SCFXML;

using namespace Resources;
using namespace System;

CVector<CAssociation> Manager_ObjectTypes;

bool __fastcall CManager::ObjectTypesAdd(_IN CString& rCategory, _IN _REF CArrayInt& rClassKeys) 
{
	 Manager_ObjectTypes.LastAdd(*(new CAssociation(rCategory, rClassKeys)));
	 return TRUE;
}

bool __fastcall CManager::Initialize()
{
	{
		CArrayInt* pClassKeys = new CArrayInt(1);
		pClassKeys->AtPut(0, ClassTexture);

		CManager::ObjectTypesAdd(*(new STRING("textures")), *pClassKeys);
	}
	{
		CArrayInt* pClassKeys = new CArrayInt(1);
		pClassKeys->AtPut(0, ClassGPUProgram);

		CManager::ObjectTypesAdd(*(new STRING("GPUPrograms")), *pClassKeys);
	}
	{
		CArrayInt* pClassKeys = new CArrayInt(1);
		pClassKeys->AtPut(0, ClassMaterial);

		CManager::ObjectTypesAdd(*(new STRING("materials")), *pClassKeys);
	}
	{
		CArrayInt* pClassKeys = new CArrayInt(1);
		pClassKeys->AtPut(0, ClassMesh);

		CManager::ObjectTypesAdd(*(new STRING("meshes")), *pClassKeys);
	}
	{
		CArrayInt* pClassKeys = new CArrayInt(1);
		pClassKeys->AtPut(0, ClassModel);

		CManager::ObjectTypesAdd(*(new STRING("models")), *pClassKeys);
	}
	{
		CArrayInt* pClassKeys = new CArrayInt(2);
		pClassKeys->AtPut(0, ClassLightSpot);
		pClassKeys->AtPut(1, ClassLightOmni);

		CManager::ObjectTypesAdd(*(new STRING("lights")), *pClassKeys);
	}
	{
		CArrayInt* pClassKeys = new CArrayInt(1);
		pClassKeys->AtPut(0, ClassCamera);

		CManager::ObjectTypesAdd(*(new STRING("cameras")), *pClassKeys);
	}
	{
		CArrayInt* pClassKeys = new CArrayInt(1);
		pClassKeys->AtPut(0, ClassParticleSystem);

		CManager::ObjectTypesAdd(*(new STRING("particleSystems")), *pClassKeys);
	}
	{
		CArrayInt* pClassKeys = new CArrayInt(1);
		pClassKeys->AtPut(0, ClassScene);

		CManager::ObjectTypesAdd(*(new STRING("scenes")), *pClassKeys);
	}

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

			for (SCF::UINT i = 0; i < Manager_ObjectTypes.Size(); i++)
			{
				CString* pCategory = (CString*)&((CAssociation&)Manager_ObjectTypes[i]).Key();

				CXMLNode* pNode = Source.RootElement()->ChildNamed(*pCategory);
				if (pNode) 
				{ 
					ObjectsLoad(*pNode, *pCategory);
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
	for (SCF::UINT i = Manager_ObjectTypes.Size(); i > 0; i--)
	{
		CString*   pCategory  = (CString*)  &((CAssociation&)Manager_ObjectTypes[i - 1]).Key();
		CArrayInt* pClassKeys = (CArrayInt*)&((CAssociation&)Manager_ObjectTypes[i - 1]).Value();

		CFile* pFile = new CFile(rDirectory, *pCategory, STRING("xml"));
		if (pFile->Exists()) { pFile->Delete(); }

		CXMLDocumentFile Document(*pFile);
		CXMLStreamWrite Writer(Document);
		Writer.BlockStart(*pCategory);

		CXMLStreamWriteObject WriterObject(Document);
		ObjectsSave(WriterObject, *pClassKeys);

		Document.Write();
	}

	return TRUE;
}

void __fastcall CManager::ObjectsLoad(_IN CXMLNode& rNode, _IN CString& rBlockText)
{
	CEventLog::BlockNew(rBlockText);
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

void __fastcall CManager::ObjectsSave(_OUT CXMLStreamWriteObject& rStream, _IN CArrayInt& rClassKeys)
{
	CEnumeratorDictionaryString* pEnumerator = &CScripting::Objects();

	while (pEnumerator->Next()) 
	{
		for (SCF::UINT i = 0; i < rClassKeys.Size(); i++)
		{ 
			if (pEnumerator->Current()->ClassKey() == (SCF::ENUM)rClassKeys[i])
			{
				rStream.Next((CXMLObjectSerializable*)pEnumerator->Current());
				break;
			}
		}
	}

	delete pEnumerator;
}

void __fastcall CManager::ObjectsDelete()
{
	CEventLog::BlockNew(STRING("Deleting objects"));
	{
		for (SCF::UINT i = Manager_ObjectTypes.Size(); i > 0; i--)
		{
			CString*   pCategory  = (CString*)  &((CAssociation&)Manager_ObjectTypes[i - 1]).Key();
			CArrayInt* pClassKeys = (CArrayInt*)&((CAssociation&)Manager_ObjectTypes[i - 1]).Value();

			ObjectsDelete(*pClassKeys, *pCategory);
		}
	}
	CEventLog::BlockClose();
}

void __fastcall CManager::ObjectsDelete(_IN CArrayInt& rClassKeys, _IN CString& rBlockText)
{
	CEventLog::BlockNew(rBlockText);
	{
		CEnumeratorDictionaryString* pEnumerator = &CScripting::Objects();

		while (pEnumerator->Next()) 
		{
			for (SCF::UINT i = 0; i < rClassKeys.Size(); i++)
			{ 
				if (pEnumerator->Current()->ClassKey() == (SCF::ENUM)rClassKeys[i])
				{
					CEventLog::EventNew(pEnumerator->CurrentPath());
					pEnumerator->CurrentShallowDelete();
					break;
				}
			}
		}

		delete pEnumerator;
	}
	CEventLog::BlockClose();
}

