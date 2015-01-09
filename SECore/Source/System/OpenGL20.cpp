#include "OpenGL20.h"

#include "../SECore.h"

#include <SCFWinAPIUndef.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

//Vertex & fragment program
PFNGLVERTEXATTRIB1DARBPROC				glVertexAttrib1dARB					= NULL;
PFNGLVERTEXATTRIB1DVARBPROC				glVertexAttrib1dvARB				= NULL;
PFNGLVERTEXATTRIB1FARBPROC				glVertexAttrib1fARB					= NULL;
PFNGLVERTEXATTRIB1FVARBPROC				glVertexAttrib1fvARB				= NULL;
PFNGLVERTEXATTRIB1SARBPROC				glVertexAttrib1sARB					= NULL;
PFNGLVERTEXATTRIB1SVARBPROC				glVertexAttrib1svARB				= NULL;
PFNGLVERTEXATTRIB2DARBPROC				glVertexAttrib2dARB					= NULL;
PFNGLVERTEXATTRIB2DVARBPROC				glVertexAttrib2dvARB				= NULL;
PFNGLVERTEXATTRIB2FARBPROC				glVertexAttrib2fARB					= NULL;
PFNGLVERTEXATTRIB2FVARBPROC				glVertexAttrib2fvARB				= NULL;
PFNGLVERTEXATTRIB2SARBPROC				glVertexAttrib2sARB					= NULL;
PFNGLVERTEXATTRIB2SVARBPROC				glVertexAttrib2svARB				= NULL;
PFNGLVERTEXATTRIB3DARBPROC				glVertexAttrib3dARB					= NULL;
PFNGLVERTEXATTRIB3DVARBPROC				glVertexAttrib3dvARB				= NULL;
PFNGLVERTEXATTRIB3FARBPROC				glVertexAttrib3fARB					= NULL;
PFNGLVERTEXATTRIB3FVARBPROC				glVertexAttrib3fvARB				= NULL;
PFNGLVERTEXATTRIB3SARBPROC				glVertexAttrib3sARB					= NULL;
PFNGLVERTEXATTRIB3SVARBPROC				glVertexAttrib3svARB				= NULL;
PFNGLVERTEXATTRIB4NBVARBPROC			glVertexAttrib4NbvARB				= NULL;
PFNGLVERTEXATTRIB4NIVARBPROC			glVertexAttrib4NivARB				= NULL;
PFNGLVERTEXATTRIB4NSVARBPROC			glVertexAttrib4NsvARB				= NULL;
PFNGLVERTEXATTRIB4NUBARBPROC			glVertexAttrib4NubARB				= NULL;
PFNGLVERTEXATTRIB4NUBVARBPROC			glVertexAttrib4NubvARB				= NULL;
PFNGLVERTEXATTRIB4NUIVARBPROC			glVertexAttrib4NuivARB				= NULL;
PFNGLVERTEXATTRIB4NUSVARBPROC			glVertexAttrib4NusvARB				= NULL;
PFNGLVERTEXATTRIB4BVARBPROC				glVertexAttrib4bvARB				= NULL;
PFNGLVERTEXATTRIB4DARBPROC				glVertexAttrib4dARB					= NULL;
PFNGLVERTEXATTRIB4DVARBPROC				glVertexAttrib4dvARB				= NULL;
PFNGLVERTEXATTRIB4FARBPROC				glVertexAttrib4fARB					= NULL;
PFNGLVERTEXATTRIB4FVARBPROC				glVertexAttrib4fvARB				= NULL;
PFNGLVERTEXATTRIB4IVARBPROC				glVertexAttrib4ivARB				= NULL;
PFNGLVERTEXATTRIB4SARBPROC				glVertexAttrib4sARB					= NULL;
PFNGLVERTEXATTRIB4SVARBPROC				glVertexAttrib4svARB				= NULL;
PFNGLVERTEXATTRIB4UBVARBPROC			glVertexAttrib4ubvARB				= NULL;
PFNGLVERTEXATTRIB4UIVARBPROC			glVertexAttrib4uivARB				= NULL;
PFNGLVERTEXATTRIB4USVARBPROC			glVertexAttrib4usvARB				= NULL;
PFNGLVERTEXATTRIBPOINTERARBPROC			glVertexAttribPointerARB			= NULL;
PFNGLENABLEVERTEXATTRIBARRAYARBPROC		glEnableVertexAttribArrayARB		= NULL;
PFNGLDISABLEVERTEXATTRIBARRAYARBPROC	glDisableVertexAttribArrayARB		= NULL;
PFNGLPROGRAMSTRINGARBPROC				glProgramStringARB					= NULL;
PFNGLBINDPROGRAMARBPROC					glBindProgramARB					= NULL;
PFNGLDELETEPROGRAMSARBPROC				glDeleteProgramsARB					= NULL;
PFNGLGENPROGRAMSARBPROC					glGenProgramsARB					= NULL;
PFNGLPROGRAMENVPARAMETER4DARBPROC		glProgramEnvParameter4dARB			= NULL;
PFNGLPROGRAMENVPARAMETER4DVARBPROC		glProgramEnvParameter4dvARB			= NULL;
PFNGLPROGRAMENVPARAMETER4FARBPROC		glProgramEnvParameter4fARB			= NULL;
PFNGLPROGRAMENVPARAMETER4FVARBPROC		glProgramEnvParameter4fvARB			= NULL;
PFNGLPROGRAMLOCALPARAMETER4DARBPROC		glProgramLocalParameter4dARB		= NULL;
PFNGLPROGRAMLOCALPARAMETER4DVARBPROC	glProgramLocalParameter4dvARB		= NULL;
PFNGLPROGRAMLOCALPARAMETER4FARBPROC		glProgramLocalParameter4fARB		= NULL;
PFNGLPROGRAMLOCALPARAMETER4FVARBPROC	glProgramLocalParameter4fvARB		= NULL;
PFNGLGETPROGRAMENVPARAMETERDVARBPROC	glGetProgramEnvParameterdvARB		= NULL;
PFNGLGETPROGRAMENVPARAMETERFVARBPROC	glGetProgramEnvParameterfvARB		= NULL;
PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC	glGetProgramLocalParameterdvARB		= NULL;
PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC	glGetProgramLocalParameterfvARB		= NULL;
PFNGLGETPROGRAMIVARBPROC				glGetProgramivARB					= NULL;
PFNGLGETPROGRAMSTRINGARBPROC			glGetProgramStringARB				= NULL;
PFNGLGETVERTEXATTRIBDVARBPROC			glGetVertexAttribdvARB				= NULL;
PFNGLGETVERTEXATTRIBFVARBPROC			glGetVertexAttribfvARB				= NULL;
PFNGLGETVERTEXATTRIBIVARBPROC			glGetVertexAttribivARB				= NULL;
PFNGLGETVERTEXATTRIBPOINTERVARBPROC		glGetVertexAttribPointervARB		= NULL;
PFNGLISPROGRAMARBPROC					glIsProgramARB						= NULL;

//Shader objects
PFNGLDELETEOBJECTARBPROC         glDeleteObjectARB         = NULL;
PFNGLGETHANDLEARBPROC            glGetHandleARB            = NULL;
PFNGLDETACHOBJECTARBPROC         glDetachObjectARB         = NULL;
PFNGLCREATESHADEROBJECTARBPROC   glCreateShaderObjectARB   = NULL;
PFNGLSHADERSOURCEARBPROC         glShaderSourceARB         = NULL;
PFNGLCOMPILESHADERARBPROC        glCompileShaderARB        = NULL;
PFNGLCREATEPROGRAMOBJECTARBPROC  glCreateProgramObjectARB  = NULL;
PFNGLATTACHOBJECTARBPROC         glAttachObjectARB         = NULL;
PFNGLLINKPROGRAMARBPROC          glLinkProgramARB          = NULL;
PFNGLUSEPROGRAMOBJECTARBPROC     glUseProgramObjectARB     = NULL;
PFNGLVALIDATEPROGRAMARBPROC      glValidateProgramARB      = NULL;
PFNGLUNIFORM1FARBPROC            glUniform1fARB            = NULL;
PFNGLUNIFORM2FARBPROC            glUniform2fARB            = NULL;
PFNGLUNIFORM3FARBPROC            glUniform3fARB            = NULL;
PFNGLUNIFORM4FARBPROC            glUniform4fARB            = NULL;
PFNGLUNIFORM1IARBPROC            glUniform1iARB            = NULL;
PFNGLUNIFORM2IARBPROC            glUniform2iARB            = NULL;
PFNGLUNIFORM3IARBPROC	         glUniform3iARB            = NULL;
PFNGLUNIFORM4IARBPROC            glUniform4iARB            = NULL;
PFNGLUNIFORM1FVARBPROC           glUniform1fvARB           = NULL;
PFNGLUNIFORM2FVARBPROC           glUniform2fvARB           = NULL;
PFNGLUNIFORM3FVARBPROC           glUniform3fvARB           = NULL;
PFNGLUNIFORM4FVARBPROC           glUniform4fvARB           = NULL;
PFNGLUNIFORM1IVARBPROC           glUniform1ivARB           = NULL;
PFNGLUNIFORM2IVARBPROC           glUniform2ivARB           = NULL;
PFNGLUNIFORM3IVARBPROC           glUniform3ivARB           = NULL;
PFNGLUNIFORM4IVARBPROC           glUniform4ivARB           = NULL;
PFNGLUNIFORMMATRIX2FVARBPROC     glUniformMatrix2fvARB     = NULL;
PFNGLUNIFORMMATRIX3FVARBPROC     glUniformMatrix3fvARB     = NULL;
PFNGLUNIFORMMATRIX4FVARBPROC     glUniformMatrix4fvARB     = NULL;
PFNGLGETOBJECTPARAMETERFVARBPROC glGetObjectParameterfvARB = NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB = NULL;
PFNGLGETINFOLOGARBPROC           glGetInfoLogARB           = NULL;
PFNGLGETATTACHEDOBJECTSARBPROC   glGetAttachedObjectsARB   = NULL;
PFNGLGETUNIFORMLOCATIONARBPROC   glGetUniformLocationARB   = NULL;
PFNGLGETACTIVEUNIFORMARBPROC     glGetActiveUniformARB     = NULL;
PFNGLGETUNIFORMFVARBPROC         glGetUniformfvARB         = NULL;
PFNGLGETUNIFORMIVARBPROC         glGetUniformivARB         = NULL;
PFNGLGETSHADERSOURCEARBPROC      glGetShaderSourceARB      = NULL;

//Vertex shader
PFNGLBINDATTRIBLOCATIONARBPROC glBindAttribLocationARB = NULL;
PFNGLGETACTIVEATTRIBARBPROC    glGetActiveAttribARB    = NULL;
PFNGLGETATTRIBLOCATIONARBPROC  glGetAttribLocationARB  = NULL;

using namespace System;

extern SCF::UINT OpenGL_uiVersionNumber;
extern CString OpenGL_Extensions;

CString OpenGL_GLSLVersion;

SCF::UINT OpenGL_uiGLSLVersionNumber = 0;

inline SCF::UINT COpenGL20::GLSLVersionNumber() { return OpenGL_uiGLSLVersionNumber; }

bool COpenGL20::Initialize()
{
	SetLastError(NULL);

	CEventLog::BlockNew(STRING("Initializing OpenGL 2.0 core extensions"));
	CEventLog::BlockNew(STRING("OpenGL 2.0"));

	bool bNullFunction = FALSE;
	CStringSearch Search(OpenGL_Extensions);

	//Non vendor specific extensions
	CEventLog::EventNew(STRING("GL_ARB_vertex_program, GL_ARB_fragment_program"));
	if ((Search.FindString(STRING("GL_ARB_vertex_program")) != -1) && (Search.FindString(STRING("GL_ARB_fragment_program")) != -1))
	{
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB1DARBPROC,             glVertexAttrib1dARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB1DVARBPROC,            glVertexAttrib1dvARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB1FARBPROC,             glVertexAttrib1fARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB1FVARBPROC,            glVertexAttrib1fvARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB1SARBPROC,             glVertexAttrib1sARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB1SVARBPROC,            glVertexAttrib1svARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB2DARBPROC,             glVertexAttrib2dARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB2DVARBPROC,            glVertexAttrib2dvARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB2FARBPROC,             glVertexAttrib2fARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB2FVARBPROC,            glVertexAttrib2fvARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB2SARBPROC,             glVertexAttrib2sARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB2SVARBPROC,            glVertexAttrib2svARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB3DARBPROC,             glVertexAttrib3dARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB3DVARBPROC,            glVertexAttrib3dvARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB3FARBPROC,             glVertexAttrib3fARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB3FVARBPROC,            glVertexAttrib3fvARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB3SARBPROC,             glVertexAttrib3sARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB3SVARBPROC,            glVertexAttrib3svARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB4NBVARBPROC,           glVertexAttrib4NbvARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB4NIVARBPROC,           glVertexAttrib4NivARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB4NSVARBPROC,           glVertexAttrib4NsvARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB4NUBARBPROC,           glVertexAttrib4NubARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB4NUBVARBPROC,          glVertexAttrib4NubvARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB4NUIVARBPROC,          glVertexAttrib4NuivARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB4NUSVARBPROC,          glVertexAttrib4NusvARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB4BVARBPROC,            glVertexAttrib4bvARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB4DARBPROC,             glVertexAttrib4dARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB4DVARBPROC,            glVertexAttrib4dvARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB4FARBPROC,             glVertexAttrib4fARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB4FVARBPROC,            glVertexAttrib4fvARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB4IVARBPROC,            glVertexAttrib4ivARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB4SARBPROC,             glVertexAttrib4sARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB4SVARBPROC,            glVertexAttrib4svARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB4UBVARBPROC,           glVertexAttrib4ubvARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB4UIVARBPROC,           glVertexAttrib4uivARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIB4USVARBPROC,           glVertexAttrib4usvARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVERTEXATTRIBPOINTERARBPROC,        glVertexAttribPointerARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLENABLEVERTEXATTRIBARRAYARBPROC,    glEnableVertexAttribArrayARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLDISABLEVERTEXATTRIBARRAYARBPROC,   glDisableVertexAttribArrayARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLPROGRAMSTRINGARBPROC,              glProgramStringARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLBINDPROGRAMARBPROC,                glBindProgramARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLDELETEPROGRAMSARBPROC,             glDeleteProgramsARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLGENPROGRAMSARBPROC,                glGenProgramsARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLPROGRAMENVPARAMETER4DARBPROC,      glProgramEnvParameter4dARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLPROGRAMENVPARAMETER4DVARBPROC,     glProgramEnvParameter4dvARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLPROGRAMENVPARAMETER4FARBPROC,      glProgramEnvParameter4fARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLPROGRAMENVPARAMETER4FVARBPROC,     glProgramEnvParameter4fvARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLPROGRAMLOCALPARAMETER4DARBPROC,    glProgramLocalParameter4dARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLPROGRAMLOCALPARAMETER4DVARBPROC,   glProgramLocalParameter4dvARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLPROGRAMLOCALPARAMETER4FARBPROC,    glProgramLocalParameter4fARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLPROGRAMLOCALPARAMETER4FVARBPROC,   glProgramLocalParameter4fvARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLGETPROGRAMENVPARAMETERDVARBPROC,   glGetProgramEnvParameterdvARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLGETPROGRAMENVPARAMETERFVARBPROC,   glGetProgramEnvParameterfvARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC, glGetProgramLocalParameterdvARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC, glGetProgramLocalParameterfvARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLGETPROGRAMIVARBPROC,               glGetProgramivARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLGETPROGRAMSTRINGARBPROC,           glGetProgramStringARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLGETVERTEXATTRIBDVARBPROC,          glGetVertexAttribdvARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLGETVERTEXATTRIBFVARBPROC,          glGetVertexAttribfvARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLGETVERTEXATTRIBIVARBPROC,          glGetVertexAttribivARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLGETVERTEXATTRIBPOINTERVARBPROC,    glGetVertexAttribPointervARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLISPROGRAMARBPROC,                  glIsProgramARB);
	}
	else 
	{ 
		SCFError(ErrorHardwareUnsupported); 
		return FALSE;
	}

	//Non vendor specific extensions
	CEventLog::EventNew(STRING("GL_ARB_shader_objects"));
	if (Search.FindString(STRING("GL_ARB_shader_objects")) != -1)
	{
		bNullFunction |= LOAD_FUNCTION(PFNGLDELETEOBJECTARBPROC,         glDeleteObjectARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLGETHANDLEARBPROC,            glGetHandleARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLDETACHOBJECTARBPROC,         glDetachObjectARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLCREATESHADEROBJECTARBPROC,   glCreateShaderObjectARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLSHADERSOURCEARBPROC,         glShaderSourceARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLCOMPILESHADERARBPROC,        glCompileShaderARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLCREATEPROGRAMOBJECTARBPROC,  glCreateProgramObjectARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLATTACHOBJECTARBPROC,         glAttachObjectARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLLINKPROGRAMARBPROC,          glLinkProgramARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLUSEPROGRAMOBJECTARBPROC,     glUseProgramObjectARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLVALIDATEPROGRAMARBPROC,      glValidateProgramARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLUNIFORM1FARBPROC,            glUniform1fARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLUNIFORM2FARBPROC,            glUniform2fARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLUNIFORM3FARBPROC,            glUniform3fARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLUNIFORM4FARBPROC,            glUniform4fARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLUNIFORM1IARBPROC,            glUniform1iARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLUNIFORM2IARBPROC,            glUniform2iARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLUNIFORM3IARBPROC,            glUniform3iARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLUNIFORM4IARBPROC,            glUniform4iARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLUNIFORM1FVARBPROC,           glUniform1fvARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLUNIFORM2FVARBPROC,           glUniform2fvARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLUNIFORM3FVARBPROC,           glUniform3fvARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLUNIFORM4FVARBPROC,           glUniform4fvARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLUNIFORM1IVARBPROC,           glUniform1ivARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLUNIFORM2IVARBPROC,           glUniform2ivARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLUNIFORM3IVARBPROC,           glUniform3ivARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLUNIFORM4IVARBPROC,           glUniform4ivARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLUNIFORMMATRIX2FVARBPROC,     glUniformMatrix2fvARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLUNIFORMMATRIX3FVARBPROC,     glUniformMatrix3fvARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLUNIFORMMATRIX4FVARBPROC,     glUniformMatrix4fvARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLGETOBJECTPARAMETERFVARBPROC, glGetObjectParameterfvARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLGETOBJECTPARAMETERIVARBPROC, glGetObjectParameterivARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLGETINFOLOGARBPROC,           glGetInfoLogARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLGETATTACHEDOBJECTSARBPROC,   glGetAttachedObjectsARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLGETUNIFORMLOCATIONARBPROC,   glGetUniformLocationARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLGETACTIVEUNIFORMARBPROC,     glGetActiveUniformARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLGETUNIFORMFVARBPROC,         glGetUniformfvARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLGETUNIFORMIVARBPROC,         glGetUniformivARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLGETSHADERSOURCEARBPROC,      glGetShaderSourceARB);
	}
	else 
	{ 
		SCFError(ErrorHardwareUnsupported); 
		return FALSE;
	}

	//Non vendor specific extensions
	CEventLog::EventNew(STRING("GL_ARB_vertex_shader, GL_ARB_fragment_shader"));
	if ((Search.FindString(STRING("GL_ARB_vertex_shader")) != -1) && (Search.FindString(STRING("GL_ARB_fragment_shader")) != -1))
	{
		bNullFunction |= LOAD_FUNCTION(PFNGLBINDATTRIBLOCATIONARBPROC, glBindAttribLocationARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLGETACTIVEATTRIBARBPROC,    glGetActiveAttribARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLGETATTRIBLOCATIONARBPROC,  glGetAttribLocationARB);
	}
	else 
	{ 
		SCFError(ErrorHardwareUnsupported); 
		return FALSE;
	}

	CEventLog::EventNew(STRING("GL_ARB_shading_language_100"));
	if (Search.FindString(STRING("GL_ARB_shading_language_100")) != -1)
	{
		OpenGL_GLSLVersion = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION_ARB);

		CString Temp = CStringRange(OpenGL_GLSLVersion, 0, 3);
		OpenGL_uiGLSLVersionNumber = (SCF::UINT)(CFloat(Temp).Value() * 10 + 0.5f);

		CEventLog::EventNew(STRING("GLSL version: ") + OpenGL_GLSLVersion);

		if (OpenGL_uiGLSLVersionNumber < 12) 
		{
			SCFError(ErrorHardwareUnsupported); 
			return FALSE;
		}
	}
	else 
	{ 
		SCFError(ErrorHardwareUnsupported); 
		return FALSE;
	}

	CEventLog::EventNew(STRING("GL_ARB_point_sprite"));
	if (Search.FindString(STRING("GL_ARB_point_sprite")) != -1)
	{
	}
	else 
	{ 
		SCFError(ErrorHardwareUnsupported); 
		return FALSE;
	}

	CEventLog::BlockClose();
	CEventLog::BlockNew(STRING("Additional extensions"));

	CEventLog::BlockClose();

	if (bNullFunction) 
	{
		SCFError(ErrorHardwareUnsupported); 
		return FALSE;
	}
	CEventLog::BlockClose();

	return TRUE;
}
