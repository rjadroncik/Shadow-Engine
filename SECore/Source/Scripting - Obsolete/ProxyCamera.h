#pragma once

#include "ProxyObjectScene.h"

#include "../Rendering/Camera.h"

namespace Scripting
{
	class SECORE_API CProxyCamera : public CProxyObjectScene
	{
	public:
		static void Register();
		static CXMLObjectSerializable* New() { return new CProxyCamera(); }

	public:
		SCF::ENUM ClassKey() _GET { return ClassProxyCamera; }
		CString   AsString() _GET { return STRING("{ProxyCamera}"); }

	public:
		CProxyCamera();
		virtual ~CProxyCamera();

	public:
		Rendering::CCamera& Value() { return *(Rendering::CCamera*)m_pObject; }

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);
	};
};
