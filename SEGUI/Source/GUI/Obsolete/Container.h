#pragma once

#include "Element.h"

namespace GUI
{
	class CContainer : public CElement
	{
		friend class CWindowAbstract;

	public:
		CContainer();
		virtual ~CContainer();

	public:
		virtual bool IsContainer() _GET { return TRUE; }

	public:
		int  IndexOf(_IN CElement& rElement) _GET;
		bool Has    (_IN CElement& rElement) _GET; //{ return (IndexOf(rElement) != -1) }
		UINT Count()                         _GET { return m_uiElementCount; }

	public:
		void Add   (_INOUT CElement& rElement) _SET;
		void Remove(_INOUT CElement& rElement) _SET;

	public:
		void AllRemove() _SET;
		void DeleteAll() _SET;

	protected:
		CElement** m_ppElements;
		UINT       m_uiElementCount;
	};
};