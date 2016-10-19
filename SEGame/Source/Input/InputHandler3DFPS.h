#pragma once

#include "../../../SECore/Source/SECore.h"

class CInputHandler3DFPS
{
public:
	static void InputHandler();

	static void KeyDownHandler(UINT uiVKey, UINT uiRepeatCount);
	static void KeyUpHandler  (UINT uiVKey, UINT uiRepeatCount);
	static void TypeHandler   (UINT uiChar, UINT uiRepeatCount);
};