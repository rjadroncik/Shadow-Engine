#include "LayoutTable.h"

using namespace GUI;

CLayoutTable::CLayoutTable()
{
	ZeroVector2(m_Table.Size);
	m_Table.fPadding = 0;
}

CLayoutTable::~CLayoutTable()
{
}

void CLayoutTable::LayOut(_IN CVector<CElement>& rElements, _IN Float4& rRect, _OUT Float2& rOutSize)
{
	Float2 ElementSpacing;
	ElementSpacing[0] = (float)(int)(rRect[2] / m_Table.Size[0]);
	ElementSpacing[1] = (float)(int)(rRect[3] / m_Table.Size[1]);

	Float4 ElementRect;
	ElementRect[2] = ElementSpacing[0] - (m_Table.fPadding * 2);
	ElementRect[3] = ElementSpacing[1] - (m_Table.fPadding * 2);

	Float2 ElementMaxSize;
	ElementMaxSize[0] = 0;
	ElementMaxSize[1] = 0;

	for (int y = 0; y < m_Table.Size[1]; y++)
	{
		for (int x = 0; x < m_Table.Size[0]; x++)
		{
			CElement* pElement =  (CElement*)&rElements[y * m_Table.Size[0] + x];

			if (pElement)
			{
				ElementRect[0] = (ElementSpacing[0] * x) + m_Table.fPadding;
				ElementRect[1] = (ElementSpacing[1] * y) + m_Table.fPadding;
			
				pElement->Rect(ElementRect);

				if (ElementMaxSize[0] < pElement->SizeMin()[0]) { ElementMaxSize[0] = pElement->SizeMin()[0]; }
				if (ElementMaxSize[1] < pElement->SizeMin()[1]) { ElementMaxSize[1] = pElement->SizeMin()[1]; }
			}
		}
	}

	if (ElementMaxSize[0] > ElementRect[2])
	{
		ElementSpacing[0] = ElementMaxSize[0] + (m_Table.fPadding * 2);
		ElementRect[2]    = ElementSpacing[0] - (m_Table.fPadding * 2);

		for (int y = 0; y < m_Table.Size[1]; y++)
		{
			for (int x = 0; x < m_Table.Size[0]; x++)
			{
				CElement* pElement =  (CElement*)&rElements[y * m_Table.Size[0] + x];

				if (pElement)
				{
					ElementRect[0] = (ElementSpacing[0] * x) + m_Table.fPadding;
					ElementRect[1] = (ElementSpacing[1] * y) + m_Table.fPadding;

					pElement->Rect(ElementRect);
				}
			}
		}
	}

	if (ElementMaxSize[1] > ElementRect[3])
	{
		ElementSpacing[1] = ElementMaxSize[1] + (m_Table.fPadding * 2);
		ElementRect[3]    = ElementSpacing[1] - (m_Table.fPadding * 2);

		for (int y = 0; y < m_Table.Size[1]; y++)
		{
			for (int x = 0; x < m_Table.Size[0]; x++)
			{
				CElement* pElement = (CElement*)&rElements[y * m_Table.Size[0] + x];

				if (pElement)
				{
					ElementRect[0] = (ElementSpacing[0] * x) + m_Table.fPadding;
					ElementRect[1] = (ElementSpacing[1] * y) + m_Table.fPadding;

					pElement->Rect(ElementRect);
				}
			}
		}
	}

	rOutSize[0] = ElementSpacing[0] * m_Table.Size[0];
	rOutSize[1] = ElementSpacing[1] * m_Table.Size[1];
}

void CLayoutTable::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
}

void CLayoutTable::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CString* pValue = NULL;

	Int2 Size = { 0, 0 };
	GETVALUE { Size[0] = CInt(*pValue).Value(); }
	GETVALUE { Size[1] = CInt(*pValue).Value(); }

	TableSize(Size); 

	GETVALUE { CellPadding(CFloat(*pValue).Value()); }

	//Columns
	rReader.GetBlock();
	{
		UINT uiCount = 0;
		GETVALUE { uiCount = CInt(*pValue).Value(); }

		for (UINT i = 0; i < uiCount; i++)
		{
			rReader.GetBlock();
		}
	}

	//Rows
	rReader.GetBlock();
	{
		UINT uiCount = 0;
		GETVALUE { uiCount = CInt(*pValue).Value(); }

		for (UINT i = 0; i < uiCount; i++)
		{
			rReader.GetBlock();
		}
	}
}