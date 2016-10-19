#include "OpenGL15.h"

#include "../SECore.h"

#include <SCFWinAPIUndef.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

//*** OpenGL core features ***//
PFNGLTEXIMAGE3DPROC						glTexImage3D						= NULL;
PFNGLSAMPLECOVERAGEPROC					glSampleCoverage					= NULL;

//*** Multi-texturing ***//
PFNGLMULTITEXCOORD2DPROC				glMultiTexCoord2d					= NULL;
PFNGLMULTITEXCOORD2DVPROC				glMultiTexCoord2dv					= NULL;
PFNGLMULTITEXCOORD3DPROC				glMultiTexCoord3d					= NULL;
PFNGLMULTITEXCOORD3DVPROC				glMultiTexCoord3dv					= NULL;
PFNGLMULTITEXCOORD4DPROC				glMultiTexCoord4d					= NULL;
PFNGLMULTITEXCOORD4DVPROC				glMultiTexCoord4dv					= NULL;

PFNGLACTIVETEXTUREPROC					glActiveTexture						= NULL;
PFNGLCLIENTACTIVETEXTUREPROC			glClientActiveTexture				= NULL;

//*** Point parameters ***//
PFNGLPOINTPARAMETERFARBPROC             glPointParameterfARB                = NULL;
PFNGLPOINTPARAMETERFVARBPROC            glPointParameterfvARB               = NULL;

//*** Vertex buffer objects ***//
PFNGLBINDBUFFERARBPROC					glBindBufferARB						= NULL;
PFNGLDELETEBUFFERSARBPROC				glDeleteBuffersARB					= NULL;
PFNGLGENBUFFERSARBPROC					glGenBuffersARB						= NULL;
PFNGLISBUFFERARBPROC					glIsBufferARB						= NULL;
PFNGLBUFFERDATAARBPROC					glBufferDataARB						= NULL;
PFNGLBUFFERSUBDATAARBPROC				glBufferSubDataARB					= NULL;
PFNGLGETBUFFERSUBDATAARBPROC			glGetBufferSubDataARB				= NULL;
PFNGLMAPBUFFERARBPROC					glMapBufferARB						= NULL;
PFNGLUNMAPBUFFERARBPROC					glUnmapBufferARB					= NULL;
PFNGLGETBUFFERPARAMETERIVARBPROC		glGetBufferParameterivARB			= NULL;
PFNGLGETBUFFERPOINTERVARBPROC			glGetBufferPointervARB				= NULL;

PFNGLCOMPRESSEDTEXIMAGE3DPROC			glCompressedTexImage3D				= NULL;
PFNGLCOMPRESSEDTEXIMAGE2DPROC			glCompressedTexImage2D				= NULL;
PFNGLCOMPRESSEDTEXIMAGE1DPROC			glCompressedTexImage1D				= NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC		glCompressedTexSubImage3D			= NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC		glCompressedTexSubImage2D			= NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC		glCompressedTexSubImage1D			= NULL;
PFNGLGETCOMPRESSEDTEXIMAGEPROC			glGetCompressedTexImage				= NULL;

//*** Occlusion queries ***//
PFNGLGENQUERIESARBPROC        glGenQueriesARB        = NULL;
PFNGLDELETEQUERIESARBPROC     glDeleteQueriesARB     = NULL;
PFNGLISQUERYARBPROC           glIsQueryARB           = NULL;
PFNGLBEGINQUERYARBPROC        glBeginQueryARB        = NULL;
PFNGLENDQUERYARBPROC          glEndQueryARB          = NULL;
PFNGLGETQUERYIVARBPROC        glGetQueryivARB        = NULL;
PFNGLGETQUERYOBJECTIVARBPROC  glGetQueryObjectivARB  = NULL;
PFNGLGETQUERYOBJECTUIVARBPROC glGetQueryObjectuivARB = NULL;

//*** Frame-buffer objects ***//
PFNGLISRENDERBUFFEREXTPROC                      glIsRenderbufferEXT                      = NULL;
PFNGLBINDRENDERBUFFEREXTPROC                    glBindRenderbufferEXT                    = NULL;
PFNGLDELETERENDERBUFFERSEXTPROC                 glDeleteRenderbuffersEXT                 = NULL;
PFNGLGENRENDERBUFFERSEXTPROC                    glGenRenderbuffersEXT                    = NULL;
PFNGLRENDERBUFFERSTORAGEEXTPROC                 glRenderbufferStorageEXT                 = NULL;
PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC          glGetRenderbufferParameterivEXT          = NULL;
PFNGLISFRAMEBUFFEREXTPROC                       glIsFramebufferEXT                       = NULL;
PFNGLBINDFRAMEBUFFEREXTPROC                     glBindFramebufferEXT                     = NULL;
PFNGLDELETEFRAMEBUFFERSEXTPROC                  glDeleteFramebuffersEXT                  = NULL;
PFNGLGENFRAMEBUFFERSEXTPROC                     glGenFramebuffersEXT                     = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC              glCheckFramebufferStatusEXT              = NULL;
PFNGLFRAMEBUFFERTEXTURE1DEXTPROC                glFramebufferTexture1DEXT                = NULL;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC                glFramebufferTexture2DEXT                = NULL;
PFNGLFRAMEBUFFERTEXTURE3DEXTPROC                glFramebufferTexture3DEXT                = NULL;
PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC             glFramebufferRenderbufferEXT             = NULL;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC glGetFramebufferAttachmentParameterivEXT = NULL;
PFNGLGENERATEMIPMAPEXTPROC                      glGenerateMipmapEXT                      = NULL;

using namespace System;

extern UINT OpenGL_uiVersionNumber;
extern CString OpenGL_Extensions;

bool COpenGL15::Initialize()
{
	CEventLog::BlockNew(STRING("Initializing OpenGL 1.5 core extensions"));

	CEventLog::BlockNew(STRING("OpenGL 1.2"));
	CEventLog::EventNew(STRING("GL_EXT_texture3D"));

	bool bNullFunction = FALSE;

	CStringSearch Search(OpenGL_Extensions);

	//3D textures (OpenGL 1.2)
	if ((OpenGL_uiVersionNumber >= 12) || (Search.FindString(STRING("GL_EXT_texture3D")) != -1))
	{
		bNullFunction |= LOAD_FUNCTION(PFNGLTEXIMAGE3DPROC,     glTexImage3D);
		bNullFunction |= LOAD_FUNCTION(PFNGLSAMPLECOVERAGEPROC, glSampleCoverage);
	}
	else 
	{ 
		SCFError(ErrorHardwareUnsupported); 
		return FALSE;
	}

	CEventLog::BlockClose();
	CEventLog::BlockNew(STRING("OpenGL 1.3"));
	CEventLog::EventNew(STRING("GL_ARB_multitexture"));

	//Multi-texture (OpenGL 1.3)
	if ((OpenGL_uiVersionNumber >= 13) || (Search.FindString(STRING("GL_ARB_multitexture")) != -1))
	{
		bNullFunction |= LOAD_FUNCTION(PFNGLACTIVETEXTUREPROC,       glActiveTexture);
		bNullFunction |= LOAD_FUNCTION(PFNGLCLIENTACTIVETEXTUREPROC, glClientActiveTexture);

		bNullFunction |= LOAD_FUNCTION(PFNGLMULTITEXCOORD2DPROC,  glMultiTexCoord2d);
		bNullFunction |= LOAD_FUNCTION(PFNGLMULTITEXCOORD2DVPROC, glMultiTexCoord2dv);
		bNullFunction |= LOAD_FUNCTION(PFNGLMULTITEXCOORD3DPROC,  glMultiTexCoord3d);
		bNullFunction |= LOAD_FUNCTION(PFNGLMULTITEXCOORD3DVPROC, glMultiTexCoord3dv);
		bNullFunction |= LOAD_FUNCTION(PFNGLMULTITEXCOORD4DPROC,  glMultiTexCoord4d);
		bNullFunction |= LOAD_FUNCTION(PFNGLMULTITEXCOORD4DVPROC, glMultiTexCoord4dv);
	}
	else 
	{ 
		SCFError(ErrorHardwareUnsupported); 
		return FALSE;
	}

	CEventLog::EventNew(STRING("GL_ARB_texture_compression, GL_EXT_texture_compression_s3tc"));

	//Texture compression (OpenGL 1.3)
	if ((OpenGL_uiVersionNumber >= 13) || ((Search.FindString(STRING("GL_ARB_texture_compression")) != -1) && (Search.FindString(STRING("GL_EXT_texture_compression_s3tc")) != -1)))
	{
		bNullFunction |= LOAD_FUNCTION(PFNGLCOMPRESSEDTEXIMAGE3DPROC,    glCompressedTexImage3D);
		bNullFunction |= LOAD_FUNCTION(PFNGLCOMPRESSEDTEXIMAGE2DPROC,    glCompressedTexImage2D);
		bNullFunction |= LOAD_FUNCTION(PFNGLCOMPRESSEDTEXIMAGE1DPROC,    glCompressedTexImage1D);
		bNullFunction |= LOAD_FUNCTION(PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC, glCompressedTexSubImage3D);
		bNullFunction |= LOAD_FUNCTION(PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC, glCompressedTexSubImage2D);
		bNullFunction |= LOAD_FUNCTION(PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC, glCompressedTexSubImage1D);
		bNullFunction |= LOAD_FUNCTION(PFNGLGETCOMPRESSEDTEXIMAGEPROC,   glGetCompressedTexImage);
	}
	else 
	{ 
		SCFError(ErrorHardwareUnsupported); 
		return FALSE;
	}

	CEventLog::BlockClose();
	CEventLog::BlockNew(STRING("OpenGL 1.4"));
	CEventLog::EventNew(STRING("GL_ARB_point_parameters"));

	//Buffer objects (OpenGL 1.4)
	if ((OpenGL_uiVersionNumber >= 14) || (Search.FindString(STRING("GL_ARB_point_parameters")) != -1))
	{
		bNullFunction |= LOAD_FUNCTION(PFNGLPOINTPARAMETERFARBPROC,  glPointParameterfARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLPOINTPARAMETERFVARBPROC, glPointParameterfvARB);
	}
	else 
	{ 
		SCFError(ErrorHardwareUnsupported); 
		return FALSE;
	}

	CEventLog::BlockClose();
	CEventLog::BlockNew(STRING("OpenGL 1.5"));
	CEventLog::EventNew(STRING("GL_ARB_vertex_buffer_object"));

	//Buffer objects (OpenGL 1.5)
	if ((OpenGL_uiVersionNumber >= 15) || (Search.FindString(STRING("GL_ARB_vertex_buffer_object")) != -1))
	{
		bNullFunction |= LOAD_FUNCTION(PFNGLBINDBUFFERARBPROC,           glBindBufferARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLDELETEBUFFERSARBPROC,        glDeleteBuffersARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLGENBUFFERSARBPROC,           glGenBuffersARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLISBUFFERARBPROC,             glIsBufferARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLBUFFERDATAARBPROC,           glBufferDataARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLBUFFERSUBDATAARBPROC,        glBufferSubDataARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLGETBUFFERSUBDATAARBPROC,     glGetBufferSubDataARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLMAPBUFFERARBPROC,            glMapBufferARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLUNMAPBUFFERARBPROC,          glUnmapBufferARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLGETBUFFERPARAMETERIVARBPROC, glGetBufferParameterivARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLGETBUFFERPOINTERVARBPROC,    glGetBufferPointervARB);
	}
	else 
	{ 
		SCFError(ErrorHardwareUnsupported); 
		return FALSE;
	}
	
	//Occlusion queries (OpenGL 1.5)
	if ((OpenGL_uiVersionNumber >= 15) || (Search.FindString(STRING("GL_ARB_occlusion_query")) != -1))
	{
		bNullFunction |= LOAD_FUNCTION(PFNGLGENQUERIESARBPROC,        glGenQueriesARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLDELETEQUERIESARBPROC,     glDeleteQueriesARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLISQUERYARBPROC,           glIsQueryARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLBEGINQUERYARBPROC,        glBeginQueryARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLENDQUERYARBPROC,          glEndQueryARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLGETQUERYIVARBPROC,        glGetQueryivARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLGETQUERYOBJECTIVARBPROC,  glGetQueryObjectivARB);
		bNullFunction |= LOAD_FUNCTION(PFNGLGETQUERYOBJECTUIVARBPROC, glGetQueryObjectuivARB);
	}
	else 
	{ 
		SCFError(ErrorHardwareUnsupported); 
		return FALSE;
	}

	CEventLog::BlockClose();
	CEventLog::BlockNew(STRING("Additional extensions"));

	//Frame-buffer objects (written against OpenGL 1.5)
	if (Search.FindString(STRING("GL_EXT_framebuffer_object")) != -1)
	{
		bNullFunction |= LOAD_FUNCTION(PFNGLISRENDERBUFFEREXTPROC,                      glIsRenderbufferEXT);
		bNullFunction |= LOAD_FUNCTION(PFNGLBINDRENDERBUFFEREXTPROC,                    glBindRenderbufferEXT);
		bNullFunction |= LOAD_FUNCTION(PFNGLDELETERENDERBUFFERSEXTPROC,                 glDeleteRenderbuffersEXT);
		bNullFunction |= LOAD_FUNCTION(PFNGLGENRENDERBUFFERSEXTPROC,                    glGenRenderbuffersEXT);
		bNullFunction |= LOAD_FUNCTION(PFNGLRENDERBUFFERSTORAGEEXTPROC,                 glRenderbufferStorageEXT);
		bNullFunction |= LOAD_FUNCTION(PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC,          glGetRenderbufferParameterivEXT);
		bNullFunction |= LOAD_FUNCTION(PFNGLISFRAMEBUFFEREXTPROC,                       glIsFramebufferEXT);
		bNullFunction |= LOAD_FUNCTION(PFNGLBINDFRAMEBUFFEREXTPROC,                     glBindFramebufferEXT);
		bNullFunction |= LOAD_FUNCTION(PFNGLDELETEFRAMEBUFFERSEXTPROC,                  glDeleteFramebuffersEXT);
		bNullFunction |= LOAD_FUNCTION(PFNGLGENFRAMEBUFFERSEXTPROC,                     glGenFramebuffersEXT);
		bNullFunction |= LOAD_FUNCTION(PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC,              glCheckFramebufferStatusEXT);
		bNullFunction |= LOAD_FUNCTION(PFNGLFRAMEBUFFERTEXTURE1DEXTPROC,                glFramebufferTexture1DEXT);
		bNullFunction |= LOAD_FUNCTION(PFNGLFRAMEBUFFERTEXTURE2DEXTPROC,                glFramebufferTexture2DEXT);
		bNullFunction |= LOAD_FUNCTION(PFNGLFRAMEBUFFERTEXTURE3DEXTPROC,                glFramebufferTexture3DEXT);
		bNullFunction |= LOAD_FUNCTION(PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC,             glFramebufferRenderbufferEXT);
		bNullFunction |= LOAD_FUNCTION(PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC, glGetFramebufferAttachmentParameterivEXT);
		bNullFunction |= LOAD_FUNCTION(PFNGLGENERATEMIPMAPEXTPROC,                      glGenerateMipmapEXT);
	}
	else 
	{ 
		SCFError(ErrorHardwareUnsupported); 
		return FALSE;
	}

	CEventLog::BlockClose();

	if (bNullFunction) 
	{
		SCFError(ErrorHardwareUnsupported); 
		return FALSE;
	}
	CEventLog::BlockClose();

	return TRUE;
}


