#include "Mesh.h"

#include "../System/OpenGL.h"
#include "../System/OpenGL20.h"

#include "../Rendering/Material.h"

#include "../Resources/File3DS.h"
#include "../Resources/DataPool.h"

#include <SCFWinAPIUndef.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace Resources;

bool CMesh::Save(_INOUT IStreamWrite& rStream) const
{
	char FOURCC[5] = "mesh";
	rStream.PutBytes(FOURCC, 4); 

	//Vertex data
	rStream.PutWord(m_usVertexCount); 
	rStream.PutBytes(m_pVertices, CMemory::SizeOf(m_pVertices));
	rStream.PutInt(m_eBufferVertexUsage);

	//Per-vertex attributes
	rStream.PutWord(m_usAttributesCount); 
	for (SCF::UINT i = 0 ; i < m_usAttributesCount; i++)
	{
		rStream.PutInt  (m_pAttributes[i].eDataType);
		rStream.PutInt  (m_pAttributes[i].eBufferUsage);
		rStream.PutInt  (m_pAttributes[i].uiComponents);
		rStream.PutBytes(m_pAttributes[i].vpData,  CMemory::SizeOf(m_pAttributes[i].vpData));
	}

	//Triangle data
	rStream.PutWord(m_usTriangleCount); 
	rStream.PutBytes(m_pTriangles, CMemory::SizeOf(m_pTriangles));
	rStream.PutInt(m_eBufferIndexUsage);

	return TRUE;
}

bool CMesh::Load(_INOUT IStreamRead& rStream)
{
	char FOURCC[4];
	rStream.GetBytes(FOURCC, 4); 

	//Vertex data
	m_usVertexCount = rStream.GetWord(); 
	m_pVertices = (Float3*)CMemory::Allocate(sizeof(Float3) * m_usVertexCount);
	
	rStream.GetBytes(m_pVertices, CMemory::SizeOf(m_pVertices));
	BufferVertexEnable(rStream.GetInt());
	
	//Per-vertex attributes
	SCF::USHORT usAttributesCount = rStream.GetWord(); 
	for (SCF::UINT i = 0 ; i < usAttributesCount; i++)
	{
		SAttribute Attrib = { 0, NULL, 0, 0, 0 };

		Attrib.eDataType    = rStream.GetInt();
		Attrib.eBufferUsage = rStream.GetInt();
		Attrib.uiComponents = rStream.GetInt();
		Attrib.vpData       = CMemory::Allocate(Attrib.uiComponents * m_usVertexCount * sizeof(float));
		
		rStream.GetBytes(Attrib.vpData, CMemory::SizeOf(Attrib.vpData));

		AttributeAdd         (Attrib.eDataType, Attrib.vpData, Attrib.uiComponents);
		BufferAttributeEnable(Attrib.eDataType, Attrib.eBufferUsage);
	}

	//Triangle data
	m_usTriangleCount = rStream.GetWord(); 
	m_pTriangles = (Short3*)CMemory::Allocate(sizeof(Short3) * m_usTriangleCount);

	rStream.GetBytes(m_pTriangles, CMemory::SizeOf(m_pTriangles));
	BufferIndexEnable(rStream.GetInt());

	return TRUE;
}

CMesh::CMesh()
{
	//Buffer objects
	m_uiBufferVertex     = 0;
	m_eBufferVertexUsage = GL_STATIC_DRAW; 

	m_uiBufferIndex     = 0;
	m_eBufferIndexUsage = GL_STATIC_DRAW;

	//Vertex data
	m_pVertices     = NULL;
	m_usVertexCount = 0;

	m_pTriangles      = NULL;
	m_usTriangleCount = 0;

	//Additional per-vertex data
	m_pAttributes    = NULL;
	m_usAttributesCount = 0;

	//Mesh metadata
	m_bHasTBNs = FALSE;

	m_pSeamVertices			= NULL;
	m_usSeamVertexListCount	= 0;
	
	m_pSmoothingGroups		= NULL;
	m_usSmoothingGroupCount	= 0;
}

CMesh::~CMesh()
{
	CMemory::Free(m_pVertices);
	CMemory::Free(m_pTriangles);
	
	if (m_pSeamVertices)
	{
		for (SCF::UINT i = 0; i < m_usSeamVertexListCount; i++)
		{
			CMemory::Free(m_pSeamVertices[i].uspTriangles);
		}
		CMemory::Free(m_pSeamVertices);
	}

	if (m_pSmoothingGroups)
	{
		for (SCF::UINT i = 0; i < m_usSmoothingGroupCount; i++)
		{
			CMemory::Free(m_pSmoothingGroups[i].uspTriangles);
		}
		CMemory::Free(m_pSmoothingGroups);
	}

	if (m_uiBufferVertex) { glDeleteBuffersARB(1, &m_uiBufferVertex); }
	if (m_uiBufferIndex)  { glDeleteBuffersARB(1, &m_uiBufferIndex); }

	if (m_pAttributes) 
	{
		for (SCF::UINT i = 0; i < m_usAttributesCount; i++)
		{
			if (m_pAttributes[i].vpData)	
			{
				if (m_pAttributes[i].uiBufferID) { glDeleteBuffersARB(1, &m_pAttributes[i].uiBufferID); }
				CMemory::Free(m_pAttributes[i].vpData); 
			}
		}

		CMemory::Free(m_pAttributes);
	}
}

bool __fastcall CMesh::ComputeTN(_IN Float3& rVertex1, _IN Float3& rVertex2, _IN Float3& rVertex3, _IN Float2& rTexCoord1, _IN Float2& rTexCoord2, _IN Float2& rTexCoord3, _OUT Float3& rOutTangent, _OUT Float3& rOutNormal)
{
	//Normal creation is easy, saves time
	MakeNormal(rOutNormal, rVertex1, rVertex2, rVertex3);

	typedef struct
	{
		Float3 Position;
		Float2 TexCoord;

	} Vertex;

	Vertex   aVertices[3];
	Vertex   TempVertex;

	float    fDotProduct;
	float    fInterpolator;
	Float3 TempVector;

	//Fill in the rOutTangent space vertex array
	aVertices[0].Position[0] = rVertex1[0];
	aVertices[0].Position[1] = rVertex1[1];
	aVertices[0].Position[2] = rVertex1[2];
	aVertices[0].TexCoord[0] = rTexCoord1[0];
	aVertices[0].TexCoord[1] = rTexCoord1[1];

	aVertices[1].Position[0] = rVertex2[0];
	aVertices[1].Position[1] = rVertex2[1];
	aVertices[1].Position[2] = rVertex2[2];
	aVertices[1].TexCoord[0] = rTexCoord2[0];
	aVertices[1].TexCoord[1] = rTexCoord2[1];

	aVertices[2].Position[0] = rVertex3[0];
	aVertices[2].Position[1] = rVertex3[1];
	aVertices[2].Position[2] = rVertex3[2];
	aVertices[2].TexCoord[0] = rTexCoord3[0];
	aVertices[2].TexCoord[1] = rTexCoord3[1];

	//Compute the rOutTangent vector
	{
		//Sort the vertices based on their t texture coordinate
		if (aVertices[0].TexCoord[1] < aVertices[1].TexCoord[1]) { TempVertex = aVertices[0]; aVertices[0] = aVertices[1]; aVertices[1] = TempVertex; }
		if (aVertices[0].TexCoord[1] < aVertices[2].TexCoord[1]) { TempVertex = aVertices[0]; aVertices[0] = aVertices[2]; aVertices[2] = TempVertex; }
		if (aVertices[1].TexCoord[1] < aVertices[2].TexCoord[1]) { TempVertex = aVertices[1]; aVertices[1] = aVertices[2]; aVertices[2] = TempVertex; }

		//Compute the parametric offset along edge02 to the middle t coordinate
		fInterpolator = (aVertices[1].TexCoord[1] - aVertices[0].TexCoord[1]) / (aVertices[2].TexCoord[1] - aVertices[0].TexCoord[1]);

		//Use the interpolation parameter to compute the vertex position along edge02 that corresponds to the same t coordinate as v[1]
		LerpVector3(TempVector, aVertices[0].Position, aVertices[2].Position, fInterpolator);

		//Compute the interpolated s coord value
		fInterpolator = aVertices[0].TexCoord[0] + (aVertices[2].TexCoord[0] - aVertices[0].TexCoord[0]) * fInterpolator;

		//The tangent vector is the ray from the middle vertex to the lerped vertex
		SubtractVectors3(rOutTangent, TempVector, aVertices[1].Position);

		//Make sure the tangent points in the right direction
		if (fInterpolator < aVertices[1].TexCoord[0]) { InvertVector3(rOutTangent, rOutTangent); }

		//Make sure the tangent vector is perpendicular to the rNormal
		fDotProduct = MultiplyVectors3(rOutNormal, rOutTangent);            
		rOutTangent[0] = rOutTangent[0] - rOutNormal[0] * fDotProduct;
		rOutTangent[1] = rOutTangent[1] - rOutNormal[1] * fDotProduct;
		rOutTangent[2] = rOutTangent[2] - rOutNormal[2] * fDotProduct;

		//Normalize the tangent vector
		NormalizeVector3(rOutTangent, rOutTangent);
	}

	return TRUE;
}

void __fastcall CMesh::AttributeTNCompute() 
{
	if (m_bHasTBNs) { return; }

	Float3* pVertexTangents = (Float3*)CMemory::Allocate(sizeof(Float3) * m_usVertexCount);
	Float3* pVertexNormals  = (Float3*)CMemory::Allocate(sizeof(Float3) * m_usVertexCount);
	
	Float3* pTriangleTangents = (Float3*)CMemory::Allocate(sizeof(Float3) * m_usTriangleCount);
	Float3* pTriangleNormals  = (Float3*)CMemory::Allocate(sizeof(Float3) * m_usTriangleCount);

	//Go through all triangles
	for (SCF::UINT i = 0; i < m_usTriangleCount; i++)
	{
		ComputeTN(
			*(Float3*)(&m_pVertices[m_pTriangles[i][0]]),
			*(Float3*)(&m_pVertices[m_pTriangles[i][1]]),
			*(Float3*)(&m_pVertices[m_pTriangles[i][2]]),
			((Float2*)m_pAttributes[0].vpData)[m_pTriangles[i][0]],
			((Float2*)m_pAttributes[0].vpData)[m_pTriangles[i][1]],
			((Float2*)m_pAttributes[0].vpData)[m_pTriangles[i][2]],
			pTriangleTangents[i],
			pTriangleNormals[i]);
	}

	//Compute tangent & binormal vectors on a per-vertex basis
	ComputeTNsPerVertex(pTriangleTangents, pTriangleNormals, pVertexTangents, pVertexNormals);

	AttributeAdd(AS_VERTEX_TANGENT, pVertexTangents, 3);
	AttributeAdd(AS_VERTEX_NORNAL,  pVertexNormals,  3);

	BufferAttributeEnable(AS_VERTEX_TANGENT, GL_STATIC_DRAW);
	BufferAttributeEnable(AS_VERTEX_NORNAL,  GL_STATIC_DRAW);

	CMemory::Free(pTriangleTangents);
	CMemory::Free(pTriangleNormals);

	m_bHasTBNs = TRUE;
}

void __fastcall CMesh::ComputeTNsPerVertex(_IN Float3* pTriangleTangents, _IN Float3* pTriangleNormals, _OUT Float3* pOutVertexTangents, _OUT Float3* pOutVertexNormals)
{
	//Zero all tangent & binormal vectors 
	ZeroMemory(pOutVertexTangents, sizeof(Float3) * m_usVertexCount);
	ZeroMemory(pOutVertexNormals,  sizeof(Float3) * m_usVertexCount);

	//Go through all triangles
	for (SCF::UINT i = 0; i < m_usTriangleCount; i++)
	{
		for (SCF::UINT j = 0; j < 3; j++)
		{
			//Sum up the tangent & binormal vectors for each vertex, to smooth out the texture space of the mesh
			AddVectors3(pOutVertexTangents[m_pTriangles[i][j]], pOutVertexTangents[m_pTriangles[i][j]], pTriangleTangents[i]);
			AddVectors3(pOutVertexNormals [m_pTriangles[i][j]], pOutVertexNormals [m_pTriangles[i][j]], pTriangleNormals[i]);
		}
	}

	//Go through all vertices..
	for (SCF::UINT i = 0; i < m_usVertexCount; i++)
	{
		//..normalize tangent & binormal of each vertex
		NormalizeVector3(pOutVertexTangents[i], pOutVertexTangents[i]);
		NormalizeVector3(pOutVertexNormals[i],  pOutVertexNormals[i]);
	}

	Float3 TempTangent;
	Float3 TempBinormal;

	for (SCF::UINT k = 0; k < m_usSmoothingGroupCount; k++)
	{
		for (SCF::UINT i = 0; i < m_usSeamVertexListCount; i++)
		{
			ZeroVector3(TempTangent);
			ZeroVector3(TempBinormal);

			for (USHORT j = 0; j < m_pSeamVertices[i].usCount; j++)
			{
				if (IsIn(m_pSmoothingGroups[k], m_pSeamVertices[i].uspTriangles[j]))
				{
					AddVectors3(TempTangent,  TempTangent,  pOutVertexTangents[m_pSeamVertices[i].uspTriangles[j]]);
					AddVectors3(TempBinormal, TempBinormal, pOutVertexNormals [m_pSeamVertices[i].uspTriangles[j]]);
				}
			}

			NormalizeVector3(TempTangent,  TempTangent);
			NormalizeVector3(TempBinormal, TempBinormal);
		
			for (USHORT j = 0; j < m_pSeamVertices[i].usCount; j++)
			{
				if (IsIn(m_pSmoothingGroups[k], m_pSeamVertices[i].uspTriangles[j]))
				{
					CopyVector3(pOutVertexTangents[m_pSeamVertices[i].uspTriangles[j]], TempTangent);
					CopyVector3(pOutVertexNormals [m_pSeamVertices[i].uspTriangles[j]], TempBinormal);
				}
			}
		}
	}
}

void __fastcall CMesh::Vertices(_IN Float3* pVertices, _IN USHORT usCount) 
{
	m_pVertices     = (Float3*)pVertices;
	m_usVertexCount = usCount; 
}

void __fastcall CMesh::VerticesUpload(_IN Float3* pVertex, _IN USHORT usCount) 
{ 
	m_pVertices = (Float3*)CMemory::Reallocate(m_pVertices, sizeof(Float3) * usCount);
	memcpy(m_pVertices, pVertex, sizeof(Float3) * usCount);

	Vertices(m_pVertices, usCount);
}

void __fastcall CMesh::Triangles(_IN Short3* pTriangles, _IN USHORT usCount) 
{ 
	m_pTriangles      = (Short3*)pTriangles; 
	m_usTriangleCount = usCount;
} 

void __fastcall CMesh::TrianglesUpload(_IN Short3* pTriangles, _IN USHORT usCount)
{ 
	m_pTriangles = (Short3*)CMemory::Reallocate(m_pTriangles, sizeof(Short3) * usCount);
	memcpy(m_pTriangles, pTriangles, sizeof(Short3) * usCount);

	Triangles(m_pTriangles, usCount);
}

void __fastcall CMesh::AttributeAdd(_IN SCF::ENUM eDataType, _IN void* vpData, _IN SCF::UINT uiComponents)
{
	m_usAttributesCount++;
	m_pAttributes = (SAttribute*)CMemory::Reallocate(m_pAttributes, sizeof(SAttribute) * m_usAttributesCount);

	m_pAttributes[m_usAttributesCount - 1].eDataType    = eDataType;
	m_pAttributes[m_usAttributesCount - 1].vpData       = (void*)vpData;
	m_pAttributes[m_usAttributesCount - 1].eBufferUsage = GL_STATIC_DRAW;
	m_pAttributes[m_usAttributesCount - 1].uiComponents = uiComponents;
	m_pAttributes[m_usAttributesCount - 1].uiBufferID   = 0;
}

void __fastcall CMesh::AttributeUpload(_IN SCF::ENUM eDataType, _IN void* vpData, _IN SCF::UINT uiComponents)
{ 
	void* tmpPtr = CMemory::Allocate(sizeof(float)* uiComponents * m_usVertexCount);

	memcpy(tmpPtr, vpData, sizeof(float)* uiComponents * m_usVertexCount);

	AttributeAdd(eDataType, tmpPtr, uiComponents);
}

void __fastcall CMesh::BufferIndexEnable(_IN SCF::ENUM eUsage) _SET
{
	glGenBuffersARB(1, &m_uiBufferIndex);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, m_uiBufferIndex);

	m_eBufferIndexUsage = eUsage;
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, sizeof(Short3) * m_usTriangleCount, m_pTriangles, m_eBufferIndexUsage); 
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void __fastcall CMesh::BufferVertexEnable(_IN SCF::ENUM eUsage)	_SET
{
	glGenBuffersARB(1, &m_uiBufferVertex);
	glBindBufferARB(GL_ARRAY_BUFFER, m_uiBufferVertex);

	m_eBufferVertexUsage = eUsage;
	glBufferDataARB(GL_ARRAY_BUFFER, sizeof(Float3) * m_usVertexCount, m_pVertices, m_eBufferVertexUsage);		
	glBindBufferARB(GL_ARRAY_BUFFER, 0);
}

void __fastcall CMesh::BufferAttributeEnable(_IN SCF::ENUM eSource, _IN SCF::ENUM eUsage) _SET
{
	for (SCF::UINT i = 0; i < m_usAttributesCount; i++)
	{
		if (m_pAttributes[i].eDataType == eSource)
		{
			register SAttribute& rSet = m_pAttributes[i];

			glGenBuffersARB(1, &rSet.uiBufferID);
			glBindBufferARB(GL_ARRAY_BUFFER, rSet.uiBufferID);

			rSet.eBufferUsage = eUsage;
			glBufferDataARB(GL_ARRAY_BUFFER, sizeof(float) * rSet.uiComponents * m_usVertexCount, rSet.vpData, rSet.eBufferUsage); 
			glBindBufferARB(GL_ARRAY_BUFFER, 0); 
			return;
		}
	}
}

bool __fastcall CMesh::IsIn(_INOUT SVertexList& rVertexList, _IN USHORT usIndex)
{
	for (SCF::UINT i = 0; i < rVertexList.usCount; i++)
	{
		if (rVertexList.uspTriangles[i] == usIndex) { return TRUE; }
	}
	return FALSE;
}

void __fastcall CMesh::AddIndex(_INOUT SVertexList& rVertexList, _IN USHORT usIndex)
{
	if (IsIn(rVertexList, usIndex)) { return; }

	rVertexList.usCount++;			
	rVertexList.uspTriangles = (SCF::USHORT*)CMemory::Reallocate(rVertexList.uspTriangles, sizeof(USHORT) * rVertexList.usCount);

	rVertexList.uspTriangles[rVertexList.usCount - 1] = usIndex;
}

void CMesh::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
	rWriter.PutValue(STRING("file"), *(new CString(m_FileName)));

	if (CFile(m_FileName).Extension().IsEqualCI(STRING("mesh"))) 
	{
		CStreamFileWrite StreamWrite(m_FileName);
		Save(StreamWrite);
	}
}

void CMesh::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	m_FileName = *rReader.GetValue();

	//Query file from data pool
	IStreamRead* pStreamRead = Resources::CDataPool::FileOpen(m_FileName);
	if (pStreamRead)
	{
		//Extract the extension to the file
		CString FileExtension;
		CDataPool::ParsePathFile(m_FileName, NULL, NULL, &FileExtension);

		if (FileExtension.IsEqualCI(STRING("3ds"))) 
		{
			Resources::CFile3DS File3DS(*pStreamRead);
			File3DS.ConvertTo(*this);
		}
		else if (FileExtension.IsEqualCI(STRING("mesh"))) 
		{
			Load(*pStreamRead);
		}

		Resources::CDataPool::FileClose(pStreamRead);
	}
}