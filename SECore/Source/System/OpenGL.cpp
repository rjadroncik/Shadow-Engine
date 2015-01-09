#include "OpenGL.h"

#include "../SECore.h"

using namespace System;

SCF::ENUM OpenGL_eVendor = VENDOR_NOT_SUPPORTED;

CString OpenGL_Extensions;
CString OpenGL_Vendor;
CString OpenGL_Renderer;
CString OpenGL_Version;

SCF::UINT OpenGL_uiVersionNumber = 0;

inline CString   COpenGL::Extensions() { return OpenGL_Extensions; }
inline CString   COpenGL::Vendor()     { return OpenGL_Vendor; }
inline SCF::ENUM COpenGL::VendorID()   { return OpenGL_eVendor; }
inline CString   COpenGL::Renderer()   { return OpenGL_Renderer; }

inline CString   COpenGL::Version()       { return OpenGL_Version; }
inline SCF::UINT COpenGL::VersionNumber() { return OpenGL_uiVersionNumber; }

void COpenGL::InitializeOpenGLState()
{
	CEventLog::EventNew(STRING("Initializing OpenGL state"));

	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 0);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); 

	glShadeModel(GL_SMOOTH); 
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0);  
	glClearDepth(1.0);
	glClearStencil(128);

 	glDisable(GL_NORMALIZE);
	glDisable(GL_BLEND);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_SCISSOR_TEST);
	//Never use standard OpenGL lights
	glDisable(GL_LIGHT0);	
	glDisable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnableClientState(GL_VERTEX_ARRAY);

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);	

	glStencilMask(0x00);
	glDepthMask(1);
	glColorMask(1, 1, 1, 1);

	glPolygonOffset(3.5, 3.0);
}

void COpenGL::Initialize()
{
	CString Temp;
	CEventLog::EventNew(STRING("Initializing OpenGL"));

	//Get strings from OpenGL & convert them into [LPTSTR] strings
	OpenGL_Renderer   = (char*)glGetString(GL_RENDERER);
	OpenGL_Extensions = (char*)glGetString(GL_EXTENSIONS);
	OpenGL_Vendor     = (char*)glGetString(GL_VENDOR);
	OpenGL_Version    = (char*)glGetString(GL_VERSION);

	//This whole little dance is just to replace spaces with carriage return - line feed pairs
	{
		SCF::UINT uiSpaceCount = 0;
		for (SCF::UINT i = 0; i < OpenGL_Extensions.Length(); i++)
		{
			if (OpenGL_Extensions[i] == ' ') { uiSpaceCount++; }
		}

		Temp.Resize(OpenGL_Extensions.Length() + uiSpaceCount);

		SCF::UINT j = 0;
		for (SCF::UINT i = 0; i <= OpenGL_Extensions.Length(); i++)
		{
			if (OpenGL_Extensions[i] == ' ') { Temp.AtPut(j, '\r'); j++; Temp.AtPut(j, '\n'); j++; }
			else                             { Temp.AtPut(j, OpenGL_Extensions[i]); j++; }
		}

		OpenGL_Extensions = Temp;
	}

	Temp = CStringRange(OpenGL_Version, 0, 3);
	OpenGL_uiVersionNumber = (SCF::UINT)(CFloat(Temp).Value() * 10 + 0.5f);

	CStringSearch VendorSearch(OpenGL_Vendor);
	if (VendorSearch.FindStringIC(STRING("ATI"))    != -1) { OpenGL_eVendor = VENDOR_ATI; }
	if (VendorSearch.FindStringIC(STRING("NVIDIA")) != -1) { OpenGL_eVendor = VENDOR_NVIDIA; }

	CEventLog::EventNew(STRING("Renderer: ") + OpenGL_Renderer);
	CEventLog::EventNew(STRING("Vendor:   ") + OpenGL_Vendor);
	CEventLog::EventNew(STRING("Version:  ") + OpenGL_Version);

	InitializeOpenGLState();
}