#pragma once

#include "OpenGL.h"

//*** OpenGL core features ***//

extern PFNGLTEXIMAGE3DPROC						glTexImage3D;
extern PFNGLSAMPLECOVERAGEPROC					glSampleCoverage;

//*** Multi-texturing ***//
extern PFNGLMULTITEXCOORD2DPROC					glMultiTexCoord2d;
extern PFNGLMULTITEXCOORD2DVPROC				glMultiTexCoord2dv;
extern PFNGLMULTITEXCOORD3DPROC					glMultiTexCoord3d;
extern PFNGLMULTITEXCOORD3DVPROC				glMultiTexCoord3dv;
extern PFNGLMULTITEXCOORD4DPROC					glMultiTexCoord4d;
extern PFNGLMULTITEXCOORD4DVPROC				glMultiTexCoord4dv;

extern PFNGLACTIVETEXTUREPROC					glActiveTexture;
extern PFNGLCLIENTACTIVETEXTUREPROC				glClientActiveTexture;

//*** Point parameters ***//
extern PFNGLPOINTPARAMETERFARBPROC              glPointParameterfARB;
extern PFNGLPOINTPARAMETERFVARBPROC             glPointParameterfvARB;

//*** Vertex buffer objects ***//
extern PFNGLBINDBUFFERARBPROC					glBindBufferARB;
extern PFNGLDELETEBUFFERSARBPROC				glDeleteBuffersARB;
extern PFNGLGENBUFFERSARBPROC					glGenBuffersARB;
extern PFNGLISBUFFERARBPROC						glIsBufferARB;
extern PFNGLBUFFERDATAARBPROC					glBufferDataARB;
extern PFNGLBUFFERSUBDATAARBPROC				glBufferSubDataARB;
extern PFNGLGETBUFFERSUBDATAARBPROC				glGetBufferSubDataARB;
extern PFNGLMAPBUFFERARBPROC					glMapBufferARB;
extern PFNGLUNMAPBUFFERARBPROC					glUnmapBufferARB;
extern PFNGLGETBUFFERPARAMETERIVARBPROC			glGetBufferParameterivARB;
extern PFNGLGETBUFFERPOINTERVARBPROC			glGetBufferPointervARB;

//*** Texture compression ***//
extern PFNGLCOMPRESSEDTEXIMAGE3DPROC			glCompressedTexImage3D;
extern PFNGLCOMPRESSEDTEXIMAGE2DPROC			glCompressedTexImage2D;
extern PFNGLCOMPRESSEDTEXIMAGE1DPROC			glCompressedTexImage1D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC			glCompressedTexSubImage3D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC			glCompressedTexSubImage2D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC			glCompressedTexSubImage1D;
extern PFNGLGETCOMPRESSEDTEXIMAGEPROC			glGetCompressedTexImage;

//*** Occlusion queries ***//
extern PFNGLGENQUERIESARBPROC        glGenQueriesARB;
extern PFNGLDELETEQUERIESARBPROC     glDeleteQueriesARB;
extern PFNGLISQUERYARBPROC           glIsQueryARB;
extern PFNGLBEGINQUERYARBPROC        glBeginQueryARB;
extern PFNGLENDQUERYARBPROC          glEndQueryARB;
extern PFNGLGETQUERYIVARBPROC        glGetQueryivARB;
extern PFNGLGETQUERYOBJECTIVARBPROC  glGetQueryObjectivARB;
extern PFNGLGETQUERYOBJECTUIVARBPROC glGetQueryObjectuivARB;

//*** Frame-buffer objects ***//
extern PFNGLISRENDERBUFFEREXTPROC                      glIsRenderbufferEXT;
extern PFNGLBINDRENDERBUFFEREXTPROC                    glBindRenderbufferEXT;
extern PFNGLDELETERENDERBUFFERSEXTPROC                 glDeleteRenderbuffersEXT;
extern PFNGLGENRENDERBUFFERSEXTPROC                    glGenRenderbuffersEXT;
extern PFNGLRENDERBUFFERSTORAGEEXTPROC                 glRenderbufferStorageEXT;
extern PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC          glGetRenderbufferParameterivEXT;
extern PFNGLISFRAMEBUFFEREXTPROC                       glIsFramebufferEXT;
extern PFNGLBINDFRAMEBUFFEREXTPROC                     glBindFramebufferEXT;
extern PFNGLDELETEFRAMEBUFFERSEXTPROC                  glDeleteFramebuffersEXT;
extern PFNGLGENFRAMEBUFFERSEXTPROC                     glGenFramebuffersEXT;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC              glCheckFramebufferStatusEXT;
extern PFNGLFRAMEBUFFERTEXTURE1DEXTPROC                glFramebufferTexture1DEXT;
extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC                glFramebufferTexture2DEXT;
extern PFNGLFRAMEBUFFERTEXTURE3DEXTPROC                glFramebufferTexture3DEXT;
extern PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC             glFramebufferRenderbufferEXT;
extern PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC glGetFramebufferAttachmentParameterivEXT;
extern PFNGLGENERATEMIPMAPEXTPROC                      glGenerateMipmapEXT;

namespace System
{
	class SECORE_API COpenGL15 : public COpenGL
	{
	public:
		static bool Initialize();

	private:
		COpenGL15();
		~COpenGL15();
	};
};
