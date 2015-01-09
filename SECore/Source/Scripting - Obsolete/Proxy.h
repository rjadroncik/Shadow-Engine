#pragma once

#ifdef SECORE_EXPORTS
#define SECORE_API __declspec(dllexport)
#else
#define SECORE_API __declspec(dllimport)
#endif

#include <SCFObjectExtensions.h>
#include <SCFScripting.h>
#include <SCFScriptingXML.h>

#include "../Classes.h"
#include "../Errors.h"

#define GETVALUE if ((pValue = rReader.GetValue()) != NULL)
#define PUTVALUE(name, value) rWriter.PutValue(STRING(name), (value))
#define PUTVALUENEW(name, value, class) rWriter.PutValue(STRING(name), *(new class(value)))

namespace Scripting
{
	class SECORE_API CProxy : public SCFXML::CXMLObjectSerializable
	{
	public:
		static void Register();

	public:
		void     IDSet(_IN CString& rID) _SET { m_ID = rID; }
		CString* ID()                    _GET { return (new CString(m_ID, false)); }

	public:
		virtual CObject& Value() = 0;

	protected:
		CProxy() {};
		virtual ~CProxy() {};

	protected:
		CString m_ID;
	};
};
