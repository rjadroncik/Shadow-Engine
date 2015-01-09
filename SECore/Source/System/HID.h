#pragma once

#include "EventLog.h"

namespace System
{
	typedef void (*HANDLER_INPUT)();

	class SECORE_API CHID
	{
	public:
		static void Initialize();

	public:
		class SECORE_API CKeyboard
		{
		public:
			//Windows keyboard message handlers
			static void          HandlerKeyDown(_IN HANDLER_INPUT pHandler) { s_Handlers.pKeyDown = pHandler; }
			static HANDLER_INPUT HandlerKeyDown()                           { return s_Handlers.pKeyDown; }

			static void          HandlerKeyUp(_IN HANDLER_INPUT pHandler)   { s_Handlers.pKeyUp = pHandler; }
			static HANDLER_INPUT HandlerKeyUp()                             { return s_Handlers.pKeyUp; }

			static void          HandlerChar(_IN HANDLER_INPUT pHandler)    { s_Handlers.pChar = pHandler; }
			static HANDLER_INPUT HandlerChar()                              { return s_Handlers.pChar; }

		public:
			static void TypingEnabled(_IN bool bEnabled) { s_State.bTypingEnabled = bEnabled; }
			static bool TypingEnabled()                  { return s_State.bTypingEnabled; }

		private:
			struct SHandlers
			{
				HANDLER_INPUT pKeyDown;
				HANDLER_INPUT pKeyUp;
				HANDLER_INPUT pChar;

			} static s_Handlers;

		private:
			struct SState 
			{
				bool bTypingEnabled;

			} static s_State;
		};

	public:
		class SECORE_API CMouse
		{
		public:
			//Windows mouse message handlers
			static void          HandlerButtonDown(_IN HANDLER_INPUT pHandler) { s_Handlers.pButtonDown = pHandler; }
			static HANDLER_INPUT HandlerButtonDown()                           { return s_Handlers.pButtonDown; }

			static void          HandlerButtonUp(_IN HANDLER_INPUT pHandler)   { s_Handlers.pButtonUp = pHandler; }
			static HANDLER_INPUT HandlerButtonUp()                             { return s_Handlers.pButtonUp; }

			static void          HandlerMove(_IN HANDLER_INPUT pHandler)   { s_Handlers.pMove = pHandler; }
			static HANDLER_INPUT HandlerMove()                             { return s_Handlers.pMove; }

		public:
			static void BalisticsEnabled(_IN bool bEnabled) { s_State.bBalisticsEnabled = bEnabled; }
			static bool BalisticsEnabled()                  { return s_State.bBalisticsEnabled; }

		private:
			struct SHandlers
			{
				HANDLER_INPUT pButtonDown;
				HANDLER_INPUT pButtonUp;
				HANDLER_INPUT pMove;

			} static s_Handlers;

		private:
			struct SState 
			{
				bool bBalisticsEnabled;

			} static s_State;
		};		

	private:
		CHID();
		~CHID();
	};
};
