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
			static SCF::UINT KeyLastDown()       { return s_uiKeyLastDown; }
			static SCF::UINT KeyLastUp()         { return s_uiKeyLastUp; }
			static SCF::UINT KeyLastRepeat()     { return s_uiKeyLastRepeat; }

			static SCF::UINT KeyPreviousDown()   { return s_uiKeyPreviousDown; }
			static SCF::UINT KeyPreviousUp()     { return s_uiKeyPreviousUp; }
			static SCF::UINT KeyPreviousRepeat() { return s_uiKeyPreviousRepeat; }

		public:
			static SCF::TCHAR CharLast()     { return s_usCharLast; }
			static SCF::TCHAR CharPrevious() { return s_usCharPrevious; }

		//Access to the state of the virtual keyboard as it is seen by the engine
		public:
			static bool KeyDown(_IN SCF::UINT uiVirtualKey) { return s_baKeysDown[uiVirtualKey]; }

		protected:
			static SCF::UINT s_uiKeyLastDown;
			static SCF::UINT s_uiKeyLastUp;
			static SCF::UINT s_uiKeyLastRepeat;

			static SCF::UINT s_uiKeyPreviousDown;
			static SCF::UINT s_uiKeyPreviousUp;
			static SCF::UINT s_uiKeyPreviousRepeat;

		public:
			static SCF::TCHAR s_usCharLast;
			static SCF::TCHAR s_usCharPrevious;

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
			static SCF::UINT ButtonLastDown()     { return s_uiButtonLastDown; }
			static SCF::UINT ButtonLastUp()       { return s_uiButtonLastUp; }

			static SCF::UINT ButtonPreviousDown() { return s_uiButtonPreviousDown; }
			static SCF::UINT ButtonPreviousUp()   { return s_uiButtonPreviousUp; }

		//Access to the state of the virtual mouse as it is seen by the engine
		public:
			static bool ButtonDown(_IN SCF::UINT uiMouseButton) { return s_baButtonsDown[uiMouseButton]; }

			static Float2& CursorPosition()                       { return s_CursorPosition; }
			static void    CursorPosition(_IN Float2& rPosititon) { CopyVector2(s_CursorPosition, rPosititon); }

			static float WheelPosition()                     { return s_fWheelPosition; }
			static void  WheelPosition(_IN float fPosititon) { s_fWheelPosition = fPosititon; }
		

		protected:
			static SCF::UINT s_uiButtonLastDown;
			static SCF::UINT s_uiButtonLastUp;

			static SCF::UINT s_uiButtonPreviousDown;
			static SCF::UINT s_uiButtonPreviousUp;

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
