#pragma once

#include "../../../SECore/Source/Scripting/Proxy.h"

#include "../GUI/LayoutTable.h"

namespace Scripting
{
	class SEGUI_API CProxyLayoutTable : public CProxy
	{
	public:
		static void Register();
		static CXMLObjectSerializable* New() { return new CProxyLayoutTable(); }
	
	public:
		SCF::ENUM ClassKey() _GET { return ClassProxyLayoutTable; }

	public:
		CProxyLayoutTable();
		virtual ~CProxyLayoutTable();

	public:
		GUI::CLayoutTable& Value() { return *m_pObject; }

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	protected:
		GUI::CLayoutTable* m_pObject;
	};
};
