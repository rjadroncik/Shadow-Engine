#pragma once

#include "../../../Core/Source/Scripting/Proxy.h"

#include "../GUI/Layout.h"

namespace Scripting
{
	class GUI_API CProxyLayout : public CProxy
	{
	public:
		static void Register();
		static void PropertiesAdd(_OUT SCFObjectExtensions::CAccessibleClass& rOutClass);

	public:
		CProxyLayout();
		virtual ~CProxyLayout();

	public:
		//Return values CANNOT be deleted if [CProperty::IsReference()] returns true
		void     PropertySet(_IN ENUM eName, _IN CObject& rValue) _SET;
		CObject* PropertyGet(_IN ENUM eName)                      _GET;

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	protected:
		GUI::CLayout* m_pObject;

	private:
		struct SProperties
		{
			UINT uiRect;
			UINT uiSizeMin;
			UINT uiSizeMax;

			UINT uiSizeIdeal;

		} static s_Properties;
	};
};
