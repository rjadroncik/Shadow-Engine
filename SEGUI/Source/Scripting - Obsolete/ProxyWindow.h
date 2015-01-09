#pragma once

#include "../Scripting/ProxyElement.h"

#include "../GUI/Window.h"

namespace Scripting
{
	class SEGUI_API CProxyWindow : public CProxyElement
	{
	public:
		static void Register();
		static CXMLObjectSerializable* New() { return new CProxyWindow(); }
	
	public:
		SCF::ENUM ClassKey() _GET { return ClassProxyWindow; }

	public:
		CProxyWindow();
		virtual ~CProxyWindow();

	public:
		GUI::CWindow& Value() { return *(GUI::CWindow*)m_pObject; }

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	private:
		struct SProperties
		{
			SCF::UINT uiPosition;
			SCF::UINT uiRotation;

		} static s_Properties;
	};
};
