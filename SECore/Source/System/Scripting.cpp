#include "Scripting.h"


#include "../Resources/Texture.h"
#include "../Resources/GPUProgram.h"
#include "../Resources/Mesh.h"

#include "../Rendering/RendererSettings.h"
#include "../Rendering/Material.h"
#include "../Rendering/Model.h"
#include "../Rendering/LightSpot.h"
#include "../Rendering/LightOmni.h"
#include "../Rendering/Camera.h"
#include "../Simulation/ParticleSystem.h"
#include "../Rendering/Scene.h"


#include <SCFScripting.h>

using namespace System;

CDictionaryString<CSEObject> CScripting::s_Objects;

#include <SCFWinAPIUndef.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void __fastcall CScripting::Initialize(_IN void* hModule)
{
	CLASS_XMLSERIALIZABLE_REGISTER("Resources:Texture",    Resources::CTexture,    (HMODULE)hModule);
	CLASS_XMLSERIALIZABLE_REGISTER("Resources:GPUProgram", Resources::CGPUProgram, (HMODULE)hModule);
	CLASS_XMLSERIALIZABLE_REGISTER("Resources:Mesh",       Resources::CMesh,       (HMODULE)hModule);
	
	CLASS_XMLSERIALIZABLE_REGISTER("Rendering:Material",  Rendering::CMaterial,  (HMODULE)hModule);
	CLASS_XMLSERIALIZABLE_REGISTER("Rendering:Model",     Rendering::CModel,     (HMODULE)hModule);
	CLASS_XMLSERIALIZABLE_REGISTER("Rendering:LightSpot", Rendering::CLightSpot, (HMODULE)hModule);
	CLASS_XMLSERIALIZABLE_REGISTER("Rendering:LightOmni", Rendering::CLightOmni, (HMODULE)hModule);
	CLASS_XMLSERIALIZABLE_REGISTER("Rendering:Camera",    Rendering::CCamera,    (HMODULE)hModule);
	CLASS_XMLSERIALIZABLE_REGISTER("Rendering:Scene",     Rendering::CScene,     (HMODULE)hModule);

	CLASS_XMLSERIALIZABLE_REGISTER("Simulation:ParticleSystem", Simulation::CParticleSystem, (HMODULE)hModule);

	CNamespace& rGUI = CREATE_NAMESPACE(GUI);

	SCFXML::CXMLEnumeration* pEnumeration = new SCFXML::CXMLEnumeration();
	{
		pEnumeration->ValueDefine(STRING("1D"),      GL_TEXTURE_1D);
		pEnumeration->ValueDefine(STRING("2D"),      GL_TEXTURE_2D);
		pEnumeration->ValueDefine(STRING("3D"),      GL_TEXTURE_3D);
		pEnumeration->ValueDefine(STRING("cubeMap"), GL_TEXTURE_CUBE_MAP);
	}
	SCFXML::CXMLEnumeration::Register(STRING("TextureTargets"), *pEnumeration);

	pEnumeration = new SCFXML::CXMLEnumeration();
	{
		pEnumeration->ValueDefine(STRING("nearest"), GL_NEAREST);
		pEnumeration->ValueDefine(STRING("linear"),  GL_LINEAR);
		pEnumeration->ValueDefine(STRING("nearestMipmapNearest"), GL_NEAREST_MIPMAP_NEAREST);
		pEnumeration->ValueDefine(STRING("linearMipmapNearest"),  GL_LINEAR_MIPMAP_NEAREST);
		pEnumeration->ValueDefine(STRING("nearestMipmapLinear"),  GL_NEAREST_MIPMAP_LINEAR);
		pEnumeration->ValueDefine(STRING("linearMipmapLinear"),   GL_LINEAR_MIPMAP_LINEAR);
	}
	SCFXML::CXMLEnumeration::Register(STRING("TextureMinifications"), *pEnumeration);

	pEnumeration = new SCFXML::CXMLEnumeration();
	{
		pEnumeration->ValueDefine(STRING("nearest"), GL_NEAREST);
		pEnumeration->ValueDefine(STRING("linear"),  GL_LINEAR);
	}
	SCFXML::CXMLEnumeration::Register(STRING("TextureMagnifications"), *pEnumeration);

	pEnumeration = new SCFXML::CXMLEnumeration();
	{
		pEnumeration->ValueDefine(STRING("clamp"),         GL_CLAMP);
		pEnumeration->ValueDefine(STRING("clampToBorder"), GL_CLAMP_TO_BORDER);
		pEnumeration->ValueDefine(STRING("repeat"),        GL_REPEAT);
	}
	SCFXML::CXMLEnumeration::Register(STRING("TextureCoordClamps"), *pEnumeration);

	pEnumeration = new SCFXML::CXMLEnumeration();
	{
		pEnumeration->ValueDefine(STRING("vertex.texCoord[0]"), AS_VERTEX_TEXCOORD0);
		pEnumeration->ValueDefine(STRING("vertex.texCoord[1]"), AS_VERTEX_TEXCOORD1);
		pEnumeration->ValueDefine(STRING("vertex.texCoord[2]"), AS_VERTEX_TEXCOORD2);
		pEnumeration->ValueDefine(STRING("vertex.texCoord[3]"), AS_VERTEX_TEXCOORD3);

		pEnumeration->ValueDefine(STRING("vertex.tangent"), AS_VERTEX_TANGENT);
		pEnumeration->ValueDefine(STRING("vertex.normal"),  AS_VERTEX_NORNAL);
	}
	SCFXML::CXMLEnumeration::Register(STRING("SourcesAttributes"), *pEnumeration);

	pEnumeration = new SCFXML::CXMLEnumeration();
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
	SCFXML::CXMLEnumeration::Register(STRING("SourcesUniforms"), *pEnumeration);

	pEnumeration = new SCFXML::CXMLEnumeration();
	{
		pEnumeration->ValueDefine(STRING("material.diffuseColor"), SS_MATERIAL_DIFFUSE_COLOR);
		pEnumeration->ValueDefine(STRING("material.normalMap"),    SS_MATERIAL_NORMALMAP);

		pEnumeration->ValueDefine(STRING("light.depthMap"),  SS_LIGHT_DEPTHMAP0);
		pEnumeration->ValueDefine(STRING("light.depthMap0"), SS_LIGHT_DEPTHMAP0);
		pEnumeration->ValueDefine(STRING("light.depthMap1"), SS_LIGHT_DEPTHMAP1);
	}
	SCFXML::CXMLEnumeration::Register(STRING("SourcesSamplers"), *pEnumeration);

	pEnumeration = new SCFXML::CXMLEnumeration();
	{
		pEnumeration->ValueDefine(STRING("spot"),         Rendering::UsageSpot);
		pEnumeration->ValueDefine(STRING("spotShadow"),   Rendering::UsageSpotShadow);
		pEnumeration->ValueDefine(STRING("omni"),         Rendering::UsageOmni);
		pEnumeration->ValueDefine(STRING("omniShadow"),   Rendering::UsageOmniShadow);
		pEnumeration->ValueDefine(STRING("direct"),       Rendering::UsageDirect);
		pEnumeration->ValueDefine(STRING("directShadow"), Rendering::UsageDirectShadow);
	}
	SCFXML::CXMLEnumeration::Register(STRING("MaterialUsages"), *pEnumeration);

	pEnumeration = new SCFXML::CXMLEnumeration();
	{
		pEnumeration->ValueDefine(STRING("zero"), GL_ZERO);
		pEnumeration->ValueDefine(STRING("one"),  GL_ONE);

		pEnumeration->ValueDefine(STRING("srcColor"),         GL_SRC_COLOR);
		pEnumeration->ValueDefine(STRING("oneMinusSrcColor"), GL_ONE_MINUS_SRC_COLOR);
		pEnumeration->ValueDefine(STRING("srcAlpha"),         GL_SRC_ALPHA);
		pEnumeration->ValueDefine(STRING("oneMinusSrcAlpha"), GL_ONE_MINUS_SRC_ALPHA);

		pEnumeration->ValueDefine(STRING("dstAlpha"),         GL_DST_ALPHA);
		pEnumeration->ValueDefine(STRING("oneMinusDstAlpha"), GL_ONE_MINUS_DST_ALPHA);
		pEnumeration->ValueDefine(STRING("dstColor"),         GL_DST_COLOR);
		pEnumeration->ValueDefine(STRING("oneMinusDstColor"), GL_ONE_MINUS_DST_COLOR);

		pEnumeration->ValueDefine(STRING("srcAlphaSaturate"), GL_SRC_ALPHA_SATURATE);
	}
	SCFXML::CXMLEnumeration::Register(STRING("BlendFactors"), *pEnumeration);

	pEnumeration = new SCFXML::CXMLEnumeration();
	{
		pEnumeration->ValueDefine(STRING("OpenGL20DGC"), Rendering::CodePathOpenGL20DGC);
		pEnumeration->ValueDefine(STRING("OpenGL20IGP"), Rendering::CodePathOpenGL20IGP);
	}
	SCFXML::CXMLEnumeration::Register(STRING("CodePaths"), *pEnumeration);
}