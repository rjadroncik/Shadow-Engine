#pragma once

#include "../../../Core/Source/Core.h"
#include "../../../Core/Source/Resources/Cursor.h"

#include "Label.h"
#include "EditField.h"
#include "Window.h"
#include "Module.h"

#include "LayoutTable.h"
#include "LayoutSplit.h"

#include <SCFXML.h>

using namespace SCFXML;
using namespace Resources;

namespace GUI
{
	class GUI_API CCache
	{
	public:
		//Create or delete ALL objects that are defined in the specified files
		static bool ObjectsCreate(_IN CString& rFile);
		static bool ObjectsDelete(_IN CString& rFile);

		//Removes all objects that are NOT defined in the input file
		static bool ObjectsLeaveOnly(_IN CString& rFile);

		//Deletes all objects
		static void ObjectsDelete();

	public:
		static inline CWindow*    Window   (_IN CString& rName) { return (CWindow*)   s_Elements.NameToObject(STRING("Window.")    + rName); }
		static inline CEditField* EditField(_IN CString& rName) { return (CEditField*)s_Elements.NameToObject(STRING("EditField.") + rName); }
		static inline CLabel*     Label    (_IN CString& rName) { return (CLabel*)    s_Elements.NameToObject(STRING("Label.")     + rName); }
	
		static inline void Window   (_IN CWindow&    rWindow)    { s_Elements.AtPut(STRING("Window.")    + rWindow.IDString(),    rWindow); }
		static inline void EditField(_IN CEditField& rEditField) { s_Elements.AtPut(STRING("EditField.") + rEditField.IDString(), rEditField); }
		static inline void Label    (_IN CLabel&     rLabel)     { s_Elements.AtPut(STRING("Label.")     + rLabel.IDString(),     rLabel); }

	public:
		static inline CCursor*  Cursor (_IN CString& rName) { return (CCursor*)  s_Cursors.NameToObject(rName); }
		static inline CFont*    Font   (_IN CString& rName) { return (CFont*)      s_Fonts.NameToObject(rName); }
		static inline CElement* Element(_IN CString& rName) { return (CElement*)s_Elements.NameToObject(rName); }

	private:
		static void  ParseText          (_IN CXMLNode& rNode, _INOUT CText&           rText);
		static void  ParseWindowAbstract(_IN CXMLNode& rNode, _INOUT CWindowAbstract& rWindow);
		static DWORD ParseAlignment     (_IN CXMLNode& rNode);
	
		static void  ParseLayout            (_IN CXMLNode& rNode, _INOUT CWindowContainer& rContainer);
		static void  ParseLayoutTable       (_IN CXMLNode& rNode, _INOUT CWindowContainer& rContainer);
		static void  ParseLayoutTableCollumn(_IN CXMLNode& rNode, _INOUT CWindowContainer& rContainer, _INOUT CLayoutTable& rLayout, _IN UINT uiColumn);
		static void  ParseLayoutSplit       (_IN CXMLNode& rNode, _INOUT CWindowContainer& rContainer);

	private:
		static void BuildControls(_IN CXMLNode& rNode, _INOUT CWindowContainer& rContainer);

		static void BuildCursors(_IN CXMLNode& rNode);
		static void BuildFonts  (_IN CXMLNode& rNode);
		static void BuildWindows(_IN CXMLNode& rNode);

	public:
		static void BuildFontDefault();

	private:
		static void DeleteFonts();
		static void DeleteWindows();
		static void DeleteEditFields();

	private:
		static CDictionaryString s_Elements;
		static CDictionaryString s_Fonts;
		static CDictionaryString s_Cursors;

	private:
		CCache();
		~CCache();
	};
};