#include "EditField.h"

#include "../../../SECore/Source/Resources/DataPool.h"
#include "../../../SECore/Source/System/Scripting.h"

using namespace GUI;
using namespace System;
using namespace Resources;

CLabel::CLabel()
{
	m_dwTextAlignment = AlignCenter;
}

CLabel::~CLabel()
{
	OnWindowClose();
}

void CLabel::Render()
{
	PositionPush();
	{
		Translate(m_Rect[0], m_Rect[1]);

		//this->RenderBackground();

		m_Text.Render();

		this->OnWindowPaint();
	}
	PositionPop();
}

void CLabel::LayOut()
{
	CopyVector2(m_SizeIdeal, m_Text.Size());
	if (!m_SizeMin[0] && !m_SizeMin[1]) { CopyVector2(m_SizeMin, m_Text.Size()); }

 	this->EnforceMinSize();

	Float4 LayoutRect = { 0, 0, m_Rect[2], m_Rect[3] };
	CWindowAbstract::TextLayOut(m_Text, LayoutRect, m_dwTextAlignment);
}

void CLabel::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
}

void CLabel::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CString* pValue = NULL;

	//Text
	rReader.GetBlock();
	{
		GETVALUE { Text().Font(*(CFont*)CScripting::Object(*pValue)); }

		GETVALUE { TextAlignment(SCFXML::CXMLEnumeration::Translate(STRING("TextAlignments"), *pValue)->Value()); }

		GETVALUE { Text().Color(CFloat4(*pValue).Value()); }

		//Text data
		//rReader.GetBlock();
		//Text().String(*rReader.GetBlockData());

		//Text data
		Text().String(*rReader.GetBlockValue());
	}

	LayOut();
}