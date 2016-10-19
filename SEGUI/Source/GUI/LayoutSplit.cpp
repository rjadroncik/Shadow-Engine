#include "LayoutSplit.h"

using namespace GUI;

CLayoutSplit::CLayoutSplit()
{
	m_Split.eType = LS_VERTICAL;
	m_Split.uiValue = 50; //In this configuration this means 50%-50% split
	m_Split.uiPadding = 0;
}

CLayoutSplit::~CLayoutSplit()
{
}

void CLayoutSplit::LayOut(_IN CVector<CElement>& rElements, _IN Float4& rRect, _OUT Float2& rOutSize)
{
	BYTE ucCoord[2];
	//The first coordinate is variable(based on the split value),
	//the second is bound to the proportions of the container
	ucCoord[0] = (m_Split.eType & LS_VERTICAL) ? (0) : (1);
	ucCoord[1] = (ucCoord[0] + 1) % 2;

  	switch(m_Split.eType & (LS_FIXED_1ST | LS_FIXED_2ND))
	{
	case LS_FIXED_1ST:
		{
			{
				Float4 Rect;
				CopyVector4(Rect, ((CElement&)rElements[0]).Rect());

				//Position
				Rect[ucCoord[0]] = 0;
				Rect[ucCoord[1]] = 0;

				//Size
				Rect[ucCoord[0] + 2] = (float)m_Split.uiValue;
				Rect[ucCoord[1] + 2] = rRect[ucCoord[1] + 2];
				((CElement&)rElements[0]).Rect(Rect);
			}

			{
				Float4 Rect;
				CopyVector4(Rect, ((CElement&)rElements[1]).Rect());

				//Position
				Rect[ucCoord[0]] = (float)m_Split.uiValue;
				Rect[ucCoord[1]] = 0;

				//Size
				Rect[ucCoord[0] + 2] = rRect[ucCoord[0] + 2] - m_Split.uiValue;
				Rect[ucCoord[1] + 2] = rRect[ucCoord[1] + 2];
				((CElement&)rElements[1]).Rect(Rect);
			}  
			break;
		}
	case LS_FIXED_2ND:
		{
			{
				Float4 Rect;
				CopyVector4(Rect, ((CElement&)rElements[0]).Rect());

				//Position
				Rect[ucCoord[0]] = 0;
				Rect[ucCoord[1]] = 0;

				//Size
				Rect[ucCoord[0] + 2] = rRect[ucCoord[0] + 2] - m_Split.uiValue;
				Rect[ucCoord[1] + 2] = rRect[ucCoord[1] + 2];
				((CElement&)rElements[0]).Rect(Rect);
			}

			{
				Float4 Rect;
				CopyVector4(Rect, ((CElement&)rElements[1]).Rect());

				//Position
				Rect[ucCoord[0]] = rRect[ucCoord[0] + 2] - m_Split.uiValue;
				Rect[ucCoord[1]] = 0;

				//Size
				Rect[ucCoord[0] + 2] = (float)m_Split.uiValue;
				Rect[ucCoord[1] + 2] = rRect[ucCoord[1] + 2];
				((CElement&)rElements[1]).Rect(Rect);
			}
			break;
		}
	default:
		{
			{
				Float4 Rect;
				CopyVector4(Rect, ((CElement&)rElements[0]).Rect());

				//Position
				Rect[ucCoord[0]] = 0;
				Rect[ucCoord[1]] = 0;

				_PENDING;
				//Size
				//Rect[ucCoord[0] + 2] = floorf(rRect[ucCoord[0] + 2] * (0.01f * m_Split.uiValue));
				Rect[ucCoord[0] + 2] = (float)(int)(rRect[ucCoord[0] + 2] * (0.01f * m_Split.uiValue));
				Rect[ucCoord[1] + 2] = rRect[ucCoord[1] + 2];
				((CElement&)rElements[0]).Rect(Rect);
			}

			{
				Float4 Rect;
				CopyVector4(Rect, ((CElement&)rElements[1]).Rect());
		
				_PENDING;
				//Position
				//Rect[ucCoord[0]] = floorf(rRect[ucCoord[0] + 2] * (0.01f * m_Split.uiValue));
				Rect[ucCoord[0]] = (float)(int)(rRect[ucCoord[0] + 2] * (0.01f * m_Split.uiValue));
				Rect[ucCoord[1]] = 0;

				_PENDING;
				//Size
				//Rect[ucCoord[0] + 2] = floorf(rRect[ucCoord[0] + 2] * (0.01f * (100 - m_Split.uiValue)));
				Rect[ucCoord[0] + 2] = (float)(int)(rRect[ucCoord[0] + 2] * (0.01f * (100 - m_Split.uiValue)));
				Rect[ucCoord[1] + 2] = rRect[ucCoord[1] + 2];
				((CElement&)rElements[1]).Rect(Rect);
			}
			break;
		}
 	}

	Float2 SizeMin;
	CopyVector2(SizeMin,          ((CElement&)rElements[0]).SizeMin());
	AddVectors2(SizeMin, SizeMin, ((CElement&)rElements[1]).SizeMin());

	if (rRect[2] < SizeMin[0]) { rOutSize[0] = SizeMin[0]; } else { rOutSize[0] = rRect[2]; }
	if (rRect[3] < SizeMin[1]) { rOutSize[1] = SizeMin[1]; } else { rOutSize[1] = rRect[3]; }
}

void CLayoutSplit::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
}

void CLayoutSplit::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CString* pValue = NULL;

	GETVALUE { SplitType(SCFXML::CXMLEnumeration::Translate(STRING("LayoutSplitTypes"), *pValue)->Value()); }
	GETVALUE { SplitValue  (CInt(*pValue).Value()); }
	GETVALUE { SplitPadding(CInt(*pValue).Value()); }
}
