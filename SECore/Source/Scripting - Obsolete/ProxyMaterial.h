#pragma once

#include "Proxy.h"

#include "../Rendering/Material.h"

namespace Scripting
{
	class SECORE_API CProxyMaterial : public CProxy
	{
	public:
		static void Register();
		static CXMLObjectSerializable* New() { return new CProxyMaterial(); }

	public:
		SCF::ENUM ClassKey() _GET { return ClassProxyMaterial; }
		CString   AsString() _GET { return STRING("{ProxyMaterial}"); }

	public:
		CProxyMaterial();
		virtual ~CProxyMaterial();

	public:
		void     TintSet(_IN CFloat3& rTint) _SET { ((Rendering::CMaterial*)m_pObject)->Tint(rTint.Value()); }	
		CFloat3* Tint()                      _GET { return new CFloat3(((Rendering::CMaterial*)m_pObject)->Tint()); }

		void    AmbientSet(_IN CFloat& rValue) _SET { ((Rendering::CMaterial*)m_pObject)->Ambient(rValue.Value()); }
		CFloat* Ambient()                      _GET { return new CFloat(((Rendering::CMaterial*)m_pObject)->Ambient()); }

		void    DiffuseSet(_IN CFloat& rValue) _SET { ((Rendering::CMaterial*)m_pObject)->Diffuse(rValue.Value()); }
		CFloat* Diffuse()                      _GET { return new CFloat(((Rendering::CMaterial*)m_pObject)->Diffuse()); }

		void    SpecularSet(_IN CFloat& rValue) _SET { ((Rendering::CMaterial*)m_pObject)->Specular(rValue.Value()); }
		CFloat* Specular()                      _GET { return new CFloat(((Rendering::CMaterial*)m_pObject)->Specular()); }
	
		void    EmissionSet(_IN CFloat& rValue) _SET { ((Rendering::CMaterial*)m_pObject)->Emission(rValue.Value()); }
		CFloat* Emission()                      _GET { return new CFloat(((Rendering::CMaterial*)m_pObject)->Emission()); }
		
		void    ShininessSet(_IN CFloat& rValue) _SET { ((Rendering::CMaterial*)m_pObject)->Shininess(rValue.Value()); }
		CFloat* Shininess()                      _GET { return new CFloat(((Rendering::CMaterial*)m_pObject)->Shininess()); }
		
	public:
		void   TransparentSet(_IN CBool& rValue) _SET { ((Rendering::CMaterial*)m_pObject)->Transparent(rValue.Value()); }
		CBool* Transparent()                     _GET { return new CBool(((Rendering::CMaterial*)m_pObject)->Transparent()); }

		void   FactorSourceSet(_IN CEnum& rValue) _SET { ((Rendering::CMaterial*)m_pObject)->FactorSource(rValue.Value()); }
		CEnum* FactorSource()                     _GET { return new CEnum(((Rendering::CMaterial*)m_pObject)->FactorSource()); }

		void   FactorDestinationSet(_IN CEnum& rValue) _SET { ((Rendering::CMaterial*)m_pObject)->FactorDestination(rValue.Value()); }
		CEnum* FactorDestination()                     _GET { return new CEnum(((Rendering::CMaterial*)m_pObject)->FactorDestination()); }

	public:
		Rendering::CMaterial& Value() { return *m_pObject; }

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	protected:
		Rendering::CMaterial* m_pObject;
	};
};
