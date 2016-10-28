#pragma once

#include "WindowContainer.h"
#include "../Resources/Cursor.h"

namespace GUI
{
	enum CursorTypes
	{
		CursorArrow = 0,
		CursorBeam,
		CursorBusy,
		CursorCross,
		CursorHand,
		CursorHelp,
		CursorMove,
		CursorNo,

		CursorSizeNESW,
		CursorSizeNWSE,
		CursorSizeWE,
		CursorSizeNS,

		CursorWait,

		CursorTypeCount,
	};

	class SEGUI_API CDispatcher;
	class SEGUI_API CModule : public CWindowContainer
	{
		friend class CDispatcher;

	public:
		System::Category Category() _GET { return System::Category::GUIModules; }

	public:
		CString XmlName() _GET { return STRING("Module"); }

	public:
		CModule();
		virtual ~CModule();

	public:
		void Render();
		void LayOut();

	public:
		class SEGUI_API CCursor
		{
			friend class CModule;

		public:
			CCursor();
			~CCursor();

		public:
			inline Float2& Position() { return m_Position; }

		public:
			void Position(_IN float fX, _IN float fY);
			void Move    (_IN float fX, _IN float fY);

		public:
			bool Show()               _GET;
			void Show(_IN bool bShow) _SET;

		public:
			void Use(_IN UINT uiIndex);
			void Enlist(_IN ENUM eType, _IN Resources::CCursor* pCursor);

		public:
			inline void          BoundsRect(_IN Float4& rRect) _SET { CopyVector4(m_Bounds, rRect); }
			inline const Float4& BoundsRect()                  _GET { return m_Bounds; }

			inline void          BoundsPosition(_IN float fX, _IN float fY) _SET { m_Bounds[0] = fX;           m_Bounds[1] = fY; }
			inline void          BoundsPosition(_IN Float2& rPosition)      _SET { m_Bounds[0] = rPosition[0]; m_Bounds[1] = rPosition[1]; }
			inline const Float2& BoundsPosition()                           _GET { return (Float2&)m_Bounds; }

			inline void          BoundsSize(_IN float fX, _IN float fY) _SET { m_Bounds[2] = fX;       m_Bounds[3] = fY; }
			inline void          BoundsSize(_IN Float2& rSize)          _SET { m_Bounds[2] = rSize[0]; m_Bounds[3] = rSize[1]; }
			inline const Float2& BoundsSize()                           _GET { return *(Float2*)(&m_Bounds[2]); }

		protected:
			bool m_bShow;

			//Both values are maintained automatically
			Float2 m_Position;
			Float2 m_PositionPrecise;

			Float4 m_Bounds;

		protected:
			void* m_pCursor;
			void* m_pCursors[CursorTypeCount];

			//const Resources::CCursor* m_pCursor;
			//const Resources::CCursor* m_pCursors[CursorTypeCount];

		} Cursor;

	//Internal use only
	protected:
		//These are used to determine the window-local position of the cursor & inside(over) which window the cursor resides
		bool             MouseLocalPos(_IN Float2& rMousePos, _IN CWindowAbstract& rWindow, _OUT Float2& rOutLocalPosition) _GET;
		CWindowAbstract* MouseOver    (_IN Float2& rMousePos, _OPT _OUT Float2* pOutLocalPosition = NULL) _GET;

	protected:
		void ChildMakeTopmost(_IN CWindowAbstract& rChild) _SET;

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);
	};
};