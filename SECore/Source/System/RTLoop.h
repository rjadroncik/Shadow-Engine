#pragma once

#include "../SECore.h"

namespace System
{
	typedef void (*HANDLER_UPDATE)();

	class SECORE_API CRTLoop
	{
	public:
		static void           HandlerUpdateStart(_IN HANDLER_UPDATE pHandler) { s_Handlers.pUpdateStart = pHandler; }
		static HANDLER_UPDATE HandlerUpdateStart()                            { return s_Handlers.pUpdateStart; }

		static void           HandlerUpdateEnd(_IN HANDLER_UPDATE pHandler) { s_Handlers.pUpdateEnd = pHandler; }
		static HANDLER_UPDATE HandlerUpdateEnd()                            { return s_Handlers.pUpdateEnd; }

	public:
		static void Start();
		static void Next();

		static void Pause()  { s_State.bRunning = FALSE; }
		static void Resume() { s_State.bRunning = TRUE; }

		static inline bool IsRunning() { return s_State.bRunning; }

	public:
		static inline SCF::UINT	UpdatesSinceRender() { return s_State.uiUpdatesSinceRender; }

		static inline float	UpdateTime() { return s_State.Timing.fUpdateTime; }
		static inline float	RenderTime() { return s_State.Timing.fRenderTime; }

	private:
		static void Update();
		static void Render();

	private:
		struct SState 
		{
			bool bRunning;

			struct STiming
			{
				SCF::UINT uiUpdateCount;

				float fLag;

				float fUpdateTime;
				float fRenderTime;

			} Timing;

			struct SStatistics
			{
				SCF::UINT uiUpdateCountPS;
				SCF::UINT uiRenderCountPS;

				float fUpdateTimePS;
				float fRenderTimePS;

			} Statistics;

			SCF::UINT uiUpdatesSinceRender;

		} static s_State;

	private:
		struct SHandlers
		{
			HANDLER_UPDATE pUpdateStart;
			HANDLER_UPDATE pUpdateEnd;

		} static s_Handlers;

	private:
		CRTLoop();
		~CRTLoop();
	};
};