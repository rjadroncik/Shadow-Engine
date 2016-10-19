#pragma once

#ifdef SECORE_EXPORTS
#define SECORE_API __declspec(dllexport)
#else
#define SECORE_API __declspec(dllimport)
#endif

#include <SCFObjectExtensions.h>
#include <SCFMathematics.h>

#include <SCFWinAPI.h>
#include <OpenGL\gl.h>

#undef GL_VERSION_1_1
#undef GL_VERSION_1_2
#undef GL_VERSION_1_3
#undef GL_VERSION_1_4
#undef GL_VERSION_1_5
#undef GL_VERSION_2_0

#include <OpenGL\glext.h>
#include <OpenGL\glu.h>

#include "Errors.h"

#include "System/EventLog.h"
#include "System/OpenGL20.h"

using namespace SCFMathematics;
using namespace SCFBase;

enum SourcesAttributes
{
	AS_VERTEX_TEXCOORD0,
	AS_VERTEX_TEXCOORD1,
	AS_VERTEX_TEXCOORD2,
	AS_VERTEX_TEXCOORD3,

	AS_VERTEX_TANGENT,
	AS_VERTEX_NORNAL,

	AS_COUNT,
};

enum SourcesUniforms
{
	LIGHT_PARAMS_START,

	US_LIGHT_POSITION,       //Position in world coord. system
	US_LIGHT_POSITION_LOCAL, //Position in object coord. system
	US_LIGHT_PARAMETERS,     //Range
	US_LIGHT_TINT,				
	US_LIGHT_AMB_DIF_SPE,				

	US_LIGHT_MATRIX_DEPTHMAP_TRANSFORM0,
	US_LIGHT_MATRIX_DEPTHMAP_TRANSFORM1,

	LIGHT_PARAMS_END,

	CAMERA_PARAMS_START,

	US_CAMERA_POSITION,       //Position in world coord. system
	US_CAMERA_POSITION_LOCAL, //Position in object coord. system
	US_CAMERA_VIEWPORT,       //Position & dimensions of viewport in pixels
	
	CAMERA_PARAMS_END,

	MATERIAL_PARAMS_START,

	US_MATERIAL_TINT,
	US_MATERIAL_AMB_DIF_SPE_EMI,				
	US_MATERIAL_SHININESS,
	
	MATERIAL_PARAMS_END,

	US_COUNT,
};

enum SourcesSamplers
{
	SS_MATERIAL_DIFFUSE_COLOR,
	SS_MATERIAL_NORMALMAP,

	SS_LIGHT_DEPTHMAP0,
	SS_LIGHT_DEPTHMAP1,

	SS_COUNT,
};


