#pragma once

#include "Label.h"

namespace GUI
{
	class SEGUI_API CEditField;

	typedef void (*HANDLER_ACTION_EDITFIELD_ENTER)(_INOUT CEditField&);

	class SEGUI_API CEditField : public CLabel
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassEditField; }

	public:
		CEditField();
		virtual ~CEditField();

	public:
		void ActionEnter() { if (m_fpOnActionEnter) { m_fpOnActionEnter(*this); } }

	public:
		void                           OnActionEnter(HANDLER_ACTION_EDITFIELD_ENTER fpActionHandler) _SET { m_fpOnActionEnter = fpActionHandler; }
		HANDLER_ACTION_EDITFIELD_ENTER OnActionEnter()                                               _GET { return m_fpOnActionEnter; }

	public:
		 //Keyboard tracking
		 void OnKeyPressed      (_IN SEventKey& rEvent);
		 void OnKeyReleased     (_IN SEventKey& rEvent);
		 void OnKeyFocusReceived(_IN SEventKey& rEvent);
		 void OnKeyFocusLost    (_IN SEventKey& rEvent);

	public:
		 //Mouse tracking
		 void OnMouseButtonPressed    (_IN SEventMouse& rEvent);
		 void OnMouseButtonReleased   (_IN SEventMouse& rEvent);
		 void OnMouseButtonDoubleClick(_IN SEventMouse& rEvent);
		 void OnMouseMove             (_IN SEventMouse& rEvent);

	public:
		const Float4& CaretRect();
		//const Float4& SelectionRect();

	public:
		void Render();
		void LayOut();

	protected:
		void SelectionRender();
		void SelectionUpdate();
		void SelectionUpdateWhileDragging();

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	protected:
		Int2   m_Selection;
		Float4 m_SelectionRect;

		int m_iCaretIndex;

	protected:
		HANDLER_ACTION_EDITFIELD_ENTER m_fpOnActionEnter;

	protected:
		static Float2 s_MousePositionStart;
		static Float2 s_MousePositionEnd;

		static int s_iCarretIndexStart;
	};
};