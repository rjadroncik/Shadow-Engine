#pragma once

#include "../SECore.h"

#include <SCFStandardUndef.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

namespace System
{
	#define KEYDOWN(name, key)    (((name)[(key)] & 0x80) != 0) 
	#define BUTTONDOWN(name, key) (((name)[(key)] & 0x80) != 0) 

	class CDirectInput
	{
	public:
		static bool Initialize();
		static void Shutdown();

		static void Acquire();

	public:
		static void UpdateMouseState();
		static void UpdateKeyboardState();

	public:
		static inline float MouseOffsetX() { return s_Mouse.State.Current.lX * s_Mouse.Settings.Sensitivity[0]; }
		static inline float MouseOffsetY() { return s_Mouse.State.Current.lY * s_Mouse.Settings.Sensitivity[1]; }
		static inline float MouseOffsetZ() { return s_Mouse.State.Current.lZ * s_Mouse.Settings.Sensitivity[2]; }

		static inline bool ButtonDown    (_IN UCHAR ucButton) { return   BUTTONDOWN(s_Mouse.State.Current.rgbButtons, ucButton); } 
		static inline bool ButtonDownLast(_IN UCHAR ucButton) { return   BUTTONDOWN(s_Mouse.State.Last.rgbButtons,    ucButton); } 
		
		static inline bool ButtonPressed (_IN UCHAR ucButton) { return (!BUTTONDOWN(s_Mouse.State.Last.rgbButtons,    ucButton) &&  BUTTONDOWN(s_Mouse.State.Current.rgbButtons, ucButton)); } 
		static inline bool ButtonReleased(_IN UCHAR ucButton) { return ( BUTTONDOWN(s_Mouse.State.Last.rgbButtons,    ucButton) && !BUTTONDOWN(s_Mouse.State.Current.rgbButtons, ucButton)); } 

	public:
		static inline bool KeyDown    (_IN UCHAR ucKey) { return   KEYDOWN(s_Keyboard.State.Current, ucKey); } 
		static inline bool KeyDownLast(_IN UCHAR ucKey) { return   KEYDOWN(s_Keyboard.State.Last,    ucKey); } 
		
		static inline bool KeyPressed (_IN UCHAR ucKey) { return (!KEYDOWN(s_Keyboard.State.Last,    ucKey) &&  KEYDOWN(s_Keyboard.State.Current, ucKey)); }
		static inline bool KeyReleased(_IN UCHAR ucKey) { return ( KEYDOWN(s_Keyboard.State.Last,    ucKey) && !KEYDOWN(s_Keyboard.State.Current, ucKey)); }

		static bool KeysDown(); 
		static void KeysDown(_OUT UCHAR* ucpOutChars, _OUT BYTE& ucrOutCount); 

	public:
		static inline Float3& MouseSensitivity()                         { return s_Mouse.Settings.Sensitivity; }
		static inline void    MouseSensitivity(_IN Float3& rSensitivity) { CopyVector3(s_Mouse.Settings.Sensitivity, rSensitivity); }

		static inline bool MousePrecisionEnhancement()                  { return s_Mouse.Settings.bPrecisionEnhancement; }
		static inline void MousePrecisionEnhancement(_IN bool bEnhance) { s_Mouse.Settings.bPrecisionEnhancement = bEnhance; }

	private:
		struct SMouse 
		{
			struct SState 
			{
				DIMOUSESTATE2 Current;
				DIMOUSESTATE2 Last;

			} State;

			struct SSettings 
			{
				Float3 Sensitivity;
				bool bPrecisionEnhancement;

			} Settings;

		} static s_Mouse;

		struct SKeyboard 
		{
			struct SState 
			{
				UCHAR Current[256];
				UCHAR Last[256];

			} State;
	  
		} static s_Keyboard;

		struct SRuntime
		{
			IDirectInput*       pDInput; 
			IDirectInputDevice* pDIMouse; 
			IDirectInputDevice* pDIKeyboard; 

		} static s_Runtime;

	private:
		CDirectInput();
		~CDirectInput();
	};
};