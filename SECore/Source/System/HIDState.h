#pragma once

#include "EventLog.h"

namespace System
{
	class SECORE_API CSystem;
	class SECORE_API CHIDState
	{
	public:
		static void Initialize();

	public:
		//Keyboard status queries
		class SECORE_API CKeyboard
		{
			friend class CHIDState;
			friend class CWindow;

		//These keep track of the last 2 keys pressed, thus they replace the arguments of event handlers 
		public:
			static UINT KeyLastDown()       { return s_uiKeyLastDown; }
			static UINT KeyLastUp()         { return s_uiKeyLastUp; }
			static UINT KeyLastRepeat()     { return s_uiKeyLastRepeat; }

			static UINT KeyPreviousDown()   { return s_uiKeyPreviousDown; }
			static UINT KeyPreviousUp()     { return s_uiKeyPreviousUp; }
			static UINT KeyPreviousRepeat() { return s_uiKeyPreviousRepeat; }

		public:
			static TCHAR CharLast()     { return s_usCharLast; }
			static TCHAR CharPrevious() { return s_usCharPrevious; }

		//Access to the state of the virtual keyboard as it is seen by the engine
		public:
			static bool KeyDown(_IN UINT uiVirtualKey) { return s_baKeysDown[uiVirtualKey]; }

		protected:
			static UINT s_uiKeyLastDown;
			static UINT s_uiKeyLastUp;
			static UINT s_uiKeyLastRepeat;

			static UINT s_uiKeyPreviousDown;
			static UINT s_uiKeyPreviousUp;
			static UINT s_uiKeyPreviousRepeat;

		public:
			static TCHAR s_usCharLast;
			static TCHAR s_usCharPrevious;

		protected:
			static bool s_baKeysDown[256];
		};

	public:
		//Mouse status queries
		class SECORE_API CMouse
		{
			friend class CHIDState;
			friend class CWindow;

		//These keep track of the last 2 buttons pressed, thus they replace the arguments of event handlers 
		public:
			static UINT ButtonLastDown()     { return s_uiButtonLastDown; }
			static UINT ButtonLastUp()       { return s_uiButtonLastUp; }

			static UINT ButtonPreviousDown() { return s_uiButtonPreviousDown; }
			static UINT ButtonPreviousUp()   { return s_uiButtonPreviousUp; }

		//Access to the state of the virtual mouse as it is seen by the engine
		public:
			static bool ButtonDown(_IN UINT uiMouseButton) { return s_baButtonsDown[uiMouseButton]; }

			static Float2& CursorPosition()                       { return s_CursorPosition; }
			static void    CursorPosition(_IN Float2& rPosititon) { CopyVector2(s_CursorPosition, rPosititon); }

			static float WheelPosition()                     { return s_fWheelPosition; }
			static void  WheelPosition(_IN float fPosititon) { s_fWheelPosition = fPosititon; }
		

		protected:
			static UINT s_uiButtonLastDown;
			static UINT s_uiButtonLastUp;

			static UINT s_uiButtonPreviousDown;
			static UINT s_uiButtonPreviousUp;

		protected:
			static bool s_baButtonsDown[16];

			static Float2 s_CursorPosition;
			static float s_fWheelPosition;
		};		

	private:
		CHIDState();
		~CHIDState();
	};
};
