#include "HID.h"

#include "System.h"
#include "../Simulation/Simulation.h"

#include "Settings.h"
#include "HID.h"

using namespace System;
using namespace Resources;

CHID::CKeyboard::SHandlers CHID::CKeyboard::s_Handlers = { NULL, NULL, NULL };
CHID::CKeyboard::SState    CHID::CKeyboard::s_State    = { TRUE };

CHID::CMouse::SHandlers CHID::CMouse::s_Handlers = { NULL, NULL, NULL };
CHID::CMouse::SState    CHID::CMouse::s_State    = { TRUE };

void CHID::Initialize()
{

}