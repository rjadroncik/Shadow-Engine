#pragma once

#ifdef SECORE_EXPORTS
#define SECORE_API __declspec(dllexport)
#else
#define SECORE_API __declspec(dllimport)
#endif

#include <SCFObjectExtensions.h>
#include <SCFScripting.h>
#include <SCFXML.h>

#include "../Errors.h"

namespace System
{
	class SECORE_API CSEObject : public SCFXML::CXMLObjectSerializable
	{
	public:
		void    ID(_IN CString& rID) _SET { m_ID = rID; }
		CString ID()                 _GET { return m_ID; }

	protected:
		CSEObject() {};
		virtual ~CSEObject() {};

	protected:
		CString m_ID;
	};
};
