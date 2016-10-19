#include "GPUProgram.h"

#include "DataPool.h"

#include "../Simulation/ParticleSystem.h"
#include "Mesh.h"

using namespace Resources;
using namespace System;
using namespace Simulation;

namespace Resources
{
	char GPUProgram_szBuffer[4096];
};

CGPUProgram::CGPUProgram()
{
	m_hGLShaderVertex   = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	m_hGLShaderFragment = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

	m_hGLProgram = glCreateProgramObjectARB();

	glAttachObjectARB(m_hGLProgram, m_hGLShaderVertex);
	glAttachObjectARB(m_hGLProgram, m_hGLShaderFragment);

	m_pSamplers = NULL;
	m_uiSamplersCount = 0;

	m_pUniforms = NULL;
	m_uiUniformsCount = 0;

	m_pAttributes = NULL;
	m_uiAttributesCount = 0;
}

CGPUProgram::~CGPUProgram()
{
	glUseProgramObjectARB(0); 

	glDetachObjectARB(m_hGLProgram, m_hGLShaderVertex);
	glDetachObjectARB(m_hGLProgram, m_hGLShaderFragment);

	glDeleteObjectARB(m_hGLProgram);
	glDeleteObjectARB(m_hGLShaderVertex);
	glDeleteObjectARB(m_hGLShaderFragment);

	for (UINT i = 0; i < m_uiSamplersCount;   i++) { delete m_pSamplers[i].pName; }
	for (UINT i = 0; i < m_uiUniformsCount;   i++) { delete m_pUniforms[i].pName; }
	for (UINT i = 0; i < m_uiAttributesCount; i++) { delete m_pAttributes[i].pName; }

	CMemory::Free(m_pSamplers);
	CMemory::Free(m_pUniforms);
	CMemory::Free(m_pAttributes);
}

bool __fastcall CGPUProgram::ShaderVertex(_IN CString& rFilePath)
{
	//Query file from data pool
	IStreamRead* pReadStream = CDataPool::FileOpen(rFilePath);
	if (pReadStream)
	{
		CEventLog::EventNew(STRING("Loading vertex shader:") + rFilePath);
		m_ShaderFileVertex = rFilePath;

		CMemoryBlock MemoryBlock((UINT)pReadStream->BytesLeft());
		pReadStream->GetBytes((BYTE*)MemoryBlock.Value(), MemoryBlock.Size());

		//Load shader source..
		const GLcharARB* Strings[] = { (GLcharARB*)MemoryBlock.Value() };
		GLint            Lengths[] = { MemoryBlock.Size() };

		glShaderSourceARB(m_hGLShaderVertex, 1, Strings, Lengths);

		CDataPool::FileClose(pReadStream);
		return TRUE;
	}

	return FALSE;
}

bool __fastcall CGPUProgram::ShaderFragment(_IN CString& rFilePath) 
{
	//Query file from data pool
	IStreamRead* pReadStream = CDataPool::FileOpen(rFilePath);
	if (pReadStream)
	{
		CEventLog::EventNew(STRING("Loading fragment shader:") + rFilePath);
		m_ShaderFileFragment = rFilePath;

		CMemoryBlock MemoryBlock((UINT)pReadStream->BytesLeft());
		pReadStream->GetBytes((BYTE*)MemoryBlock.Value(), MemoryBlock.Size());

		//Load shader source..
		const GLcharARB* Strings[] = { (GLcharARB*)MemoryBlock.Value() };
		GLint            Lengths[] = { MemoryBlock.Size() };

		glShaderSourceARB(m_hGLShaderFragment, 1, Strings, Lengths);
		
		CDataPool::FileClose(pReadStream);
		return TRUE;
	}

	return FALSE;
}

int __fastcall CGPUProgram::AttributeTarget(_IN CString& rName)
{
	rName.ToASCII(GPUProgram_szBuffer);
	return glGetAttribLocationARB(m_hGLProgram, GPUProgram_szBuffer);
}

int __fastcall CGPUProgram::UniformTarget(_IN CString& rName)
{
	rName.ToASCII(GPUProgram_szBuffer);
	return glGetUniformLocationARB(m_hGLProgram, GPUProgram_szBuffer);
}

bool __fastcall CGPUProgram::Relink()
{
	int iStatus = 0;

	CEventLog::EventNew(STRING("Relinking program."));
	glLinkProgramARB(m_hGLProgram);
	glGetObjectParameterivARB(m_hGLProgram, GL_OBJECT_LINK_STATUS_ARB, &iStatus);

	if (!iStatus) 
	{
		CEventLog::ErrorNew(STRING("Relinking failed."));
		InfoLogsPrint();
		return FALSE; 
	}

	return TRUE;
}

bool __fastcall CGPUProgram::CompileAndLink()
{
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_ARB);

	///////////////////////////// Compile stage ////////////////////////////////
	CEventLog::BlockNew(STRING("Building program"));

	int iStatus = 0;

	CEventLog::EventNew(STRING("Compiling vertex shader."));
	glCompileShaderARB(m_hGLShaderVertex);
	glGetObjectParameterivARB(m_hGLShaderVertex, GL_OBJECT_COMPILE_STATUS_ARB, &iStatus);

	if (!iStatus) 
	{
		CEventLog::ErrorNew(STRING("Compilation failed."));
		InfoLogsPrint();
		return FALSE; 
	}

	CEventLog::EventNew(STRING("Compiling fragment shader."));
	glCompileShaderARB(m_hGLShaderFragment);
	glGetObjectParameterivARB(m_hGLShaderFragment, GL_OBJECT_COMPILE_STATUS_ARB, &iStatus);

	if (!iStatus) 
	{
		CEventLog::ErrorNew(STRING("Compilation failed."));
		InfoLogsPrint();
		return FALSE; 
	}

	////////////////////////////// Link stage //////////////////////////////////

	CEventLog::EventNew(STRING("Linking program."));
	glLinkProgramARB(m_hGLProgram);
	glGetObjectParameterivARB(m_hGLProgram, GL_OBJECT_LINK_STATUS_ARB, &iStatus);

	if (!iStatus) 
	{
		CEventLog::ErrorNew(STRING("Linking failed."));
		InfoLogsPrint();
		return FALSE; 
	}

	CEventLog::EventNew(STRING("Program ready to use."));

	InfoLogsPrint();

	GLint iActiveAttributes = 0;
	glGetObjectParameterivARB(m_hGLProgram, GL_OBJECT_ACTIVE_ATTRIBUTES_ARB, &iActiveAttributes);

	GLint iActiveUniforms = 0;
	glGetObjectParameterivARB(m_hGLProgram, GL_OBJECT_ACTIVE_UNIFORMS_ARB, &iActiveUniforms);

	CEventLog::EventNew(STRING("Programs has ") + CInt(iActiveAttributes).ToString() + STRING(" active attributes, ") + CInt(iActiveUniforms).ToString() + STRING(" active uniforms."));
	
	for (int i = 0; i < iActiveAttributes; i++)
	{
		ENUM eAttribType = 0;
		int  iAttribSize = 0;
		glGetActiveAttribARB(m_hGLProgram, i, 4096, NULL, &iAttribSize, &eAttribType, GPUProgram_szBuffer);
	}
	
	CEventLog::BlockClose();
	return TRUE;
}

void __fastcall CGPUProgram::InfoLogsPrint()
{
	CString Line;

	CEventLog::BlockNew(STRING("Vertex shader info log"));
	{
		glGetInfoLogARB(m_hGLShaderVertex, 4096, NULL, GPUProgram_szBuffer);

		CString Report(GPUProgram_szBuffer);
		if (Report.Length())
		{
			CStreamStringRead ReportStream(Report);
			CStreamReadTextUCS2 ReportStreamText(ReportStream);

			while (ReportStreamText.GetLine(Line)) { if (Line.Length() > 1) { CEventLog::EventNew(Line); } }
		}
	}
	CEventLog::BlockClose();

	CEventLog::BlockNew(STRING("Fragment shader info log"));
	{
		glGetInfoLogARB(m_hGLShaderFragment, 4096, NULL, GPUProgram_szBuffer);

		CString Report(GPUProgram_szBuffer);
		if (Report.Length())
		{
			CStreamStringRead ReportStream(Report);
			CStreamReadTextUCS2 ReportStreamText(ReportStream);

			while (ReportStreamText.GetLine(Line)) { if (Line.Length() > 1) { CEventLog::EventNew(Line); } }
		}
	}
	CEventLog::BlockClose();

	CEventLog::BlockNew(STRING("Program info log"));
	{
		glGetInfoLogARB(m_hGLProgram, 4096, NULL, GPUProgram_szBuffer);

		CString Report(GPUProgram_szBuffer);
		if (Report.Length())
		{
			CStreamStringRead ReportStream(Report);
			CStreamReadTextUCS2 ReportStreamText(ReportStream);

			while (ReportStreamText.GetLine(Line)) { if (Line.Length() > 1) { CEventLog::EventNew(Line); } }
		}
	}
	CEventLog::BlockClose();
}

void __fastcall CGPUProgram::SamplerAdd(_IN CString& rName, _IN ENUM eSource) _SET
{
	int iTarget = UniformTarget(rName);

	if (iTarget != -1)
	{
		m_uiSamplersCount++;
		m_pSamplers = (SBinding*)CMemory::Reallocate(m_pSamplers, sizeof(SBinding) * m_uiSamplersCount);

		m_pSamplers[m_uiSamplersCount - 1].pName = new CString(rName);
		m_pSamplers[m_uiSamplersCount - 1].eSource = eSource;
		m_pSamplers[m_uiSamplersCount - 1].uiTarget = iTarget;
	}
}

void __fastcall CGPUProgram::UniformAdd(_IN CString& rName, _IN ENUM eSource) _SET
{ 
	int iTarget = UniformTarget(rName);

	if (iTarget != -1)
	{
		m_uiUniformsCount++;
		m_pUniforms = (SBinding*)CMemory::Reallocate(m_pUniforms, sizeof(SBinding) * m_uiUniformsCount);

		m_pUniforms[m_uiUniformsCount - 1].pName = new CString(rName);
		m_pUniforms[m_uiUniformsCount - 1].eSource = eSource;
		m_pUniforms[m_uiUniformsCount - 1].uiTarget = iTarget;
	}
}

void __fastcall CGPUProgram::AttributeAdd(_IN CString& rName, _IN ENUM eSource) _SET
{ 
	int iTarget = AttributeTarget(rName);

	if (iTarget != -1)
	{
		m_uiAttributesCount++;
		m_pAttributes = (SBinding*)CMemory::Reallocate(m_pAttributes, sizeof(SBinding) * m_uiAttributesCount);

		m_pAttributes[m_uiAttributesCount - 1].pName = new CString(rName);
		m_pAttributes[m_uiAttributesCount - 1].eSource = eSource;
		m_pAttributes[m_uiAttributesCount - 1].uiTarget = iTarget;
	}
}

void CGPUProgram::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
	rWriter.BlockStart(STRING("shaderVertex"));
	PUTVALUE_TOSTRING("file", ShaderVertex(), CString);
	rWriter.BlockEnd();

	rWriter.BlockStart(STRING("shaderFragment"));
	PUTVALUE_TOSTRING("file", ShaderFragment(), CString);
	rWriter.BlockEnd();

	rWriter.BlockStart(STRING("attributes"));
	PUTVALUE_TOSTRING("count", AttributesCount(), CInt);

	for (UINT i = 0; i < AttributesCount(); i++)
	{
		rWriter.BlockStart(STRING("attribute"));

		PUTVALUE_TOSTRING("name",    *(Attribute(i)->pName), CString);
		PUTVALUE_TOSTRING("source",  *(SCFXML::CXMLEnumeration::Translate(STRING("SourcesAttributes"), Attribute(i)->eSource)), CString);

		rWriter.BlockEnd();
	}
	rWriter.BlockEnd();

	rWriter.BlockStart(STRING("uniforms"));
	PUTVALUE_TOSTRING("count", UniformsCount(), CInt);

	for (UINT i = 0; i < UniformsCount(); i++)
	{
		rWriter.BlockStart(STRING("uniform"));

		PUTVALUE_TOSTRING("name",   *(Uniform(i)->pName), CString);
		PUTVALUE_TOSTRING("source", *(SCFXML::CXMLEnumeration::Translate(STRING("SourcesUniforms"), Uniform(i)->eSource)), CString);

		rWriter.BlockEnd();
	}
	rWriter.BlockEnd();

	rWriter.BlockStart(STRING("samplers"));
	PUTVALUE_TOSTRING("count", SamplersCount(), CInt);

	for (UINT i = 0; i < SamplersCount(); i++)
	{
		rWriter.BlockStart(STRING("sampler"));

		PUTVALUE_TOSTRING("name",   *(Sampler(i)->pName), CString);
		PUTVALUE_TOSTRING("source", *(SCFXML::CXMLEnumeration::Translate(STRING("SourcesSamplers"), Sampler(i)->eSource)), CString);

		rWriter.BlockEnd();
	}
	rWriter.BlockEnd();
}

void CGPUProgram::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CString* pValue = NULL;

	rReader.GetBlock();
	GETVALUE { ShaderVertex(*pValue); }
	rReader.GetBlock();
	GETVALUE { ShaderFragment(*pValue); }

	CompileAndLink();

	//Attributes
	rReader.GetBlock();
	{
		UINT uiCount = 0;
		GETVALUE { uiCount = CInt(*pValue).Value(); }

		for (UINT i = 0; i < uiCount; i++)
		{
			rReader.GetBlock();
			CString* pName   = rReader.GetValue();
			CString* pSource = rReader.GetValue();

			if (pName && pSource)
			{ 
				CEnum* pEnum = SCFXML::CXMLEnumeration::Translate(STRING("SourcesAttributes"), *pSource);
				if (pEnum) { AttributeAdd(*pName, pEnum->Value()); }
			}
		}
	}

	//Uniforms
	rReader.GetBlock();
	{
		UINT uiCount = 0;
		GETVALUE { uiCount = CInt(*pValue).Value(); }

		for (UINT i = 0; i < uiCount; i++)
		{
			rReader.GetBlock();
			CString* pName   = rReader.GetValue();
			CString* pSource = rReader.GetValue();

			if (pName && pSource)
			{ 
				CEnum* pEnum = SCFXML::CXMLEnumeration::Translate(STRING("SourcesUniforms"), *pSource);
				if (pEnum) { UniformAdd(*pName, pEnum->Value()); }
			}
		}
	}

	//Samplers
	rReader.GetBlock();
	{
		UINT uiCount = 0;
		GETVALUE { uiCount = CInt(*pValue).Value(); }

		for (UINT i = 0; i < uiCount; i++)
		{
			rReader.GetBlock();
			CString* pName   = rReader.GetValue();
			CString* pSource = rReader.GetValue();

			if (pName && pSource)
			{ 
				CEnum* pEnum = SCFXML::CXMLEnumeration::Translate(STRING("SourcesSamplers"), *pSource);
				if (pEnum) { SamplerAdd(*pName, pEnum->Value()); }
			}
		}
	}
}