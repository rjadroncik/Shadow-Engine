#pragma once

#include "../../../SECore/Source/Scripting/Proxy.h"

#include "../Resources/Font.h"

namespace Scripting
{
	class SEGUI_API CProxyFont : public CProxy
	{
	public:
		static void Register();
		static CXMLObjectSerializable* New() { return new CProxyFont(); }
	
	public:
		SCF::ENUM ClassKey() _GET { return ClassProxyFont; }

	public:
		CProxyFont();
		virtual ~CProxyFont();

	public:
		Resources::CFont& Value() { return *m_pObject; }

	public:
		//Return values CANNOT be deleted if [CProperty::IsReference()] returns true
		void     PropertySet(_IN SCF::ENUM eName, _IN CObject& rValue) _SET;
		CObject* PropertyGet(_IN SCF::ENUM eName)                      _GET;

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	protected:
		Resources::CFont* m_pObject;
	};
};
