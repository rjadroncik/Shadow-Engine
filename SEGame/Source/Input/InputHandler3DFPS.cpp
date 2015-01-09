#include "InputHandler3DFPS.h"

void CInputHandler3DFPS::InputHandler()
{
	//No scene or camera -> no go :D
/*	if (!Simulation::CSimulation::Scene() || !Simulation::CSimulation::Scene()->CameraCurrent()) { return; }

	_PENDING;
	//if (!s_State.Cursor.bShow)
	{
		if ((CDirectInput::MouseOffsetX() != 0) || (CDirectInput::MouseOffsetY() != 0))
		{
			Simulation::CSimulation::Scene()->CameraCurrent()->RotationOffset(-CDirectInput::MouseOffsetY(), -CDirectInput::MouseOffsetX(), 0);
			Simulation::CSimulation::Scene()->CameraCurrent()->Changed(TRUE);
		}
	}

	if (!CDirectInput::KeyDown(DIK_LCONTROL) && !CDirectInput::KeyDown(DIK_RCONTROL))
	{
		if (Simulation::CSimulation::Scene()->CameraCurrent())
		{
			if (CDirectInput::KeyDown(DIK_W)) { Simulation::CSimulation::Scene()->CameraCurrent()->MoveForward( 1200.0f / Simulation::CSimulation::Settings::UpdateRate()); Simulation::CSimulation::Scene()->CameraCurrent()->Changed(TRUE); }
			if (CDirectInput::KeyDown(DIK_S)) { Simulation::CSimulation::Scene()->CameraCurrent()->MoveForward(-1200.0f / Simulation::CSimulation::Settings::UpdateRate()); Simulation::CSimulation::Scene()->CameraCurrent()->Changed(TRUE); }

			if (CDirectInput::KeyDown(DIK_A)) { Simulation::CSimulation::Scene()->CameraCurrent()->MoveRight  (-1200.0f / Simulation::CSimulation::Settings::UpdateRate()); Simulation::CSimulation::Scene()->CameraCurrent()->Changed(TRUE); }
			if (CDirectInput::KeyDown(DIK_D)) { Simulation::CSimulation::Scene()->CameraCurrent()->MoveRight  ( 1200.0f / Simulation::CSimulation::Settings::UpdateRate()); Simulation::CSimulation::Scene()->CameraCurrent()->Changed(TRUE); }

			if (CDirectInput::KeyDown(DIK_Q)) { Simulation::CSimulation::Scene()->CameraCurrent()->MoveUp     (-1200.0f / Simulation::CSimulation::Settings::UpdateRate()); Simulation::CSimulation::Scene()->CameraCurrent()->Changed(TRUE); }
			if (CDirectInput::KeyDown(DIK_E)) { Simulation::CSimulation::Scene()->CameraCurrent()->MoveUp     ( 1200.0f / Simulation::CSimulation::Settings::UpdateRate()); Simulation::CSimulation::Scene()->CameraCurrent()->Changed(TRUE); }
		}
	}

	if (CDirectInput::KeyPressed(DIK_SPACE))
	{														   
		if (Simulation::CSimulation::IsRunning()) { Simulation::CSimulation::Pause(); }
		else                                      { Simulation::CSimulation::Resume(); }    
	}*/
}
