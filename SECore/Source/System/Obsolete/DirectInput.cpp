#include "DirectInput.h"

#include "System.h"

using namespace System;

CDirectInput::SMouse    CDirectInput::s_Mouse;
CDirectInput::SKeyboard CDirectInput::s_Keyboard;
CDirectInput::SRuntime  CDirectInput::s_Runtime;

bool CDirectInput::Initialize()
{
	CEventLog::BlockNew(STRING("Initializing direct input"));
	{
		CEventLog::EventNew(STRING("Direct input create"));
		DirectInput8Create(CSystem::Instance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&s_Runtime.pDInput, NULL);

		if (!s_Runtime.pDInput) 
		{ 
			SCFError(ErrorDIObjectMainCreationFailed); 
			return FALSE;
		}

		CEventLog::EventNew(STRING("Direct input mouse device object create"));
		s_Runtime.pDInput->CreateDevice(GUID_SysMouse,    &s_Runtime.pDIMouse,    NULL);
		if (!s_Runtime.pDIMouse)
		{ 
			SCFError(ErrorDIObjectMouseCreationFailed); 
			return FALSE;
		}

		CEventLog::EventNew(STRING("Direct input keyboard device object create"));
		s_Runtime.pDInput->CreateDevice(GUID_SysKeyboard, &s_Runtime.pDIKeyboard, NULL);
		if (!s_Runtime.pDIKeyboard) 
		{
			SCFError(ErrorDIObjectKeyboardCreationFailed); 
			return FALSE;
		}

		s_Runtime.pDIMouse->SetCooperativeLevel(CSystem::Window(), DISCL_FOREGROUND | DISCL_EXCLUSIVE);
		s_Runtime.pDIMouse->SetDataFormat(&c_dfDIMouse2);

		s_Runtime.pDIKeyboard->SetCooperativeLevel(CSystem::Window(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		s_Runtime.pDIKeyboard->SetDataFormat(&c_dfDIKeyboard);

		s_Mouse.Settings.Sensitivity[0] = 1;
		s_Mouse.Settings.Sensitivity[1] = 1;
		s_Mouse.Settings.bPrecisionEnhancement = TRUE;
	}
	CEventLog::BlockClose();

	return TRUE;
}

void CDirectInput::Acquire()
{
	CEventLog::BlockNew(STRING("Acquiring Direct input"));
	{
		CEventLog::EventNew(STRING("Acquiring direct input for mouse"));
		if (FAILED(s_Runtime.pDIMouse->Acquire()))
		{
			//CSystem::ShutdownOnError(STRING("Unable to acquire direct input for mouse.")));
		}

		CEventLog::EventNew(STRING("Acquiring direct input for keyboard"));
		if (FAILED(s_Runtime.pDIKeyboard->Acquire()))
		{
			//CSystem::ShutdownOnError(STRING("Unable to acquire direct input for keyboard.")));
		}
	}
	CEventLog::BlockClose();
}

void CDirectInput::Shutdown()
{
	CEventLog::BlockNew(STRING("Shutting down direct input"));
	{
		CEventLog::EventNew(STRING("Releasing direct input for mouse"));
		s_Runtime.pDIMouse->Unacquire();
		s_Runtime.pDIMouse->Release();

		CEventLog::EventNew(STRING("Releasing direct input for keyboard"));
		s_Runtime.pDIKeyboard->Unacquire();
		s_Runtime.pDIKeyboard->Release();

		CEventLog::EventNew(STRING("Releasing direct input object"));
		s_Runtime.pDInput->Release();
	}
	CEventLog::BlockClose();
}

void CDirectInput::UpdateMouseState()
{
	s_Mouse.State.Last = s_Mouse.State.Current;

	s_Runtime.pDIMouse->GetDeviceState(sizeof(s_Mouse.State.Current), &s_Mouse.State.Current);

	if (s_Mouse.Settings.bPrecisionEnhancement)
	{
		if (s_Mouse.State.Current.lX != 0) { s_Mouse.State.Current.lX += (int)(s_Mouse.State.Current.lX * log((double)__abs(s_Mouse.State.Current.lX)) * 0.6); }
		if (s_Mouse.State.Current.lY != 0) { s_Mouse.State.Current.lY += (int)(s_Mouse.State.Current.lY * log((double)__abs(s_Mouse.State.Current.lY)) * 0.6); }
	}
}

void CDirectInput::UpdateKeyboardState()
{
	memcpy(s_Keyboard.State.Last, s_Keyboard.State.Current, 256);

	s_Runtime.pDIKeyboard->GetDeviceState(sizeof(s_Keyboard.State.Current), &s_Keyboard.State.Current);
}

void CDirectInput::KeysDown(_OUT UCHAR* ucpOutChars, _OUT BYTE& ucrOutCount) 
{
	ucrOutCount = 0;

	for (BYTE i = 0; i < 255; i++)
	{
		if (KEYDOWN(s_Keyboard.State.Current, i)) { ucpOutChars[ucrOutCount] = i; ucrOutCount++; }
	}
}

bool CDirectInput::KeysDown()
{
	for (BYTE i = 0; i < 255; i++)
	{
		if (KEYDOWN(s_Keyboard.State.Current, i)) { return TRUE; }
	}

	return FALSE;
}