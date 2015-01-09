#pragma once

#include "../../../SECore/Source/Scripting/Proxy.h"

#include "../Resources/Cursor.h"

namespace Scripting
{
	class SEGUI_API CProxyCursor : public CProxy
	{
	public:
		static void Register();
		static CXMLObjectSerializable* New() { return new CProxyCursor(); }
	
	public:
		SCF::ENUM ClassKey() _GET { return ClassProxyCursor; }

	public:
		CProxyCursor();
		virtual ~CProxyCursor();

	public:
		Resources::CCursor& Value() { return *m_pObject; }

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	protected:
		Resources::CCursor* m_pObject;
	};
};
