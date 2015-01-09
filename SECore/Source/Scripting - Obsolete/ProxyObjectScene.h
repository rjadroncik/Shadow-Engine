#pragma once

#include "Proxy.h"

#include "../Rendering/ObjectScene.h"

namespace Scripting
{
	class SECORE_API CProxyObjectScene : public CProxy
	{
	public:
		static void Register();

	public:
		CProxyObjectScene();
		virtual ~CProxyObjectScene();

	public:
		void     CProxyObjectScene::PositionSet(_IN CFloat4& rValue) _SET { m_pObject->Position(((CFloat4&)rValue).Value()); }
		CFloat4* CProxyObjectScene::Position()                       _GET { return new CFloat4(m_pObject->Position()); }
	
		void     CProxyObjectScene::RotationSet(_IN CFloat3& rValue) _SET { m_pObject->Rotation(((CFloat3&)rValue).Value()); }
		CFloat3* CProxyObjectScene::Rotation()                       _GET { return new CFloat3(m_pObject->Rotation()); }

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	protected:
		Rendering::CObjectScene* m_pObject;
	};
};
