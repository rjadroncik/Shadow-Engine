#pragma once

#include "SEObject.h"

namespace System
{
	class SECORE_API CScripting
	{
	public:
		static void __fastcall Initialize(_IN void* hModule);

	public:
		static inline void     Object(_IN CString& rName, _IN CObject& rObject) { s_Objects.AtPut(rName, rObject); }
		static inline CObject* Object(_IN CString& rName)                       { return s_Objects.At(rName); }

	public:
		static inline CEnumeratorDictionaryString& Objects() { return *(new CEnumeratorDictionaryString(s_Objects)); }
	
	private:
		#pragma warning(disable:4251)
		static CDictionaryString<CObject> s_Objects;
		#pragma warning(default:4251)

	private:
		CScripting();
		~CScripting();
	};
};
