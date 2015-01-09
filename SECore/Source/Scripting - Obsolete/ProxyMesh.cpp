#include "ProxyMesh.h"

#include "../Resources/File3DS.h"
#include "../Resources/DataPool.h"

using namespace Scripting;
using namespace Resources;

using namespace SCFXML;
using namespace SCFScripting;
//using namespace SCFScriptingXML;

CProxyMesh::CProxyMesh()
{
	m_pObject = new Resources::CMesh();
	ADDREF(*(m_pObject));
}

CProxyMesh::~CProxyMesh()
{
	RELEASE(*(m_pObject));
	delete m_pObject;
}

void CProxyMesh::Register()
{
	CXMLObjectSerializable::ConstructorRegister(ClassProxyMesh, CProxyMesh::New);

	CClass* pClass = new CClass(CRegistry::Class(ClassProxy));
	{
	}
	CRegistry::ClassRegister(STRING("Mesh"), ClassProxyMesh, *pClass);
}

void CProxyMesh::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
	rWriter.PutValue(STRING("file"), *(new CString(m_FileName)));

	if (CFile(m_FileName).Extension().IsEqualCI(STRING("mesh"))) 
	{
		CStreamFileWrite StreamWrite(m_FileName);
		m_pObject->Save(StreamWrite);
	}
}

void CProxyMesh::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
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
			File3DS.ConvertTo(*m_pObject);
		}
		else if (FileExtension.IsEqualCI(STRING("mesh"))) 
		{
			m_pObject->Load(*pStreamRead);
		}

		Resources::CDataPool::FileClose(pStreamRead);
	}
}

void CProxyMesh::AttributeAdd   (_IN CEnum& rDataType, _IN CPointer& rData, _IN CInt& rComponents) _SET { m_pObject->AttributeAdd   (rDataType.Value(), rData.Value(), rComponents.Value()); }
void CProxyMesh::AttributeUpload(_IN CEnum& rDataType, _IN CPointer& rData, _IN CInt& rComponents) _SET { m_pObject->AttributeUpload(rDataType.Value(), rData.Value(), rComponents.Value()); }

void CProxyMesh::Vertices      (_IN CPointer& rVertices, _IN CInt& rCount) _SET { m_pObject->Vertices      ((SCFMathematics::Float3*)rVertices.Value(), (SCF::USHORT)rCount.Value()); }
void CProxyMesh::VerticesUpload(_IN CPointer& rVertices, _IN CInt& rCount) _SET { m_pObject->VerticesUpload((SCFMathematics::Float3*)rVertices.Value(), (SCF::USHORT)rCount.Value()); }

void CProxyMesh::Triangles      (_IN CPointer& rTriangles, _IN CInt& rCount) _SET { m_pObject->Triangles      ((SCFMathematics::Short3*)rTriangles.Value(), (SCF::USHORT)rCount.Value()); }
void CProxyMesh::TrianglesUpload(_IN CPointer& rTriangles, _IN CInt& rCount) _SET { m_pObject->TrianglesUpload((SCFMathematics::Short3*)rTriangles.Value(), (SCF::USHORT)rCount.Value()); }