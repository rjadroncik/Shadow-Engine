#pragma once

#include "Element.h"

namespace GUI
{
	class SEGUI_API CLayout : public System::CSEObject
	{
	public:
		virtual void LayOut(_IN CVector<CElement>& rElements, _IN Float4& rRect, _OUT Float2& rOutSize) = 0;
	};
};