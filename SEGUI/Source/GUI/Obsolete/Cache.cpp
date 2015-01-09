#include "Cache.h"

#include "../../../Core/Source/System/System.h"

using namespace GUI;
using namespace System;

CDictionaryString CCache::s_Cursors;
CDictionaryString CCache::s_Fonts;
CDictionaryString CCache::s_Elements;

bool CCache::ObjectsCreate(_IN CString& rFile)
{
	CEventLog::BlockNew(CString(STRING("Creating GUI objects")));
	{
		if (!s_Fonts.NameToObject(CString(STRING("Default.Font"))))
		{
			CCache::BuildFontDefault();
		}
 
		CEventLog::EventNew(CString(STRING("Loading: ")) + CFile(rFile).PathFull());

		CXMLDocumentFile Source(*(new CFile(rFile)));
		Source.ResolveIncludes();

		CXMLNode* pCursors = Source.Root()->ChildNamed(CString(STRING("cursors")));
		CXMLNode* pFonts   = Source.Root()->ChildNamed(CString(STRING("fonts")));
		CXMLNode* pWindows = Source.Root()->ChildNamed(CString(STRING("windows")));
	
		if (pFonts)   { BuildFonts  (*pFonts); }
		if (pWindows) { BuildWindows(*pWindows); }
	}
	CEventLog::BlockClose();

	return TRUE;
}

void CCache::BuildFonts(_IN CXMLNode& rNode)
{
	CEventLog::BlockNew(CString(STRING("Creating fonts")));
	{
		CXMLNode* pCurrent = (CXMLNode*)rNode.ChildFirst();

		while (pCurrent)
		{
			if (pCurrent->Name() == CString(STRING("font")))
			{
				//Skip unlabeled nodes
				if (!pCurrent->AttributeFirst()) { continue; }

				//Find nodes containing relevant data
				CXMLAttribute* pFileXML   = pCurrent->AttributeNamed(CString(STRING("xmlFile")));
				CXMLAttribute* pFileImage = pCurrent->AttributeNamed(CString(STRING("imageFile")));

				//Check whether required nodes were found
				if (!pFileXML || !pFileImage) { continue; }

				CEventLog::EventNew(CString(STRING("Creating: ")) + pCurrent->IDString());

				//Create a new material & register it in the material cache
				CFont* pFont = new CFont();
				s_Fonts.AtPut(pCurrent->IDString(), *pFont);

				pFont->Create(pFileXML->ValueString(), pFileImage->ValueString());
			}

			pCurrent = (CXMLNode*)pCurrent->Next();
		}   
	}
	CEventLog::BlockClose();
}

void CCache::BuildFontDefault()
{
	CEventLog::EventNew(CString(STRING("Creating default font")));

	CFont* pFont = new CFont();
	s_Fonts.AtPut(CString(STRING("Default.Font")), *pFont);

	pFont->Create(CString(STRING("Data/Fonts/Tahoma.xml")), CString(STRING("Data/Fonts/Tahoma.bmp")));
}	

DWORD CCache::ParseAlignment(_IN CXMLNode& rNode)
{
	CXMLAttribute* pAlignment = rNode.AttributeNamed(CString(STRING("alignment")));
	
	if (pAlignment) 
	{ 
		if (CString(STRING("left"))   == pAlignment->ValueString()) { return AlignLeft; }
		if (CString(STRING("right"))  == pAlignment->ValueString()) { return AlignRight; }
		if (CString(STRING("top"))    == pAlignment->ValueString()) { return AlignTop; }
		if (CString(STRING("bottom")) == pAlignment->ValueString()) { return AlignBottom; }
	
		if (CString(STRING("center")) == pAlignment->ValueString()) { return AlignCenter; }

		if (CString(STRING("topLeft"))  == pAlignment->ValueString()) { return AlignTop | AlignLeft; }
		if (CString(STRING("topRight")) == pAlignment->ValueString()) { return AlignTop | AlignRight; }

		if (CString(STRING("bottomLeft"))  == pAlignment->ValueString()) { return AlignBottom | AlignLeft; }
		if (CString(STRING("bottomRight")) == pAlignment->ValueString()) { return AlignBottom | AlignRight; }
	}

	return AlignCenter;
}

void CCache::ParseWindowAbstract(_IN CXMLNode& rNode, _INOUT CWindowAbstract& rWindow)
{
	//Find nodes containing relevant data
	CXMLAttribute* pPosition = rNode.AttributeNamed(CString(STRING("position")));
	CXMLAttribute* pSize     = rNode.AttributeNamed(CString(STRING("size")));
	CXMLAttribute* pSizeMin  = rNode.AttributeNamed(CString(STRING("sizeMin")));
	CXMLAttribute* pSizeMax  = rNode.AttributeNamed(CString(STRING("sizeMax")));

	CXMLAttribute* pEnabled   = rNode.AttributeNamed(CString(STRING("enabled")));
	CXMLAttribute* pResizable = rNode.AttributeNamed(CString(STRING("resizable")));

	if (pPosition) { rWindow.Position(CFloat2::Parse(pPosition->ValueString())); }
	if (pSize)     { rWindow.Size    (CFloat2::Parse(    pSize->ValueString())); }
	if (pSizeMin)  { rWindow.SizeMin (CFloat2::Parse( pSizeMin->ValueString())); }
	if (pSizeMax)  { rWindow.SizeMax (CFloat2::Parse( pSizeMax->ValueString())); }

	if (pEnabled)   { rWindow.Enabled  (CBool(  pEnabled->ValueString()).Value()); }
	if (pResizable) { rWindow.Resizable(CBool(pResizable->ValueString()).Value()); }
}

void CCache::ParseText(_IN CXMLNode& rNode, _INOUT CText& rText)
{
	//Find nodes containing relevant data
	CXMLAttribute* pFont  = rNode.AttributeNamed(CString(STRING("font")));
	CXMLAttribute* pColor = rNode.AttributeNamed(CString(STRING("color")));

	rText.String(rNode.ValueString()); 
	
	if (pFont)
	{
		CFont* pFontObj = (CFont*)s_Fonts.NameToObject(pFont->ValueString()); 
		if (pFontObj) { rText.Font(*pFontObj); }
	}
	else { rText.Font(*(CFont*)s_Fonts.NameToObject(CString(STRING("Default.Font")))); }

	if (pColor) 
	{
		rText.Color(CFloat4::Parse(pColor->ValueString())); 
	}
}

void CCache::ParseLayout(_IN CXMLNode& rNode, _INOUT CWindowContainer& rContainer)
{
	CXMLNode* pLayout = rNode.ChildNamed(CString(STRING("layout")));
	if (pLayout)
	{
		CXMLAttribute* pType = pLayout->AttributeNamed(CString(STRING("type")));

		if (!pType)	{ return; }

		if (pType->ValueString() == CString(STRING("table")))
		{
			ParseLayoutTable(*pLayout, rContainer);
			return;
		}
		if (pType->ValueString() == CString(STRING("split")))
		{
			ParseLayoutSplit(*pLayout, rContainer);
			return;
		}
	} 
}


void CCache::ParseLayoutTableCollumn(_IN CXMLNode& rNode, _INOUT CWindowContainer& rContainer, _INOUT CLayoutTable& rLayout, _IN UINT uiColumn)
{
	UINT uiRow = 0;
	CXMLNode* pCurrent = (CXMLNode*)rNode.ChildFirst();
	while (pCurrent)
	{
		if (pCurrent->Name() == CString(STRING("cell")))
		{
			CXMLAttribute* pElement = pCurrent->AttributeNamed(CString(STRING("element")));

			if (!pElement) { continue; }

			Int2 ElementLocation = { uiColumn, uiRow };
			CString ElementName  = pElement->ValueString() + CString(STRING(".")) + rContainer.IDString() + CString(STRING(".")) + pCurrent->ValueString();

			if (pElement->ValueString() == CString(STRING("layout")))
			{
			}
			else
			{
				rLayout.TableElement(ElementLocation, CCache::Element(ElementName));
				uiRow++;
			}

		}

		pCurrent = (CXMLNode*)pCurrent->Next();
	}
}

void CCache::ParseLayoutTable(_IN CXMLNode& rNode, _INOUT CWindowContainer& rContainer)
{
	CXMLAttribute* pRows    = rNode.AttributeNamed(CString(STRING("rows")));
	CXMLAttribute* pColumns = rNode.AttributeNamed(CString(STRING("columns")));

	if (!pColumns || !pRows) { return; }

	//Create a new layout object & assign it to it's owner
	CLayoutTable* pLayout = new CLayoutTable();
	rContainer.Layout(pLayout);

	//Extract table size
	Int2 TableSize;
	TableSize[0] = CInt(pColumns->ValueString()).Value();
	TableSize[1] = CInt(pRows->ValueString()).Value();

	pLayout->TableSize(TableSize);

	//Extract cell data
	CXMLNode* pCells = rNode.ChildNamed(CString(STRING("cells")));
	if (pCells)
	{
		CXMLAttribute* pPadding = pCells->AttributeNamed(CString(STRING("padding")));
		if (pPadding) { pLayout->CellPadding(CInt(pColumns->ValueString()).Value()); }

		UINT uiColumn = 0;
		CXMLNode* pCurrent = (CXMLNode*)pCells->ChildFirst();
		while (pCurrent)
		{
			if (pCurrent->Name() == CString(STRING("column")))
			{
				ParseLayoutTableCollumn(*pCurrent, rContainer, *pLayout, uiColumn); 
				uiColumn++;
			}
			pCurrent = (CXMLNode*)pCurrent->Next();
		}
	}
}

void CCache::ParseLayoutSplit(_IN CXMLNode& rNode, _INOUT CWindowContainer& rContainer)
{
	CXMLAttribute* pSplitType  = rNode.AttributeNamed(CString(STRING("splitType")));
	CXMLAttribute* pSplitValue = rNode.AttributeNamed(CString(STRING("splitValue")));
	CXMLAttribute* pFixed      = rNode.AttributeNamed(CString(STRING("fixed")));

	CXMLNode* pFirst  = rNode.ChildNamed( CString(STRING("first")));
	CXMLNode* pSecond = rNode.ChildNamed( CString(STRING("second")));

	//Create a new layout object & assign it to it's owner
	CLayoutSplit* pLayout = new CLayoutSplit();
	rContainer.Layout(pLayout);
									 
	ENUM eLayoutType = 0;

	if (pSplitType)
	{
		if (pSplitType->ValueString() == CString(STRING("horizontal"))) { eLayoutType = LS_HORIZONTAL; }
		if (pSplitType->ValueString() == CString(STRING("vertical")))   { eLayoutType = LS_VERTICAL; }
	}

	if (pFixed)
	{
		if (pFixed->ValueString() == CString(STRING("first")))  { eLayoutType |= LS_FIXED_1ST; }
		if (pFixed->ValueString() == CString(STRING("second"))) { eLayoutType |= LS_FIXED_2ND; }
	}

	pLayout->SplitType(eLayoutType);

	if (pSplitValue)
	{
		pLayout->SplitValue(CInt(pSplitValue->ValueString()).Value());
	}

	if (pFirst)
	{
		CXMLAttribute* pElementType = pFirst->AttributeNamed(CString(STRING("element")));

		CString csElementName = pElementType->ValueString() + CString(STRING(".")) + rContainer.IDString() + CString(STRING(".")) + pFirst->ValueString();
		pLayout->Element(0, CCache::Element(csElementName));
	}

	if (pSecond)
	{
		CXMLAttribute* pElementType = pSecond->AttributeNamed(CString(STRING("element")));

		CString csElementName = pElementType->ValueString() + CString(STRING(".")) + rContainer.IDString() + CString(STRING(".")) + pSecond->ValueString();
		pLayout->Element(1, CCache::Element(csElementName));
	}
}

void CCache::BuildControls(_IN CXMLNode& rNode, _INOUT CWindowContainer& rContainer)
{
	CEventLog::BlockNew(CString(STRING("Creating controls")));
	{
		CXMLNode* pCurrent = (CXMLNode*)rNode.ChildFirst();

		while (pCurrent)
		{
			if (pCurrent->Name() == CString(STRING("editField")))
			{
				//Skip unlabeled nodes
				if (!pCurrent->AttributeFirst()) { continue; }

				CEventLog::EventNew(CString(STRING("Creating edit field: ")) + pCurrent->IDString());

				//Create a new edit field & register it in the element cache
				CEditField* pEditField = new CEditField(rContainer.IDString() + CString(STRING(".")) + pCurrent->IDString(), rContainer);
				CCache::EditField(*pEditField);

				ParseWindowAbstract(*pCurrent, *pEditField);

				CXMLNode* pText = pCurrent->ChildNamed(CString(STRING("text")));
				if (pText) 
				{ 
					ParseText(*pText, pEditField->Text());
					pEditField->TextAlignment(ParseAlignment(*pText));
				}
				pEditField->OnWindowOpen();
			}

			if (pCurrent->Name() == CString(STRING("label")))
			{
				//Skip unlabeled nodes
				if (!pCurrent->AttributeFirst()) { continue; }

				CEventLog::EventNew(CString(STRING("Creating edit field: ")) + pCurrent->IDString());

				//Create a new edit field & register it in the element cache
				CLabel* pLabel = new CLabel(rContainer.IDString() + CString(STRING(".")) + pCurrent->IDString(), rContainer);
				CCache::Label(*pLabel);

				ParseWindowAbstract(*pCurrent, *pLabel);

				CXMLNode* pText = pCurrent->ChildNamed(CString(STRING("text")));
				if (pText) 
				{ 
					ParseText(*pText, pLabel->Text());
					pLabel->TextAlignment(ParseAlignment(*pText));
				}
				pLabel->OnWindowOpen();
			}
			pCurrent = (CXMLNode*)pCurrent->Next();
		} 
	}
	CEventLog::BlockClose();
}

void CCache::BuildWindows(_IN CXMLNode& rNode)
{
	CEventLog::BlockNew(CString(STRING("Creating windows")));
	{
		CXMLNode* pCurrent = (CXMLNode*)rNode.ChildFirst();

		while (pCurrent)
		{
			if (pCurrent->Name() == CString(STRING("window")))
			{
				//Skip unlabeled nodes
				if (!pCurrent->AttributeFirst()) { continue; }

				CEventLog::EventNew(CString(STRING("Creating: ")) + pCurrent->IDString());

				//Create a new window & register it in the element cache
				CWindow* pWindow = new CWindow(pCurrent->IDString());
				CCache::Window(*pWindow);

				CXMLAttribute* pButtonMinEnabled   = pCurrent->AttributeNamed(CString(STRING("buttonMin")));
				CXMLAttribute* pButtonMaxEnabled   = pCurrent->AttributeNamed(CString(STRING("buttonMax")));
				CXMLAttribute* pButtonCloseEnabled = pCurrent->AttributeNamed(CString(STRING("buttonClose")));

				if (pButtonMinEnabled)   { pWindow->ButtonMin().Enabled  (CBool(pButtonMinEnabled->ValueString()).Value()); }
				if (pButtonMaxEnabled)   { pWindow->ButtonMax().Enabled  (CBool(pButtonMaxEnabled->ValueString()).Value()); }
				if (pButtonCloseEnabled) { pWindow->ButtonClose().Enabled(CBool(pButtonCloseEnabled->ValueString()).Value()); }
			
				ParseWindowAbstract(*pCurrent, *pWindow);
				BuildControls      (*pCurrent, *pWindow);
				ParseLayout        (*pCurrent, *pWindow);

				CXMLNode* pText = pCurrent->ChildNamed(CString(STRING("text")));
				if (pText) 
				{ 
					ParseText(*pText, pWindow->Text());
				}

				pWindow->OnWindowOpen();
			}

			pCurrent = (CXMLNode*)pCurrent->Next();
		}   
	}
	CEventLog::BlockClose();
}

void CCache::DeleteWindows()
{
	CEventLog::BlockNew(CString(STRING("Deleting all windows")));
	{
		CEnumeratorDictionaryString Enumerator(s_Elements);
		while (Enumerator.Next()) 
		{
			if (Enumerator.Current()->ClassKey() == ClassWindow)
			{
				CEventLog::EventNew(CString(STRING("Deleting: ")) + s_Elements.ObjectToName(*Enumerator.Current()));
				Enumerator.CurrentShallowDelete();
			}
		}
		//s_Windows.AllRemove();
	}
	CEventLog::BlockClose();
}

void CCache::DeleteEditFields()
{
	CEventLog::BlockNew(CString(STRING("Deleting all edit fields")));
	{
		CEnumeratorDictionaryString Enumerator(s_Elements);
		while (Enumerator.Next()) 
		{
			if (Enumerator.Current()->ClassKey() == ClassEditField)
			{
				CEventLog::EventNew(CString(STRING("Deleting: ")) + s_Elements.ObjectToName(*Enumerator.Current()));
				Enumerator.CurrentShallowDelete();
			}
		}
		//s_EditFields.AllRemove();
	}
	CEventLog::BlockClose();
}

void CCache::DeleteFonts()
{
	CEventLog::BlockNew(CString(STRING("Deleting all fonts")));
	{
		CEnumeratorDictionaryString FontEnumerator(s_Fonts);
		while (FontEnumerator.Next()) 
		{
			CEventLog::EventNew(CString(STRING("Deleting: ")) + s_Fonts.ObjectToName(*FontEnumerator.Current()));
			FontEnumerator.CurrentShallowDelete();
		}
		s_Fonts.AllRemove();
	}
	CEventLog::BlockClose();
}

void CCache::ObjectsDelete()
{
	CEventLog::BlockNew(CString(STRING("Deleting all GUI objects")));
	{
		DeleteWindows();
		DeleteEditFields();
		DeleteFonts();
	}
	CEventLog::BlockClose();
}