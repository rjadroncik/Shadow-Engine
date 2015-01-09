#include "ProxyGPUProgram.h"

using namespace Scripting;

using namespace SCFXML;
using namespace SCFScripting;
//using namespace SCFScriptingXML;

CProxyGPUProgram::CProxyGPUProgram()
{
	m_pObject = new Resources::CGPUProgram();
	ADDREF(*(m_pObject));
}

CProxyGPUProgram::~CProxyGPUProgram()
{
	RELEASE(*(m_pObject));
	delete m_pObject;
}

void CProxyGPUProgram::Register()
{
	CXMLObjectSerializable::ConstructorRegister(ClassProxyGPUProgram, CProxyGPUProgram::New);

	CEnumeration* pEnumeration = new CEnumeration();
	{
		pEnumeration->ValueDefine(STRING("vertex.texCoord[0]"), AS_VERTEX_TEXCOORD0);
		pEnumeration->ValueDefine(STRING("vertex.texCoord[1]"), AS_VERTEX_TEXCOORD1);
		pEnumeration->ValueDefine(STRING("vertex.texCoord[2]"), AS_VERTEX_TEXCOORD2);
		pEnumeration->ValueDefine(STRING("vertex.texCoord[3]"), AS_VERTEX_TEXCOORD3);

		pEnumeration->ValueDefine(STRING("vertex.tangent"), AS_VERTEX_TANGENT);
		pEnumeration->ValueDefine(STRING("vertex.normal"),  AS_VERTEX_NORNAL);
	}
	CRegistry::EnumerationRegister(STRING("SourcesAttributes"), *pEnumeration);
	
	pEnumeration = new CEnumeration();
	{
		pEnumeration->ValueDefine(STRING("light.position"),               US_LIGHT_POSITION);
		pEnumeration->ValueDefine(STRING("light.position.local"),         US_LIGHT_POSITION_LOCAL);
		pEnumeration->ValueDefine(STRING("light.parameters"),             US_LIGHT_PARAMETERS);
		pEnumeration->ValueDefine(STRING("light.tint"),                   US_LIGHT_TINT);
		pEnumeration->ValueDefine(STRING("light.ambientDiffuseSpecular"), US_LIGHT_AMB_DIF_SPE);

		pEnumeration->ValueDefine(STRING("light.matrix.depthMapTransform"),  US_LIGHT_MATRIX_DEPTHMAP_TRANSFORM0);
		pEnumeration->ValueDefine(STRING("light.matrix.depthMapTransform0"), US_LIGHT_MATRIX_DEPTHMAP_TRANSFORM0);
		pEnumeration->ValueDefine(STRING("light.matrix.depthMapTransform1"), US_LIGHT_MATRIX_DEPTHMAP_TRANSFORM1);

		pEnumeration->ValueDefine(STRING("camera.position"),       US_CAMERA_POSITION);
		pEnumeration->ValueDefine(STRING("camera.position.local"), US_CAMERA_POSITION_LOCAL);
		pEnumeration->ValueDefine(STRING("camera.viewport"),       US_CAMERA_VIEWPORT);

		pEnumeration->ValueDefine(STRING("material.tint"),                           US_MATERIAL_TINT);
		pEnumeration->ValueDefine(STRING("material.ambientDiffuseSpecularEmission"), US_MATERIAL_AMB_DIF_SPE_EMI);
		pEnumeration->ValueDefine(STRING("material.shininess"),                      US_MATERIAL_SHININESS);
	}
	CRegistry::EnumerationRegister(STRING("SourcesUniforms"), *pEnumeration);
	
	pEnumeration = new CEnumeration();
	{
		pEnumeration->ValueDefine(STRING("material.diffuseColor"), SS_MATERIAL_DIFFUSE_COLOR);
		pEnumeration->ValueDefine(STRING("material.normalMap"),    SS_MATERIAL_NORMALMAP);
	
		pEnumeration->ValueDefine(STRING("light.depthMap"),  SS_LIGHT_DEPTHMAP0);
		pEnumeration->ValueDefine(STRING("light.depthMap0"), SS_LIGHT_DEPTHMAP0);
		pEnumeration->ValueDefine(STRING("light.depthMap1"), SS_LIGHT_DEPTHMAP1);
	}
	CRegistry::EnumerationRegister(STRING("SourcesSamplers"), *pEnumeration);

	CClass* pClass = new CClass(CRegistry::Class(ClassProxy));
	{
	}
	CRegistry::ClassRegister(STRING("GPUProgram"), ClassProxyGPUProgram, *pClass);
}

void CProxyGPUProgram::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
	rWriter.BlockStart(STRING("shaderVertex"));
	PUTVALUE("file", *(new CString(m_pObject->ShaderVertex())));
	rWriter.BlockEnd();

	rWriter.BlockStart(STRING("shaderFragment"));
	PUTVALUE("file", *(new CString(m_pObject->ShaderFragment())));
	rWriter.BlockEnd();

	rWriter.BlockStart(STRING("attributes"));
	PUTVALUE("count", *(new CInt(m_pObject->AttributesCount())));

	for (SCF::UINT i = 0; i < m_pObject->AttributesCount(); i++)
	{
		rWriter.BlockStart(STRING("attribute"));
		
		PUTVALUE("name",   *(new CString(*(m_pObject->Attribute(i)->pName))));
		PUTVALUE("source", *(new CString(*(CRegistry::EnumerationTranslate(STRING("SourcesAttributes"), m_pObject->Attribute(i)->eSource)))));

		rWriter.BlockEnd();
	}
	rWriter.BlockEnd();

	rWriter.BlockStart(STRING("uniforms"));
	PUTVALUE("count", *(new CInt(m_pObject->UniformsCount())));

	for (SCF::UINT i = 0; i < m_pObject->UniformsCount(); i++)
	{
		rWriter.BlockStart(STRING("uniform"));

		PUTVALUE("name",   *(new CString(*(m_pObject->Uniform(i)->pName))));
		PUTVALUE("source", *(new CString(*(CRegistry::EnumerationTranslate(STRING("SourcesUniforms"), m_pObject->Uniform(i)->eSource)))));

		rWriter.BlockEnd();
	}
	rWriter.BlockEnd();

	rWriter.BlockStart(STRING("samplers"));
	PUTVALUE("count", *(new CInt(m_pObject->SamplersCount())));

	for (SCF::UINT i = 0; i < m_pObject->SamplersCount(); i++)
	{
		rWriter.BlockStart(STRING("sampler"));

		PUTVALUE("name",   *(new CString(*(m_pObject->Sampler(i)->pName))));
		PUTVALUE("source", *(new CString(*(CRegistry::EnumerationTranslate(STRING("SourcesSamplers"), m_pObject->Sampler(i)->eSource)))));

		rWriter.BlockEnd();
	}
	rWriter.BlockEnd();
}

void CProxyGPUProgram::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CString* pValue = NULL;

	rReader.GetBlock();
	GETVALUE { m_pObject->ShaderVertex(*pValue); }
	rReader.GetBlock();
	GETVALUE { m_pObject->ShaderFragment(*pValue); }
	
	m_pObject->CompileAndLink();

	//Attributes
	rReader.GetBlock();
	{
		SCF::UINT uiCount = 0;
		GETVALUE { uiCount = CInt(*pValue).Value(); }

		for (SCF::UINT i = 0; i < uiCount; i++)
		{
			rReader.GetBlock();
			CString* pName   = rReader.GetValue();
			CString* pSource = rReader.GetValue();

			if (pName && pSource)
			{ 
				CEnum* pEnum = CRegistry::EnumerationTranslate(STRING("SourcesAttributes"), *pSource);
				if (pEnum) { m_pObject->AttributeAdd(*pName, pEnum->Value()); }
			}
		}
	}

	//Uniforms
	rReader.GetBlock();
	{
		SCF::UINT uiCount = 0;
		GETVALUE { uiCount = CInt(*pValue).Value(); }

		for (SCF::UINT i = 0; i < uiCount; i++)
		{
			rReader.GetBlock();
			CString* pName   = rReader.GetValue();
			CString* pSource = rReader.GetValue();

			if (pName && pSource)
			{ 
				CEnum* pEnum = CRegistry::EnumerationTranslate(STRING("SourcesUniforms"), *pSource);
				if (pEnum) { m_pObject->UniformAdd(*pName, pEnum->Value()); }
			}
		}
	}

	//Samplers
	rReader.GetBlock();
	{
		SCF::UINT uiCount = 0;
		GETVALUE { uiCount = CInt(*pValue).Value(); }

		for (SCF::UINT i = 0; i < uiCount; i++)
		{
			rReader.GetBlock();
			CString* pName   = rReader.GetValue();
			CString* pSource = rReader.GetValue();

			if (pName && pSource)
			{ 
				CEnum* pEnum = CRegistry::EnumerationTranslate(STRING("SourcesSamplers"), *pSource);
				if (pEnum) { m_pObject->SamplerAdd(*pName, pEnum->Value()); }
			}
		}
	}
}