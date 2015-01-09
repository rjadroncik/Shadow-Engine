#pragma once

#include "../Scripting/ProxyElement.h"

#include "../GUI/Window.h"

namespace Scripting
{
	class GUI_API CProxyText : public CProxyElement
	{
	public:
		static void Register();
		static CObject* New() { return new CProxyText(); }
	
	public:
		//Dynamic casting
		void* AsType(_IN ENUM eType) const; 
		ENUM ClassKey() _GET { return ClassProxyText; }

	public:
		CProxyText();
		virtual ~CProxyText();

	public:
		GUI::CWindow& Value() { return *m_pObject; }

	public:
		//Return values CANNOT be deleted if [CProperty::IsReference()] returns true
		void     PropertySet(_IN ENUM eName, _IN CObject& rValue) _SET;
		CObject* PropertyGet(_IN ENUM eName)                      _GET;

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	protected:
		GUI::CWindow* m_pObject;

	private:
		struct SProperties
		{
			UINT uiPosition;
			UINT uiRotation;

		} static s_Properties;
	};
};
