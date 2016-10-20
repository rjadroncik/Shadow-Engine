#pragma once

#include "../SECore.h"
#include "../System/SEObject.h"

namespace Simulation { class CParticleSystem; };
namespace Resources  { class CMesh; };

namespace Resources
{
	struct SBinding
	{
		//Represents tha variable name in the shader
		CString* pName;
		
		//Source is a numeric constant interpreted by the engine
		ENUM eSource;
		//Target is the numeric position assigned to the ariable by the GL shader compiler
		UINT uiTarget;
	};

	class SECORE_API CGPUProgram : public System::CSEObject
	{
	public:
		System::Category Category() _GET { return System::Category::GPUPrograms; }

	public:
		CString XmlName() _GET { return STRING("GPUProgram"); }
		CString ToString() _GET { return STRING("{GPUProgram}"); }

	public:
		CGPUProgram();
		virtual ~CGPUProgram();

	public:
		inline GLhandleARB Handle() _GET { return m_hGLProgram; }

	public:
		bool __fastcall ShaderVertex  (_IN CString& rFilePath) _SET;
		bool __fastcall ShaderFragment(_IN CString& rFilePath) _SET;

	public:
		inline CString ShaderVertex()   _GET { return STRINGREF(m_ShaderFileVertex); }
		inline CString ShaderFragment() _GET { return STRINGREF(m_ShaderFileFragment); }

	public:
		bool __fastcall CompileAndLink();
		void __fastcall InfoLogsPrint();
		bool __fastcall Relink();

	public:
		int __fastcall AttributeTarget(_IN CString& rName);
		int __fastcall UniformTarget  (_IN CString& rName);

	public:
		void __fastcall SamplerAdd  (_IN CString& rName, _IN ENUM eSource) _SET;
		void __fastcall UniformAdd  (_IN CString& rName, _IN ENUM eSource) _SET;
		void __fastcall AttributeAdd(_IN CString& rName, _IN ENUM eSource) _SET;

		inline const SBinding* Sampler(_IN UINT uiIndex) _GET { return &m_pSamplers[uiIndex]; }
		inline UINT       SamplersCount()                _GET { return m_uiSamplersCount; }

		inline const SBinding* Uniform(_IN UINT uiIndex) _GET { return &m_pUniforms[uiIndex]; }
		inline UINT       UniformsCount()                _GET { return m_uiUniformsCount; }

		inline const SBinding* Attribute(_IN UINT uiIndex) _GET { return &m_pAttributes[uiIndex]; }
		inline UINT       AttributesCount()                _GET { return m_uiAttributesCount; }

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	protected:
		GLhandleARB m_hGLShaderVertex;
		GLhandleARB m_hGLShaderFragment;
		GLhandleARB m_hGLProgram;

	protected:
		SBinding* m_pSamplers;
		UINT m_uiSamplersCount;

		SBinding* m_pUniforms;
		UINT m_uiUniformsCount;

		SBinding* m_pAttributes;
		UINT m_uiAttributesCount;

	protected:
		CString m_ShaderFileVertex;
		CString m_ShaderFileFragment;
	};
};