#include "SEGUI.h"
#include "GUI/Dispatcher.h"

#include "Resources/Font.h"
#include "Resources/Cursor.h"

#include "GUI/Label.h"
#include "GUI/EditField.h"
#include "GUI/LayoutSplit.h"
#include "GUI/LayoutTable.h"
#include "GUI/Window.h"
#include "GUI/Module.h"

#include "../../SECore/Source/Resources/Manager.h"

using namespace GUI;
using namespace Resources;

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

bool SEGUI_API __fastcall GUI::Initialize(_IN void* hModule)
{
	CLASS_XMLSERIALIZABLE_REGISTER(ClassFont,   Resources::CFont,   (HMODULE)hModule);
	CLASS_XMLSERIALIZABLE_REGISTER(ClassCursor, Resources::CCursor, (HMODULE)hModule);
	
	CLASS_XMLSERIALIZABLE_REGISTER(ClassLabel,       GUI::CLabel,       (HMODULE)hModule);
	CLASS_XMLSERIALIZABLE_REGISTER(ClassEditField,   GUI::CEditField,   (HMODULE)hModule);
	CLASS_XMLSERIALIZABLE_REGISTER(ClassLayoutSplit, GUI::CLayoutSplit, (HMODULE)hModule);
	CLASS_XMLSERIALIZABLE_REGISTER(ClassLayoutTable, GUI::CLayoutTable, (HMODULE)hModule);
	CLASS_XMLSERIALIZABLE_REGISTER(ClassWindow,      GUI::CWindow,      (HMODULE)hModule);
	CLASS_XMLSERIALIZABLE_REGISTER(ClassModule,      GUI::CModule,      (HMODULE)hModule);

	{
		CArrayInt* pClassKeys = new CArrayInt(1);
		pClassKeys->AtPut(0, ClassFont);

		CManager::ObjectTypesAdd(STRING("fonts"), STRING("Resources"), *pClassKeys);
	}
	{
		CArrayInt* pClassKeys = new CArrayInt(1);
		pClassKeys->AtPut(0, ClassCursor);

		CManager::ObjectTypesAdd(STRING("cursors"), STRING("Resources"), *pClassKeys);
	}
	{
		CArrayInt* pClassKeys = new CArrayInt(2);
		pClassKeys->AtPut(0, ClassLabel);
		pClassKeys->AtPut(1, ClassEditField);

		CManager::ObjectTypesAdd(STRING("controls"), STRING("GUI"), *pClassKeys);
	}
	{
		CArrayInt* pClassKeys = new CArrayInt(2);
		pClassKeys->AtPut(0, ClassLayoutSplit);
		pClassKeys->AtPut(1, ClassLayoutTable);

		CManager::ObjectTypesAdd(STRING("layouts"), STRING("GUI"), *pClassKeys);
	}
	{
		CArrayInt* pClassKeys = new CArrayInt(1);
		pClassKeys->AtPut(0, ClassWindow);

		CManager::ObjectTypesAdd(STRING("windows"), STRING("GUI"), *pClassKeys);
	}
	{
		CArrayInt* pClassKeys = new CArrayInt(1);
		pClassKeys->AtPut(0, ClassModule);

		CManager::ObjectTypesAdd(STRING("GUIModules"), STRING("GUI"), *pClassKeys);
	}

	CXMLEnumeration* pEnumeration = new SCFXML::CXMLEnumeration();
	{
		pEnumeration->ValueDefine(STRING("center"), AlignCenter);
		pEnumeration->ValueDefine(STRING("left"),   AlignLeft );
		pEnumeration->ValueDefine(STRING("right"),  AlignRight);
		pEnumeration->ValueDefine(STRING("top"),    AlignTop );
		pEnumeration->ValueDefine(STRING("bottom"), AlignBottom);
	}
	CXMLEnumeration::Register(STRING("TextAlignments"), *pEnumeration);

	pEnumeration = new SCFXML::CXMLEnumeration();
	{
		pEnumeration->ValueDefine(STRING("horizontal"),            LS_HORIZONTAL );
		pEnumeration->ValueDefine(STRING("horizontalFixedTop"),    LS_HORIZONTAL | LS_FIXED_1ST);
		pEnumeration->ValueDefine(STRING("horizontalFixedBottom"), LS_HORIZONTAL | LS_FIXED_2ND);
		pEnumeration->ValueDefine(STRING("vertical"),              LS_VERTICAL );
		pEnumeration->ValueDefine(STRING("verticalFixedTop"),      LS_VERTICAL | LS_FIXED_1ST);
		pEnumeration->ValueDefine(STRING("verticalFixedBottom"),   LS_VERTICAL | LS_FIXED_2ND);
	}
	CXMLEnumeration::Register(STRING("LayoutSplitTypes"), *pEnumeration);

	pEnumeration = new SCFXML::CXMLEnumeration();
	{
		pEnumeration->ValueDefine(STRING("Cursor.Arrow"), CursorArrow);
		pEnumeration->ValueDefine(STRING("Cursor.Beam"), CursorBeam);
		pEnumeration->ValueDefine(STRING("Cursor.Busy"), CursorBusy);
		pEnumeration->ValueDefine(STRING("Cursor.Cross"), CursorCross);
		pEnumeration->ValueDefine(STRING("Cursor.Hand"), CursorHand);
		pEnumeration->ValueDefine(STRING("Cursor.Help"), CursorHelp);
		pEnumeration->ValueDefine(STRING("Cursor.Move"), CursorMove);
		pEnumeration->ValueDefine(STRING("Cursor.No"), CursorNo);

		pEnumeration->ValueDefine(STRING("Cursor.Size1"), CursorSizeNESW);
		pEnumeration->ValueDefine(STRING("Cursor.Size2"), CursorSizeNWSE);
		pEnumeration->ValueDefine(STRING("Cursor.Size3"), CursorSizeWE);
		pEnumeration->ValueDefine(STRING("Cursor.Size4"), CursorSizeNS);

		pEnumeration->ValueDefine(STRING("Cursor.Wait"), CursorWait);
	}
	CXMLEnumeration::Register(STRING("CursorTypes"), *pEnumeration);

	CDispatcher::Initialize();

	return TRUE;
}

bool APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	SCF_UNREFERENCED_PARAMETER(hModule);
	SCF_UNREFERENCED_PARAMETER(lpReserved);

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}
