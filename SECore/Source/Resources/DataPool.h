#pragma once

#include "../SECore.h"

namespace Resources
{
	class SECORE_API CDataPool
	{
	public:
		//Initialize data-pool in either file-system or datafile mode 
		static bool Initialize();

	public:
		//These are the only functions which work directly with the FS, 
		//others use [SCFDatafileIO.dll]
		static bool DatafileAdd(_IN CString& rFile);

	public:
		static bool Rebuild();

	public:
		static bool ParsePathFile     (_IN CString& rPath, _OUT CString* pOutPath, _OUT CString* pOutName, _OUT CString* pOutExtension);
		static bool ParsePathDirectory(_IN CString& rPath, _OUT CString* pOutPath, _OUT CString* pOutName);

		static IStreamRead* FileOpen (_IN CString& rFilePath);
		static void         FileClose(_INOUT IStreamRead* pStream);

	private:
		CDataPool();
		~CDataPool();
	};
};