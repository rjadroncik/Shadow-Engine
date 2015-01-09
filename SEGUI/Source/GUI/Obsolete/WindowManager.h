#pragma once

#include "Container.h"

namespace GUI
{
	class CWindowManager : public CContainer
	{
	public:
		//Dynamic casting
		void* AsType(_IN ENUM eType) const; 
		ENUM ClassKey() _GET { return ClassWindowManager; }

	public:
		CWindowManager();
		virtual ~CWindowManager();

	public:


	};
};
