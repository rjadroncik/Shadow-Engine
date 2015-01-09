#include "Light.h"

using namespace Rendering;

CLight::CLight()
{
	m_fRange = 10000.0f;

	m_fScissorFactor = 2.0f;
	m_fScissorRange  = 0.0f;

	SetVector3(m_Tint, 1, 1, 1);

	m_fAmbient  = 0.0f;
	m_fDiffuse  = 1.0f;
	m_fSpecular = 1.0f;

	m_bEnabled = TRUE;
	
	m_bCastShadows   = FALSE;
	m_bShadowsUpdate = TRUE;

	m_iShadowMapLOD = 0;
}

CLight::~CLight()
{
}

void __fastcall CLight::Use() const
{
	Float4 Ambient  = { m_Tint[0], m_Tint[1], m_Tint[2], m_fAmbient };
	Float4 Diffuse  = { m_Tint[0], m_Tint[1], m_Tint[2], m_fDiffuse };
	Float4 Specular = { m_Tint[0], m_Tint[1], m_Tint[2], m_fSpecular };

 	//Update light color properties - used within fragment program
	glLightfv(GL_LIGHT0, GL_AMBIENT,  Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  Diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Specular);
}

void CLight::XMLSerialize(_INOUT SCFXML::IXMLStreamWrite& rWriter) const
{
	CObjectScene::XMLSerialize(rWriter);

	PUTVALUENEW("tint",     Tint(),     CFloat3);
	PUTVALUENEW("ambient",  Ambient(),  CFloat);
	PUTVALUENEW("diffuse",  Diffuse(),  CFloat);
	PUTVALUENEW("specular", Specular(), CFloat);
	PUTVALUENEW("range",    Range(),    CFloat);
	PUTVALUENEW("shadows",  Shadows(),  CBool);

	if (Shadows())
	{
		PUTVALUENEW("shadowMapLOD", ShadowMapLOD(), CInt);
	}
}

void CLight::XMLDeserialize(_INOUT SCFXML::IXMLStreamRead& rReader)
{
	CObjectScene::XMLDeserialize(rReader);

	CString* pValue = NULL;

	GETVALUE { Tint    (CFloat3(*pValue).Value()); }
	GETVALUE { Ambient (CFloat(*pValue).Value()); }
	GETVALUE { Diffuse (CFloat(*pValue).Value()); }
	GETVALUE { Specular(CFloat(*pValue).Value()); }
	GETVALUE { Range   (CFloat(*pValue).Value()); }

	GETVALUE 
	{
		Shadows(CBool(*pValue).Value());
		if (Shadows())
		{
			GETVALUE { ShadowMapLOD(CInt(*pValue).Value()); }
		}
	}

	BoundsUpdate();
}
