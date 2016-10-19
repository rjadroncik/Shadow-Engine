#pragma once

#include "WindowAbstract.h"
#include "Layout.h"

namespace GUI
{
	class SEGUI_API CWindowContainer : public CWindowAbstract
	{
		friend class CWindowAbstract;

	public:
		CWindowContainer();
		virtual ~CWindowContainer();

	public:
		virtual bool IsContainer() _GET { return TRUE; }

	public:
		virtual void Render();
		virtual void LayOut();

	public:
		CLayout* Layout()                     { return m_pLayout; }
		void     Layout(_IN CLayout* pLayout) { m_pLayout = (CLayout*)pLayout; }

	public:
		CWindowAbstract* MouseOver    (_IN Float2& rMousePos, _OPT _OUT Float2* pOutLocalPosition = NULL) _GET;
		bool             MouseLocalPos(_IN Float2& rMousePos, _IN CWindowAbstract& rWindow, _OUT Float2& rOutLocalPosition) _GET;

	public:
		int  ChildIndexOf(_IN CWindowAbstract& rChild) _GET;
		bool ChildHas    (_IN CWindowAbstract& rChild) _GET { return (ChildIndexOf(rChild) != -1); }
		UINT ChildCount()                              _GET { return m_Windows.Size(); }

	public:
		void ChildAdd   (_INOUT CWindowAbstract& rChild) _SET;
		void ChildRemove(_INOUT CWindowAbstract& rChild) _SET;

	public:
		void ChildrenRemoveAll() _SET;
		void ChildrenDeleteAll() _SET;

	protected:
#pragma warning(disable:4251)
		CVector<CWindowAbstract> m_Windows;
#pragma warning(default:4251)

	protected:
		CLayout* m_pLayout;
	};
};