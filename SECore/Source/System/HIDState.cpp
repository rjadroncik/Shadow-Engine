#include "HIDState.h"

#include "System.h"
#include "Settings.h"

using namespace System;

SCF::UINT CHIDState::CKeyboard::s_uiKeyLastDown   = 0;
SCF::UINT CHIDState::CKeyboard::s_uiKeyLastUp     = 0;
SCF::UINT CHIDState::CKeyboard::s_uiKeyLastRepeat = 0;

SCF::UINT CHIDState::CKeyboard::s_uiKeyPreviousDown   = 0;
SCF::UINT CHIDState::CKeyboard::s_uiKeyPreviousUp     = 0;
SCF::UINT CHIDState::CKeyboard::s_uiKeyPreviousRepeat = 0;

SCF::TCHAR CHIDState::CKeyboard::s_usCharLast     = 0;
SCF::TCHAR CHIDState::CKeyboard::s_usCharPrevious = 0;

bool CHIDState::CKeyboard::s_baKeysDown[256];


SCF::UINT CHIDState::CMouse::s_uiButtonLastDown = 0;
SCF::UINT CHIDState::CMouse::s_uiButtonLastUp   = 0;

SCF::UINT CHIDState::CMouse::s_uiButtonPreviousDown = 0;
SCF::UINT CHIDState::CMouse::s_uiButtonPreviousUp   = 0;

bool CHIDState::CMouse::s_baButtonsDown[16];

Float2 CHIDState::CMouse::s_CursorPosition = { 0, 0 };
float CHIDState::CMouse::s_fWheelPosition = 0;

void CHIDState::Initialize()
{
	for (SCF::UINT i = 0; i < 256; i++) { CKeyboard::s_baKeysDown[i] = FALSE; }
	for (SCF::UINT i = 0; i < 16; i++)  { CMouse::s_baButtonsDown[i] = FALSE; }
}
