#pragma once

#include "../../../SECore/Source/SECore.h"

class CInputHandlerGUI
{
public:
	static void InputHandler();

	static void KeyDownHandler(SCF::UINT uiVKey, SCF::UINT uiRepeatCount);
	static void KeyUpHandler  (SCF::UINT uiVKey, SCF::UINT uiRepeatCount);
	static void TypeHandler   (SCF::UINT uiChar, SCF::UINT uiRepeatCount);
};