#pragma once

#include "SEObject.h"

namespace System
{
	template class __declspec(dllexport) SCFBase::CDictionaryString<System::CSEObject>;

	class SECORE_API CScripting
	{
	public:
		static void __fastcall Initialize(_IN void* hModule);

	public:
		static inline void       Object(_IN CString& rName, _IN CSEObject& rObject) { s_Objects.AtPut(rName, rObject); }
		static inline CSEObject* Object(_IN CString& rName)                         { return s_Objects.At(rName); }

	public:
		static inline CEnumeratorDictionaryString<CSEObject>* EnumarateObjects() { return new CEnumeratorDictionaryString<CSEObject>(s_Objects); }
	
	private:
		static CDictionaryString<CSEObject> s_Objects;

	private:
		CScripting();
		~CScripting();
	};
};
