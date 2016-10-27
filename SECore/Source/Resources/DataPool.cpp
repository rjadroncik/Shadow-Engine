#include "DataPool.h"

#include "../System/Settings.h"

#include <SCFDatafileIO.h>

using namespace Resources;
using namespace System;

//DatafileIO mode variables
SCFDatafileIO::CDataPool DataPool_Pool;

//Common variables (although usage is slightly different depending on mode)
CDictionaryInt64<CStreamFileRead> DataPool_FilesOpen;

//Used only in file native system mode (contains a mapping between 
//logical file names & their respective file system names)
CDictionaryString<CString> DataPool_Files;

bool CDataPool::DatafileAdd(_IN CString& rFile)
{
	if (CSettings::CResources::DatafilesUse())
	{
		return DataPool_Pool.DatafileAdd(*(new SCFDatafileIO::CDatafile(rFile, TRUE)));
	}
	else { return FALSE; }
}

bool CDataPool::Rebuild()
{
	if (CSettings::CResources::DatafilesUse())
	{
		return DataPool_Pool.Rebuild(TRUE);
	}
	else
	{
		DataPool_FilesOpen.AllDelete();
		DataPool_Files.AllDelete();

		for (UINT i = 0; i < CSettings::CResources::DatafileDirectoriesSource().Size(); i++)
		{
			CDirectory SDFDirectory((CString&)(CSettings::CResources::DatafileDirectoriesSource()[i]));

			CVector<CString> SDFContentDirectories;
			SDFDirectory.Read(NULL, &SDFContentDirectories);

			for (UINT k = 0; k < SDFContentDirectories.Size(); k++)
			{
				CDirectory SDFContentDirectory(SDFDirectory.PathFull() + (CString&)SDFContentDirectories[k] + STRING("\\"));

				CVector<CString> Files;
				SDFContentDirectory.ListContents(&Files, NULL);

				UINT uiPathLength = SDFContentDirectory.PathFull().Length() - 1;

				for (UINT j = 0; j < Files.Size(); j++)
				{
					DataPool_Files.AtPut(CStringRange((CString&)Files[j], uiPathLength), (CString&)Files[j]);
				}

				//Files.AllDelete(); //Don't delete ... strings are directly stored in dictionary
			}

			SDFContentDirectories.AllDelete();
		}

		return TRUE;
	}
}

bool CDataPool::Initialize()
{
	if (CSettings::CResources::DatafilesUse())
	{
		for (UINT i = 0; i < CSettings::CResources::DatafileDirectories().Size(); i++)
		{
			CDirectory Directory((CString&)(CSettings::CResources::DatafileDirectories()[i]));

			CVector<CString> Files;
			Directory.ListContents(&Files, NULL);

			for (UINT j = 0; j < Files.Size(); j++)
			{
				Resources::CDataPool::DatafileAdd((CString&)Files[j]);
			}

			Files.AllDelete();
		}
	}

	return Rebuild();
}


IStreamRead* CDataPool::FileOpen(_IN CString& rFilePath)
{
	if (CSettings::CResources::DatafilesUse())
	{
		SCFDatafileIO::CDFFile* pDFFile = DataPool_Pool.File(rFilePath);
		if (pDFFile)
		{
			SCFDatafileIO::CDFStreamFileRead* pReadStream = new SCFDatafileIO::CDFStreamFileRead(*pDFFile);
			if (pReadStream)
			{
				DataPool_FilesOpen.AtPut((UINT64)((IStreamRead*)pReadStream), *pReadStream);

				delete pDFFile;
				return pReadStream;
			}

			delete pDFFile;
		}

		return NULL;
	}
	else
	{
		CString* pFilePath = (CString*)(DataPool_Files.At(rFilePath));
		if (pFilePath)
		{
			CStreamFileRead* pReadStream = new CStreamFileRead(*pFilePath);
			if (pReadStream)
			{
				DataPool_FilesOpen.AtPut((UINT64)((IStreamRead*)pReadStream), *pReadStream);

				return pReadStream;
			}
		}

		return NULL;
	}
}

void CDataPool::FileClose(_INOUT IStreamRead* pStream)
{
	CObject* pObject = DataPool_FilesOpen.Remove((UINT64)pStream);
	if (pObject)
	{
		delete pObject;
	}
}

bool CDataPool::ParsePathFile(_IN CString& rPath, _OUT CString* pOutPath, _OUT CString* pOutName, _OUT CString* pOutExtension)
{
	if (CSettings::CResources::DatafilesUse())
	{
		return DataPool_Pool.ParsePathFile(rPath, pOutPath, pOutName, pOutExtension);
	}
	else
	{
		CFile::ParsePath(rPath, pOutPath, pOutName, pOutExtension);
		return TRUE;
	}
}

bool CDataPool::ParsePathDirectory(_IN CString& rPath, _OUT CString* pOutPath, _OUT CString* pOutName)
{
	if (CSettings::CResources::DatafilesUse())
	{
		return DataPool_Pool.ParsePathDirectory(rPath, pOutPath, pOutName);
	}
	else
	{
		CDirectory::ParsePath(rPath, pOutPath, pOutName);
		return TRUE;
	}
}
