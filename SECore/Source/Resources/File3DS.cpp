#include "File3DS.h"

#include <SCFWinAPIUndef.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <lib3ds\file.h>
#include <lib3ds\mesh.h>

using namespace Resources;

CFile3DS::CFile3DS(_INOUT IStreamRead& rStream)
{
	m_pObjects = NULL;
	m_uiObjectCount = 0;

	Next(rStream);
}

CFile3DS::~CFile3DS()
{
	if (m_pObjects)
	{
		for (SCF::UINT i = 0; i < m_uiObjectCount; i++)
		{
			delete m_pObjects[i].pName;

			if (m_pObjects[i].pVertices)     { CMemory::Free(m_pObjects[i].pVertices); }
			if (m_pObjects[i].pTexCoords)    { CMemory::Free(m_pObjects[i].pTexCoords); }
			if (m_pObjects[i].pTriangles)    { CMemory::Free(m_pObjects[i].pTriangles); }
			if (m_pObjects[i].pSmoothGroups) { CMemory::Free(m_pObjects[i].pSmoothGroups); }
		}

		CMemory::Free(m_pObjects);
	}
}

bool CFile3DS::ConvertTo(_INOUT CMesh& rMesh)
{
	if (m_pObjects)
	{
		rMesh.Vertices (m_pObjects[0].pVertices,  m_pObjects[0].usVertexCount);
		rMesh.Triangles(m_pObjects[0].pTriangles, m_pObjects[0].usTriangleCount);

		rMesh.AttributeAdd(AS_VERTEX_TEXCOORD0, m_pObjects[0].pTexCoords, 2);


		SVertexList TempList = { 0, NULL };

		for (SCF::USHORT i = 0; i < m_pObjects[0].usTriangleCount; i++)
		{
			SCF::UINT dwMask = 0x1;

			for (SCF::USHORT j = 0; j < 32; j++)
			{
				if (m_pObjects[0].pSmoothGroups[i] & dwMask)
				{
					CMesh::AddIndex(TempList, (SCF::USHORT)dwMask);
				}

				dwMask <<= 1;
			}
		}


		rMesh.m_usSmoothingGroupCount = TempList.usCount;
		rMesh.m_pSmoothingGroups = (SVertexList*)CMemory::Reallocate(rMesh.m_pSmoothingGroups, sizeof(SVertexList) * rMesh.m_usSmoothingGroupCount);

		ZeroMemory(rMesh.m_pSmoothingGroups, sizeof(SVertexList) * rMesh.m_usSmoothingGroupCount);

		for (SCF::USHORT i = 0; i < rMesh.m_usSmoothingGroupCount; i++)
		{
			for (SCF::USHORT j = 0; j < m_pObjects[0].usTriangleCount; j++)
			{
				if (m_pObjects[0].pSmoothGroups[j] & TempList.uspTriangles[i]) 
				{ 
					CMesh::AddIndex(rMesh.m_pSmoothingGroups[i], m_pObjects[0].pTriangles[j][0]); 
					CMesh::AddIndex(rMesh.m_pSmoothingGroups[i], m_pObjects[0].pTriangles[j][1]); 
					CMesh::AddIndex(rMesh.m_pSmoothingGroups[i], m_pObjects[0].pTriangles[j][2]); 
				}
			}
		}

		//!!!!!!!!!!!!!!!!!!!!!!!!!!  IMPORTANT  !!!!!!!!!!!!!!!!!!!!!!!!!!!!
		CMemory::Free(TempList.uspTriangles);

		rMesh.m_usSeamVertexListCount = rMesh.m_usVertexCount;
		rMesh.m_pSeamVertices = (SVertexList*)CMemory::Reallocate(rMesh.m_pSeamVertices, sizeof(SVertexList) * rMesh.m_usSeamVertexListCount);

		ZeroMemory(rMesh.m_pSeamVertices, sizeof(SVertexList) * rMesh.m_usSeamVertexListCount);

		//Go through all vertices
		for (SCF::USHORT i = 0; i < rMesh.m_usVertexCount; i++)
		{
			for (SCF::USHORT j = (i + 1); j < rMesh.m_usVertexCount; j++)
			{
				if (CompareVectors3(rMesh.m_pVertices[i], rMesh.m_pVertices[j]))
				{
					CMesh::AddIndex(rMesh.m_pSeamVertices[i], i);
					CMesh::AddIndex(rMesh.m_pSeamVertices[i], j);
				}
			}
		}
		
		//Unbind arrays from 3ds file
		m_pObjects[0].pVertices  = NULL;
		m_pObjects[0].pTriangles = NULL;
		m_pObjects[0].pTexCoords = NULL;
	}

	//rMesh.BoundsUpdate();

	rMesh.BufferVertexEnable(GL_STATIC_DRAW);
	rMesh.BufferIndexEnable (GL_STATIC_DRAW);
	rMesh.BufferAttributeEnable(AS_VERTEX_TEXCOORD0, GL_STATIC_DRAW);

	return TRUE;
}

bool CFile3DS::Next(_INOUT IStreamRead& rStream)
{
	if (rStream.BytesLeft())
	{
		SCF::USHORT usID     = rStream.GetWord();
		SCF::UINT   uiLength = rStream.GetInt();

		switch (usID)
		{
		case ChunkMain:                   { return ChunkProcessMain                  (rStream); }
		case Chunk3DEditor:               { return ChunkProcess3DEditor              (rStream); }
		case ChunkObjectBlock:            { return ChunkProcessObjectBlock           (rStream); }
		case ChunkTriangularMesh:         { return ChunkProcessTriangularMesh        (rStream); }
		case ChunkVerticesList:           { return ChunkProcessVerticesList          (rStream); }
		case ChunkFacesDescription:       { return ChunkProcessFacesDescription      (rStream); }
		//case ChunkFacesMaterial:          { return ChunkProcessFacesMaterial         (rStream); }
		case ChunkMappingCoordinatesList: { return ChunkProcessMappingCoordinatesList(rStream); }
		case ChunkSmoothingGroupList:     { return ChunkProcessSmoothingGroupList    (rStream); }
		//case ChunkLocalCoordinatesSystem: { return ChunkProcessLocalCoordinatesSystem(rStream); }

		default: { rStream.SkipBytes(uiLength - 6); return Next(rStream); }
		}
	}
	else { return TRUE; }
}

bool CFile3DS::ChunkProcessMain(_INOUT IStreamRead& rStream)
{
	return Next(rStream);
}
bool CFile3DS::ChunkProcess3DEditor(_INOUT IStreamRead& rStream)
{
	return Next(rStream);
}
bool CFile3DS::ChunkProcessObjectBlock(_INOUT IStreamRead& rStream)
{
	m_uiObjectCount++;
	m_pObjects = (SObject*)CMemory::Reallocate(m_pObjects, sizeof(SObject) * m_uiObjectCount);

	m_pObjects[m_uiObjectCount -1].pName = new CString();
	m_pObjects[m_uiObjectCount -1].pVertices  = NULL;
	m_pObjects[m_uiObjectCount -1].pTriangles = NULL;
	m_pObjects[m_uiObjectCount -1].pTexCoords = NULL;
	m_pObjects[m_uiObjectCount -1].pSmoothGroups = NULL;

	char cChar = rStream.GetByte();
	while ((cChar != 0) && (m_pObjects[m_uiObjectCount -1].pName->Length() < 20))
	{
		*(m_pObjects[m_uiObjectCount -1].pName) += cChar;

		cChar = rStream.GetByte();
	}

	return Next(rStream);
}
bool CFile3DS::ChunkProcessTriangularMesh(_INOUT IStreamRead& rStream)
{
	return Next(rStream);
}
bool CFile3DS::ChunkProcessVerticesList(_INOUT IStreamRead& rStream)
{
	m_pObjects[m_uiObjectCount -1].usVertexCount = rStream.GetWord();

	m_pObjects[m_uiObjectCount -1].pVertices = (Float3*)CMemory::Allocate(sizeof(Float3) * m_pObjects[m_uiObjectCount -1].usVertexCount);

	rStream.GetBytes(m_pObjects[m_uiObjectCount -1].pVertices, sizeof(Float3) * m_pObjects[m_uiObjectCount -1].usVertexCount);

	return Next(rStream);
}
bool CFile3DS::ChunkProcessFacesDescription(_INOUT IStreamRead& rStream)
{
	m_pObjects[m_uiObjectCount -1].usTriangleCount = rStream.GetWord();

	m_pObjects[m_uiObjectCount -1].pTriangles = (Short3*)CMemory::Allocate(sizeof(Short3) * m_pObjects[m_uiObjectCount -1].usTriangleCount);

	for (SCF::UINT i = 0; i < m_pObjects[m_uiObjectCount -1].usTriangleCount; i++)
	{
		rStream.GetBytes(&(m_pObjects[m_uiObjectCount -1].pTriangles[i]), sizeof(Short3));

		rStream.SkipBytes(sizeof(WORD));
	}

	return Next(rStream);
}
bool CFile3DS::ChunkProcessFacesMaterial(_INOUT IStreamRead& rStream)
{
	return Next(rStream);
}
bool CFile3DS::ChunkProcessMappingCoordinatesList(_INOUT IStreamRead& rStream)
{
	m_pObjects[m_uiObjectCount -1].usTexCoordCount = rStream.GetWord();

	m_pObjects[m_uiObjectCount -1].pTexCoords = (Float2*)CMemory::Allocate(sizeof(Float2) * m_pObjects[m_uiObjectCount -1].usTexCoordCount);

	rStream.GetBytes(m_pObjects[m_uiObjectCount -1].pTexCoords, sizeof(Float2) * m_pObjects[m_uiObjectCount -1].usTexCoordCount);

	for (SCF::UINT i = 0; i < m_pObjects[m_uiObjectCount -1].usTexCoordCount; i++)
	{
		m_pObjects[m_uiObjectCount -1].pTexCoords[i][1] = 1 - m_pObjects[m_uiObjectCount -1].pTexCoords[i][1];
	}

	return Next(rStream);
}
bool CFile3DS::ChunkProcessSmoothingGroupList(_INOUT IStreamRead& rStream)
{
	m_pObjects[m_uiObjectCount -1].pSmoothGroups = (SCF::UINT*)CMemory::Allocate(sizeof(SCF::UINT) * m_pObjects[m_uiObjectCount -1].usTriangleCount);

	rStream.GetBytes(m_pObjects[m_uiObjectCount -1].pSmoothGroups, sizeof(SCF::UINT) * m_pObjects[m_uiObjectCount -1].usTriangleCount);

	return Next(rStream);
}
bool CFile3DS::ChunkProcessLocalCoordinatesSystem(_INOUT IStreamRead& rStream)
{
	return Next(rStream);
}