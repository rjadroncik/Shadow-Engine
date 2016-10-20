#pragma once
#include "Light.h"
#include "Material.h"
#include "Camera.h"

#include "../Resources/Mesh.h"

namespace Rendering
{
	class SECORE_API CModel : public CObjectScene
	{
	public:
		System::Category Category() _GET { return System::Category::Models; }

	public:
		CString XmlName() _GET { return STRING("Model"); }
		CString ToString() _GET { return STRING("{Model}"); }

	public:
		CModel(_IN _REF Resources::CMesh& rMesh, _IN _REF CMaterial& rMaterial);
		virtual ~CModel();

	public:
		inline void ShadowCasts(_IN bool bCasts) _SET { m_bShadowCasts = bCasts; }
		inline bool ShadowCasts()                _GET { return m_bShadowCasts; }

	public:
		void __fastcall BoundsUpdate();

	public:
		inline const Resources::CMesh& Mesh()     _GET { return *m_pMesh; }
		inline const        CMaterial& Material() _GET { return *m_pMaterial; }

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	private:
		const CMaterial*        m_pMaterial;
		const Resources::CMesh* m_pMesh;

	private:
		bool m_bShadowCasts;

	private:
		CModel();
	};
};