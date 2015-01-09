#pragma once

#include "Proxy.h"

#include "../Resources/Mesh.h"

namespace Scripting
{
	class SECORE_API CProxyMesh : public CProxy
	{
	public:
		static void Register();
		static CXMLObjectSerializable* New() { return new CProxyMesh(); }

	public:
		SCF::ENUM ClassKey() _GET { return ClassProxyMesh; }
		CString   AsString() _GET { return STRING("{ProxyMesh}"); }

	public:
		CProxyMesh();
		virtual ~CProxyMesh();

	public:
		Resources::CMesh& Value() { return *m_pObject; }

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	public:
		void FileName(_IN CString& rFileName) _SET { m_FileName = rFileName; }

	public:
		void AttributeAdd   (_IN CEnum& rDataType, _IN CPointer& rData, _IN CInt& rComponents) _SET;
		void AttributeUpload(_IN CEnum& rDataType, _IN CPointer& rData, _IN CInt& rComponents) _SET;

	public:
		void Vertices      (_IN CPointer& rVertices, _IN CInt& rCount) _SET;
		void VerticesUpload(_IN CPointer& rVertices, _IN CInt& rCount) _SET;

		void Triangles      (_IN CPointer& rTriangles, _IN CInt& rCount) _SET;
		void TrianglesUpload(_IN CPointer& rTriangles, _IN CInt& rCount) _SET;

	protected:
		Resources::CMesh* m_pObject;
		CString m_FileName;
	};
};
