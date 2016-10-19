#include "InputHandlerGUI.h"

void CInputHandlerGUI::InputHandler()
{
	//No scene or camera -> no go :D
/*	if (!Simulation::CSimulation::Scene() || !Simulation::CSimulation::Scene()->CameraCurrent()) { return; }

	if ((CDirectInput::MouseOffsetX() != 0) || (CDirectInput::MouseOffsetY() != 0))
	{
		s_Objects.GUI.pModule->CursorMove(CDirectInput::MouseOffsetX(), CDirectInput::MouseOffsetY());
	}

	if (s_Objects.GUI.Element.bDragging) { InputHandlerGUIDragging(); }
	else                                 { InputHandlerGUINormal(); }*/
}

void CInputHandlerGUI::KeyDownHandler(UINT uiVKey, UINT uiRepeatCount)
{
/*	if (uiVKey == VK_F1)
	{
		bool bEnabled = CEventLog::Enabled();
		CEventLog::Enabled(FALSE);

		Resources::CManager::ObjectsDelete();
		Resources::CDataPool::Rebuild();

		CInputHandlerGUI::Initialize();

#ifdef _DEBUG
		{
			CTrackerObject TrackerObject;
			CTrackerMemory TrackerMemory;
			TrackerObject.Enabled(TRUE);
			TrackerMemory.Enabled(TRUE);

			//UINT64 uiAllocatedBytes01 = CMemory::AllocatedBytes();
			for (UINT i = 0; i < CSettings::Resources::DefinitionFiles().Size(); i++)
			{
				Resources::CManager::ObjectsCreate((CString&)CSettings::Resources::DefinitionFiles()[i]);
			}
			Resources::CManager::ObjectsDelete();
			//UINT64 uiAllocatedBytes02 = CMemory::AllocatedBytes();

			TrackerObject.Enabled(FALSE);
			TrackerMemory.Enabled(FALSE);

			CStreamFileWrite StreamWrite(STRING("ObejctsLeakedAtRebuild.txt"));
			CStreamWriteTextUCS2 StreamWriteText(StreamWrite);

			StreamWriteText.PutLine("Object leaks:");

			CEnumeratorBag EnumeratorObjects(TrackerObject.Objects());
			while (EnumeratorObjects.Next())
			{
				CObject* pObject = EnumeratorObjects.Current();
				StreamWriteText.PutString(CEnum(pObject->ClassKey()).ToString());
				StreamWriteText.PutString(STRING(": "));
				StreamWriteText.PutLine(pObject->ToString());
			}

			StreamWriteText.PutLine("Memory leaks:");
			UINT uiSizeLeaked = 0;

			CEnumeratorBag EnumeratorBlocks(TrackerMemory.Blocks());
			while (EnumeratorBlocks.Next())
			{
				void* vpCurrent = (void*)EnumeratorBlocks.Current();
				uiSizeLeaked += CMemory::SizeOf(vpCurrent);

				StreamWriteText.PutLine(CPointer(vpCurrent).ToString());
				CMemory::Free(vpCurrent);
			}

			StreamWriteText.PutString("Memory leaked:");
			StreamWriteText.PutString(CInt(uiSizeLeaked).ToString());
			StreamWriteText.Close();
		}
#endif

		for (UINT i = 0; i < CSettings::Resources::DefinitionFiles().Size(); i++)
		{
			Resources::CManager::ObjectsCreate((CString&)CSettings::Resources::DefinitionFiles()[i]);
		}

		Simulation::CSimulation::Scene(&((Scripting::CScene*) Scripting::CScripting::Object(CSettings::StartupScene()))->Value());
		GUI::CInputHandlerGUI::GUIModule   (&((Scripting::CModule*)Scripting::CScripting::Object(CSettings::StartupGUIModule()))->Value());

		CSystem::Update();
		Simulation::CSimulation::Start();

		CEventLog::Enabled(bEnabled);
	}*/

/*	if (uiVKey == VK_OEM_3) //'~' key
	{
		if (s_Objects.GUI.pModule->CursorShow()) 
		{
			CUserInterface::InputHandler(CUserInterface::InputHandler3DFPS);

			CDirectInput::MouseSensitivity(CSettings::UI::MouseSensitivity3DFPS());
			s_Objects.GUI.pModule->CursorShow(FALSE);
			CUserInterface::Keyboard::TypingEnabled(FALSE);

			s_Objects.GUI.Element.pFocused = NULL;
		}
		else
		{
			CUserInterface::InputHandler(CInputHandlerGUI::InputHandlerGUI);

			CDirectInput::MouseSensitivity(CSettings::UI::MouseSensitivityGUI());
			s_Objects.GUI.pModule->CursorShow(TRUE);
			CUserInterface::Keyboard::TypingEnabled(TRUE);
		}
		return;
	}*/
}

void CInputHandlerGUI::KeyUpHandler(UINT uiVKey, UINT uiRepeatCount)
{
}

void CInputHandlerGUI::TypeHandler(UINT uiChar, UINT uiRepeatCount)
{
}


/*
void CDispatcher::RenderHandler()
{
	static UINT uiFPS = 0;
	static UINT uiRPS = 0;
	static UINT uiUPS = 0;

	//Frame Render Time & Frame Update Time in micro-seconds
	static UINT uiFRT = 0;
	static UINT uiFUT = 0;

	static UINT uiDisplayUpdateCounter = Simulation::CSimulation::Settings::UpdateRate() - 1;

	static float fRenderTimeSum = 100000000000.0f;
	static float fUpdateTimeSum = 100000000000.0f;

	if (uiDisplayUpdateCounter == Simulation::CSimulation::Settings::UpdateRate())
	{
		//Renders Per Second, Updates Per Second 
		uiRPS = (int)(Simulation::CSimulation::Settings::UpdateRate() / fRenderTimeSum);
		uiUPS = (int)(Simulation::CSimulation::Settings::UpdateRate() / fUpdateTimeSum);

		//Frames Per Second
		uiFPS = (int)(Simulation::CSimulation::Settings::UpdateRate() / (fRenderTimeSum + fUpdateTimeSum));

		//Frame Render Time & Frame Update Time in micro-seconds
		uiFRT = (int)(fRenderTimeSum * (1000000 / Simulation::CSimulation::Settings::UpdateRate()));
		uiFUT = (int)(fUpdateTimeSum * (1000000 / Simulation::CSimulation::Settings::UpdateRate()));

		uiDisplayUpdateCounter = 0;

		fRenderTimeSum = 0;
		fUpdateTimeSum = 0;

		         CLabel* s_pMemoryUsageValue = &(((CLabel*)CScripting::Object(STRING("Label.System.Memory_Usage_Value")))->Value());
		BETAONLY(CLabel* s_pObjectCountValue = &(((CLabel*)CScripting::Object(STRING("Label.System.Object_Count_Value")))->Value());)
		         CLabel* s_pBlockCountValue  = &(((CLabel*)CScripting::Object(STRING("Label.System.Block_Count_Value")))->Value());

		         if (s_pMemoryUsageValue) { s_pMemoryUsageValue->Text().String(CInt64(CMemory::AllocatedBytes() + SCFPrivate::CFSBHeap::AllocatedBytesTotal()).ToString()); }
		BETAONLY(if (s_pObjectCountValue) { s_pObjectCountValue->Text().String(CInt(CObject::TotalSystemCount()).ToString()); })
		         if (s_pBlockCountValue)  { s_pBlockCountValue->Text().String(CInt(CMemoryBlock::Count()).ToString()); }

		CLabel* s_pRPSValue = &(((CLabel*)CScripting::Object(STRING("Label.Statistics.RPS_Value")))->Value());
		CLabel* s_pUPSValue = &(((CLabel*)CScripting::Object(STRING("Label.Statistics.UPS_Value")))->Value());

		if (s_pRPSValue) { s_pRPSValue->Text().String(CInt(uiRPS).ToString()); }
		if (s_pUPSValue) { s_pUPSValue->Text().String(CInt(uiUPS).ToString()); }
	}
	else
	{
		uiDisplayUpdateCounter++;

		fRenderTimeSum += Simulation::CSimulation::RenderTime();
		fUpdateTimeSum += Simulation::CSimulation::UpdateTime();
	}

	CLabel* s_pShadowsValue = &(((CLabel*)CScripting::Object(STRING("Label.Optimizations.Shadows_Processed_Value")))->Value());
	CLabel* s_pObjectsValue = &(((CLabel*)CScripting::Object(STRING("Label.Optimizations.Objects_Processed_Value")))->Value());
	CLabel* s_pLightsValue  = &(((CLabel*)CScripting::Object(STRING("Label.Optimizations.Lights_Processed_Value")))->Value());

	if (s_pShadowsValue) { s_pShadowsValue->Text().String(CInt(CStatistics::CRendering::ProcessedShadows()).ToString()); }
	if (s_pObjectsValue) { s_pObjectsValue->Text().String(CInt(CStatistics::CRendering::ProcessedObjects()).ToString()); }
	if (s_pLightsValue)  { s_pLightsValue->Text().String (CInt(CStatistics::CRendering::ProcessedLights ()).ToString()); }
}
*/