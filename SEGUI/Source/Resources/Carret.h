#pragma once

#include "../../../SECore/Source/SECore.h"
#include "../../../SECore/Source/System/SEObject.h"

#include "../Classes.h"

namespace Resources
{
	class SEGUI_API CCarret : public System::CSEObject
	{
	public:
		System::Category Category() _GET { return System::Category::Carrets; }

	public:
		CString XmlName() _GET { return STRING("Carret"); }
		CString ToString() _GET { return STRING("{Carret}"); }

	public:
		CCarret();
		virtual ~CCarret();

	public:
		//static inline SCF:UINT CarretUpTime()                     { return s_State.Carret.Rect; }
		//static inline void     CarretUpTime(_IN SCF:UINT uiValue) { CopyVector4(s_State.Carret.Rect, rRect); }

		//static inline void CarretTimerReset() { s_State.Carret.uiTimer = CarretUpTime(); s_State.Carret.bVisible = TRUE; }

		//static inline Float4& CarretRect()                  { return s_State.Carret.Rect; }
		//static inline void    CarretRect(_IN Float4& rRect) { CopyVector4(s_State.Carret.Rect, rRect); }

		//static inline bool CarretShow()               { return s_State.Carret.bShow; }
		//static inline void CarretShow(_IN bool bShow) { s_State.Carret.bShow = bShow; }

	public:
		void __fastcall Update();
		void __fastcall RenderAt(_IN Float2& rPosition) const;

	private:
		Float2 m_Size;

		UINT m_uiTimer;
		UINT m_uiUpTime;
	};
};