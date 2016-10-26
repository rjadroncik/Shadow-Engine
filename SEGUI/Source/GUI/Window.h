#pragma once

#include "WindowContainer.h"
#include "ButtonClick.h"

namespace GUI
{
	class SEGUI_API CWindow : public CWindowContainer
	{
	public:
		CString XmlName() _GET { return STRING("Window"); }

	public:
		CWindow();
		virtual ~CWindow();

	public:
		virtual bool IsDraggable() _GET { return TRUE; }


	public:
		CButtonClick& ButtonMin()   { return *m_pButtonMin; }
		CButtonClick& ButtonMax()   { return *m_pButtonMax; }
		CButtonClick& ButtonClose() { return *m_pButtonClose; }

	public:
		void OnWindowClose();
		void OnWindowMove();
		void OnWindowSize();

		void OnMouseMove(_IN SEventMouse& rEvent);

	public:
		CWindowAbstract* MouseOver    (_IN Float2& rMousePos, _OPT _OUT Float2* pOutLocalPosition = NULL) _GET;
		bool             MouseLocalPos(_IN Float2& rMousePos, _IN CWindowAbstract& rWindow, _OUT Float2& rOutLocalPosition) _GET;

	public:
		bool CanStartDragging(_IN Float2& rMousePos);

		//Returns a bitfield with alignment values describing which sizable part
		//of the frame the cursor is over, 0 - no sizing possible
		DWORD CanStartSizing(_IN Float2& rMousePos);

	public:
		inline CText& Text() _GET { return (CText&)m_Text; }

	public:
		void Render();
		void LayOut();

	protected:
		virtual void RenderNC();
		virtual void RenderNCButtons();

	private:
		static void OnWindowPaintButtonClose(_INOUT CWindowAbstract& rButton);
		static void OnWindowPaintButtonMin  (_INOUT CWindowAbstract& rButton);
		static void OnWindowPaintButtonMax  (_INOUT CWindowAbstract& rButton);

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	private:
		//Returns the width of the active buttons as they should be displayed
		float ButtonsWidth();

	protected:
		CText m_Text;
		float m_fBorder;

	protected:
		Float4 m_RectClient;
		float  m_fButtonSize;

	protected:
		CButtonClick* m_pButtonMin;
		CButtonClick* m_pButtonMax;
		CButtonClick* m_pButtonClose;
	};
};