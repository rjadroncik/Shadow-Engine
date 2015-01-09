#pragma once

#include "ProxyObjectScene.h"
#include "ProxyMesh.h"
#include "ProxyMaterial.h"

#include "../Rendering/Model.h"

namespace Scripting
{
	class SECORE_API CProxyModel : public CProxyObjectScene
	{
	public:
		static void Register();
		static CXMLObjectSerializable* New() { return new CProxyModel(); }

	public:
		SCF::ENUM ClassKey() _GET { return ClassProxyModel; }
		CString   AsString() _GET { return STRING("{ProxyModel}"); }

	public:
		CProxyModel();
		virtual ~CProxyModel();

	public:
		void   ShadowCastsSet(_IN CBool& rCasts) _SET { ((Rendering::CModel*)m_pObject)->ShadowCasts(rCasts.Value()); }
		CBool* ShadowCasts()                     _GET { return new CBool(((Rendering::CModel*)m_pObject)->ShadowCasts()); }

	public:
		CProxyMesh*     Mesh()     _GET { return m_pMesh; }
		CProxyMaterial* Material() _GET { return m_pMaterial; }

	public:
		void BoundsUpdate() { ((Rendering::CModel*)m_pObject)->BoundsUpdate(); }

	public:
		Rendering::CModel& Value() { return *(Rendering::CModel*)m_pObject; }

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	protected:
		CProxyMesh*     m_pMesh;
		CProxyMaterial* m_pMaterial;
	};
};
