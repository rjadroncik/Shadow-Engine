#include "WindowManager.h"

using namespace GUI;

CWindowManager::CWindowManager() {}
CWindowManager::~CWindowManager() {}


void* CWindowManager::AsType(_IN ENUM eType) const
{
	switch (eType)
	{
	case ClassWindowManager: { return (CWindowManager*)this; }
	}

	return NULL;
}
