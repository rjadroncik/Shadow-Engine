#pragma once

#include "Light.h"
#include "Material.h"

#include "../Resources/Mesh.h"

namespace Rendering
{
	enum VertexParameterSources
	{
		VPS_LIGHT_POSITION = 0,   //Position in world coord. system
		VPS_LIGHT_POSITION_LOCAL, //Position in mesh/object coord. system
		VPS_LIGHT_PARAMETERS,     //Range, ??

		VPS_CAMERA_POSITION,       //Position in world coord. system
		VPS_CAMERA_POSITION_LOCAL, //Position in mesh/object coord. system
	};

	enum VertexParameterTargets
	{
		VPT_PROGRAM_LOCAL0 = 0,
		VPT_PROGRAM_LOCAL1,
		VPT_PROGRAM_LOCAL2,
		VPT_PROGRAM_LOCAL3,
		VPT_PROGRAM_LOCAL4,
		VPT_PROGRAM_LOCAL5,
		VPT_PROGRAM_LOCAL6,
		VPT_PROGRAM_LOCAL7,
		VPT_PROGRAM_LOCAL8,
		VPT_PROGRAM_LOCAL9,
		VPT_PROGRAM_LOCAL10,
		VPT_PROGRAM_LOCAL11,
		VPT_PROGRAM_LOCAL12,
		VPT_PROGRAM_LOCAL13,
		VPT_PROGRAM_LOCAL14,
		VPT_PROGRAM_LOCAL15,
	};

	class CMeshBinder : public CObject
	{
	public:
		//Dynamic casting
		void* AsType(_IN ENUM eType) const; 

	public:
		UINT    ClassKey()       _GET { return ClassMeshBinder; }
		CString ClassKeyString() _GET { return CString(STRING("MeshProxy")); }

		CMeshBinder(_IN Resources::CMesh& rMesh);
		virtual ~CMeshBinder();
	  
	public:
		inline const Resources::CMesh& Mesh() _GET { return *m_pMesh; }

	public:
		void __fastcall         Material(_IN _REF CMaterial& rMaterial) _SET;
		inline const CMaterial* Material()                              _GET { return m_pMaterial; }

		inline bool MaterialHas() _GET { if (m_pMaterial) { return TRUE; } return FALSE; }

		void __fastcall MaterialApply() const;
		void __fastcall MaterialAttributesBind() const;

	public:
		float __fastcall BoundsRadiusCompute() _GET;

	protected:
		const CMaterial*        m_pMaterial;
		const Resources::CMesh* m_pMesh;
	};
};