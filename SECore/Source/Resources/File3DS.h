#pragma once

#include "../SECore.h"
#include "../System/SEObject.h"

#include "Mesh.h"

namespace Resources
{
	enum ChunkIDs
	{
		ChunkMain                   = 0x4D4D,
		Chunk3DEditor               = 0x3D3D,
		ChunkObjectBlock            = 0x4000,
		ChunkTriangularMesh         = 0x4100,
		ChunkVerticesList           = 0x4110,
		ChunkFacesDescription       = 0x4120,
		ChunkFacesMaterial          = 0x4130,
		ChunkMappingCoordinatesList = 0x4140,
		ChunkSmoothingGroupList     = 0x4150,
		ChunkLocalCoordinatesSystem = 0x4160,
	};

	class SECORE_API CFile3DS : public System::CSEObject
	{
	public:
		System::Category Category() _GET { return System::Category::Meshes; }

	public:
		CString XmlName() _GET { return STRING("File3DS"); }
		CString ToString() _GET { return STRING("{File3DS}"); }

	public:
		CFile3DS(_INOUT IStreamRead& rStream);
		virtual ~CFile3DS();

	public:
		bool ConvertTo(_INOUT CMesh& rMesh);

	protected:
		bool Next(_INOUT IStreamRead& rStream);

	protected:
		bool ChunkProcessMain                  (_INOUT IStreamRead& rStream);
		bool ChunkProcess3DEditor              (_INOUT IStreamRead& rStream);
		bool ChunkProcessObjectBlock           (_INOUT IStreamRead& rStream);
		bool ChunkProcessTriangularMesh        (_INOUT IStreamRead& rStream);
		bool ChunkProcessVerticesList          (_INOUT IStreamRead& rStream);
		bool ChunkProcessFacesDescription      (_INOUT IStreamRead& rStream);
		bool ChunkProcessFacesMaterial         (_INOUT IStreamRead& rStream);
		bool ChunkProcessMappingCoordinatesList(_INOUT IStreamRead& rStream);
		bool ChunkProcessSmoothingGroupList    (_INOUT IStreamRead& rStream);
		bool ChunkProcessLocalCoordinatesSystem(_INOUT IStreamRead& rStream);

	protected:
		struct SObject 
		{
			CString* pName;

			Float3* pVertices;
			USHORT  usVertexCount;

			Short3* pTriangles;
			USHORT  usTriangleCount;
		
			Float2* pTexCoords;
			USHORT  usTexCoordCount;
			
			//Per-triangle smooth group mask
			UINT*   pSmoothGroups;

		}* m_pObjects;

		UINT m_uiObjectCount;
	};
};