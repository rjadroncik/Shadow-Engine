#pragma once

#include "Proxy.h"

#include "../Resources/GPUProgram.h"

namespace Scripting
{
	class SECORE_API CProxyGPUProgram : public CProxy
	{
	public:
		static void Register();
		static CXMLObjectSerializable* New() { return new CProxyGPUProgram(); }

	public:
		SCF::ENUM ClassKey() _GET { return ClassProxyGPUProgram; }
		CString   AsString() _GET { return STRING("{ProxyGPUProgram}"); }

	public:
		CProxyGPUProgram();
		virtual ~CProxyGPUProgram();

	public:
		Resources::CGPUProgram& Value() { return *m_pObject; }

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	protected:
		Resources::CGPUProgram* m_pObject;
	};
};
