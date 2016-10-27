#include "EventLog.h"

#include <SCFXML.h>

using namespace System;
using namespace SCFXML;

CString EventLog_Filename;
CStack<CString>  EventLog_BlockStack;

CStreamFileWrite* EventLog_pStreamFile = NULL;
CStreamWriteText* EventLog_pStream     = NULL;

bool EventLog_bEnabled = TRUE;

inline void CEventLog::Enabled(_IN bool bEnabled) { EventLog_bEnabled = bEnabled; }
inline bool CEventLog::Enabled()                  { return EventLog_bEnabled; }

inline CString CEventLog::Filename() { return EventLog_Filename; }

void CEventLog::Flush()
{
	EventLog_pStreamFile->Commit();
}

void CEventLog::Finish()
{
	BlockClose();
	Flush();
}

void CEventLog::EventNew(_IN CString& rString)
{
	if (EventLog_bEnabled)
	{
		for (UINT i = 0; i < EventLog_BlockStack.Size(); i++) { EventLog_pStream->PutString(SCFTEXT("  "), 2); }

		EventLog_pStream->PutLine(rString);

		EventLog_pStreamFile->Commit();
	}
}

void CEventLog::EventContinue(_IN CString& rString)
{
	if (EventLog_bEnabled)
	{
		for (UINT i = 0; i < EventLog_BlockStack.Size(); i++) { EventLog_pStream->PutString(SCFTEXT("  "), 2); }

		EventLog_pStream->PutLine(rString);
		
		EventLog_pStreamFile->Commit();
	}
}

void CEventLog::WarningNew(_IN CString& rString)
{
	if (EventLog_bEnabled)
	{
		for (UINT i = 0; i < EventLog_BlockStack.Size(); i++) { EventLog_pStream->PutString(SCFTEXT("  "), 2); }

		EventLog_pStream->PutString(SCFTEXT("Warning: "), 9);
		EventLog_pStream->PutLine(rString);
		
		EventLog_pStreamFile->Commit();
	}
}

void CEventLog::ErrorNew(_IN CString& rString)
{
	if (EventLog_bEnabled)
	{
		for (UINT i = 0; i < EventLog_BlockStack.Size(); i++) { EventLog_pStream->PutString(SCFTEXT("  "), 2); }

		EventLog_pStream->PutString(SCFTEXT("Error: "), 7);
		EventLog_pStream->PutLine(rString);

		EventLog_pStreamFile->Commit();
	}
}

//Transforms the string so that it is a valid XML node name
void NameShape(_INOUT CString& rString)
{
	register TCHAR* pChars  = rString.Value();
	register UINT   uiChars = rString.Length();

	for (UINT i = 0; i < uiChars; i++)
	{
		for (UINT j = 0; j < WHITESPACE_CHARS_COUNT; j++)
		{
			if (pChars[i] == WHITESPACE_CHARS[j])
			{
				pChars[i] = '_';
				break;
			}
		}
	}
}

void CEventLog::BlockNew(_IN CString& rString)
{
	if (EventLog_bEnabled)
	{
		//EventLog_pStream->PutLine();

		for (UINT i = 0; i < EventLog_BlockStack.Size(); i++) { EventLog_pStream->PutString(SCFTEXT("  "), 2); }

		CString* pStringTransformed = new CString(rString);
		NameShape(*pStringTransformed);

		EventLog_pStream->PutString(SCFTEXT("<"), 1);
		EventLog_pStream->PutString(*pStringTransformed);
		EventLog_pStream->PutString(SCFTEXT(">"), 1);
		EventLog_pStream->PutLine();

		//EventLog_pStream->PutLine();
		
		EventLog_pStreamFile->Commit();

		EventLog_BlockStack.Push(*pStringTransformed);
	}
}

void CEventLog::BlockClose()
{
	if (EventLog_bEnabled)
	{
		//EventLog_pStream->PutLine();

		for (UINT i = 0; i < (EventLog_BlockStack.Size() - 1); i++) { EventLog_pStream->PutString(SCFTEXT("  "), 2); }

		EventLog_pStream->PutString(SCFTEXT("</"), 2);
		EventLog_pStream->PutString(*EventLog_BlockStack.Top());
		EventLog_pStream->PutString(SCFTEXT(">"), 1);
		EventLog_pStream->PutLine();

		//EventLog_pStream->PutLine();
		
		EventLog_pStreamFile->Commit();

		EventLog_BlockStack.PopAndDelete();
	}
}

void CEventLog::Filename(_IN CString& rFilename)
{
	EventLog_Filename = rFilename;

	if (EventLog_pStream)     { delete EventLog_pStream; }
	if (EventLog_pStreamFile) { delete EventLog_pStreamFile; }

	EventLog_pStreamFile = new CStreamFileWrite(EventLog_Filename, false);
	EventLog_pStream     = new CStreamWriteTextUCS2(*EventLog_pStreamFile);

	EventLog_pStream->PutLine(SCFTEXT("<?xml version='1.0' encoding='UTF-16' standalone='yes'?>"));

	BlockNew(SCFTEXT("Event log"));
}
