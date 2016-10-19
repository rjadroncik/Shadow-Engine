#pragma once

#include "../../../SECore/Source/SECore.h"
#include "../../../SECore/Source/System/SEObject.h"

#include "../../../SECore/Source/Resources/Texture.h"

#include "../Classes.h"

namespace Resources
{
	class SEGUI_API CCursor : public System::CSEObject
	{
	public:
		CString XmlName() _GET { return STRING("Cursor"); }
		CString ToString() _GET { return STRING("{Cursor}"); }

	public:
		CCursor(_IN CTexture& rTexture);
 		virtual ~CCursor();

	public:
		const Float2& Size()                  _GET { return m_Size; }
		void          Size(_IN Float2& rSize) _SET { m_Size[0] = rSize[0]; m_Size[1] = rSize[1]; }

		const Float2& HotSpot()                     _GET { return m_HotSpot; }
		void          HotSpot(_IN Float2& rHotSpot) _SET { m_HotSpot[0] = rHotSpot[0]; m_HotSpot[1] = rHotSpot[1]; }

	public:
		void __fastcall RenderAt(_IN Float2& rPosition) const;

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	protected:
		const CTexture* m_pTexture;

		Float2 m_Size;
		Float2 m_HotSpot;

	private:
		CCursor();
	};
};
