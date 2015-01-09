#pragma once

#include "ProxyObjectScene.h"

#include "../Rendering/Light.h"

namespace Scripting
{
	class SECORE_API CProxyLight : public CProxyObjectScene
	{
	public:
		static void Register();

	public:
		CProxyLight();
		virtual ~CProxyLight();

	public:
		void Shadows(_IN bool bCastShadows) _SET { ((Rendering::CLight*)m_pObject)->Shadows(bCastShadows); }
		bool Shadows()                      _GET { return ((Rendering::CLight*)m_pObject)->Shadows(); }

		void Enabled(_IN bool bEnabled) _SET { ((Rendering::CLight*)m_pObject)->Enabled(bEnabled); }
		bool Enabled()                  _GET { return ((Rendering::CLight*)m_pObject)->Enabled(); }

	public:
		void  Range(_IN float fRange) _SET { ((Rendering::CLight*)m_pObject)->Range(fRange); }
		float Range()                 _GET { return ((Rendering::CLight*)m_pObject)->Range(); }

	public:
		void          Tint(_IN Float3& rTint) _SET { ((Rendering::CLight*)m_pObject)->Tint(rTint); }	
		const Float3& Tint()                  _GET { return ((Rendering::CLight*)m_pObject)->Tint(); }

		void  Ambient(_IN float fValue) _SET { ((Rendering::CLight*)m_pObject)->Ambient(fValue); }
		float Ambient()                 _GET { return ((Rendering::CLight*)m_pObject)->Ambient(); }

		void  Diffuse(_IN float fValue) _SET { ((Rendering::CLight*)m_pObject)->Diffuse(fValue); }
		float Diffuse()                 _GET { return ((Rendering::CLight*)m_pObject)->Diffuse(); }
	
		void  Specular(_IN float fValue) _SET { ((Rendering::CLight*)m_pObject)->Specular(fValue); }
		float Specular()                 _GET { return ((Rendering::CLight*)m_pObject)->Specular(); }

	public:
		Rendering::CLight& Value() { return *(Rendering::CLight*)m_pObject; }

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	public:
		CString AsString() _GET { return STRING("{ProxyLight}"); }
	};
};
