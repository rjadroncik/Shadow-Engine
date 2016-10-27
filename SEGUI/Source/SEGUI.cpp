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
using namespace System;

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

bool SEGUI_API __fastcall GUI::Initialize(_IN void* hModule)
{
    CLASS_XMLSERIALIZABLE_REGISTER("Resources:Font",   Resources::CFont,   (HMODULE)hModule);
    CLASS_XMLSERIALIZABLE_REGISTER("Resources:Cursor", Resources::CCursor, (HMODULE)hModule);
    
    CLASS_XMLSERIALIZABLE_REGISTER("GUI:Label",       GUI::CLabel,       (HMODULE)hModule);
    CLASS_XMLSERIALIZABLE_REGISTER("GUI:EditField",   GUI::CEditField,   (HMODULE)hModule);
    CLASS_XMLSERIALIZABLE_REGISTER("GUI:LayoutSplit", GUI::CLayoutSplit, (HMODULE)hModule);
    CLASS_XMLSERIALIZABLE_REGISTER("GUI:LayoutTable", GUI::CLayoutTable, (HMODULE)hModule);
    CLASS_XMLSERIALIZABLE_REGISTER("GUI:Window",      GUI::CWindow,      (HMODULE)hModule);
    CLASS_XMLSERIALIZABLE_REGISTER("GUI:Module",      GUI::CModule,      (HMODULE)hModule);

    CNamespace& rGUI = CREATE_NAMESPACE(GUI);
    CNamespace& rResources = CManager::Namespace(Namespace::Resources);

    CManager::NamespaceAdd(rGUI);

    CManager::CategoryAdd(CREATE_CATEGORY(Fonts, rResources));
    CManager::CategoryAdd(CREATE_CATEGORY(Cursors, rResources));

    CManager::CategoryAdd(CREATE_CATEGORY(Controls, rGUI));
    CManager::CategoryAdd(CREATE_CATEGORY(Layouts, rGUI));
    CManager::CategoryAdd(CREATE_CATEGORY(Windows, rGUI));
    CManager::CategoryAdd(CREATE_CATEGORY(GUIModules, rGUI));

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
