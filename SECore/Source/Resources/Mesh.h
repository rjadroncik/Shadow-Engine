#pragma once

#include "../SECore.h"
#include "../System/SEObject.h"

namespace Resources
{
	struct SAttribute
	{
		//Marks this attribute as a source of a given type of data
		//(i. e. eDataType == AS_VERTEX_TANGENT
		//means that vertex tangents are stored in this set)
		SCF::ENUM eDataType;
		void* vpData;

		SCF::UINT uiBufferID;
		SCF::ENUM eBufferUsage;

		SCF::UINT uiComponents;
	};

	struct SVertexList  
	{
		SCF::USHORT* uspTriangles;
		SCF::USHORT usCount;
	};

	class SECORE_API CMesh : public System::CSEObject
	{
		friend class CFile3DS;
	 
	public:
		SCF::ENUM ClassKey() _GET { return ClassMesh; }
		CString   ToString() _GET { return STRING("{Mesh}"); }

	public:
		CMesh();
		virtual ~CMesh();

	public:
		bool Load(_INOUT IStreamRead&  rStream);
		bool Save(_INOUT IStreamWrite& rStream) const;

	public:
		inline bool HasTBNs() _GET { return m_bHasTBNs; }

	public:
		//Mesh data manipulation
		inline const Float3& Vertex(_IN SCF::UINT uiIndex) _GET { return m_pVertices[uiIndex]; }
		inline const Float3* Vertices()                    _GET { return m_pVertices; }
		
		void __fastcall Vertices      (_IN Float3* pVertices, _IN SCF::USHORT usCount) _SET;
		void __fastcall VerticesUpload(_IN Float3* pVertices, _IN SCF::USHORT usCount) _SET;

		inline const Short3& Triangle(_IN SCF::UINT uiIndex) _GET { return m_pTriangles[uiIndex]; }
		inline const Short3* Triangles()                     _GET { return m_pTriangles; }
		
		void __fastcall Triangles      (_IN Short3* pTriangles, _IN SCF::USHORT usCount) _SET;
		void __fastcall TrianglesUpload(_IN Short3* pTriangles, _IN SCF::USHORT usCount) _SET;

		inline SCF::USHORT VertexCount()   _GET { return m_usVertexCount; }
		inline SCF::USHORT TriangleCount() _GET { return m_usTriangleCount; }

	public:
		inline void BufferIndexBind()  const { glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, m_uiBufferIndex); }
		inline void BufferVertexBind() const { glBindBufferARB(GL_ARRAY_BUFFER,         m_uiBufferVertex); }

	public:
		void __fastcall AttributeAdd   (_IN SCF::ENUM eDataType, _IN void* vpData, _IN SCF::UINT uiComponents) _SET;
		void __fastcall AttributeUpload(_IN SCF::ENUM eDataType, _IN void* vpData, _IN SCF::UINT uiComponents) _SET;

		inline const SAttribute* Attribute(_IN SCF::UINT uiIndex) _GET { return &m_pAttributes[uiIndex]; }
		inline SCF::USHORT       AttributesCount()                _GET { return m_usAttributesCount; }

		//A helper function which computes tangents & binormals & sets them as vertex attributes with the specified indexes 
		void __fastcall AttributeTNCompute(); 

	protected:
		void __fastcall BufferIndexEnable    (_IN SCF::ENUM eUsage) _SET;
		void __fastcall BufferVertexEnable   (_IN SCF::ENUM eUsage) _SET;
		void __fastcall BufferAttributeEnable(_IN SCF::ENUM eSource, _IN SCF::ENUM eUsage) _SET;

	protected:
		//Bump mapping internal functions
		void __fastcall ComputeTNsPerVertex(_IN Float3* pTriangleTangents, _IN Float3* pTriangleBinormals, _OUT Float3* pOutVertexTangents, _OUT Float3* pOutVertexBinormals);
		bool __fastcall ComputeTN(_IN Float3& rVertex1, _IN Float3& rVertex2, _IN Float3& rVertex3, _IN Float2& rTexCoord1, _IN Float2& rTexCoord2, _IN Float2& rTexCoord3, _OUT Float3& rOutTangent, _OUT Float3& rOutNormal);

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	protected:
		CString m_FileName;

	protected:
		//Vertex buffer
		SCF::UINT m_uiBufferVertex;
		SCF::ENUM m_eBufferVertexUsage;

		//Index buffer
		SCF::UINT m_uiBufferIndex;
		SCF::ENUM m_eBufferIndexUsage;

	protected:
		Float3*     m_pVertices;
		SCF::USHORT m_usVertexCount;

		Short3*     m_pTriangles;
		SCF::USHORT m_usTriangleCount;

	protected:
		//Vertex attribute sets
		SAttribute* m_pAttributes;
		SCF::USHORT m_usAttributesCount;

		bool m_bHasTBNs;

	protected:
		//VertexList used for seam manipulation
		SVertexList* m_pSeamVertices;
		SVertexList* m_pSmoothingGroups;

		SCF::USHORT m_usSeamVertexListCount;
		SCF::USHORT m_usSmoothingGroupCount;

	protected:
		static void __fastcall AddIndex(_INOUT SVertexList& rVertexList, _IN SCF::USHORT usIndex);
		static bool __fastcall IsIn    (_INOUT SVertexList& rVertexList, _IN SCF::USHORT uisIndex);
	};
};