#include "RendererDebug.h"

using namespace Rendering;

CRendererDebug::Scene::SDisplay CRendererDebug::Scene::s_Display = { FALSE, FALSE, FALSE, FALSE, FALSE, FALSE };

void CRendererDebug::Scene::RenderDataVisualisation(_IN CScene& rScene)
{
	glPushAttrib(GL_ENABLE_BIT);
	{
		//Prepare renderer
		glLoadIdentity();
		//CMaterialUsage::ApplyDefault();

		if (rScene.CameraCurrent()->TargetObject())
		{
			if (rScene.CameraCurrent()->CheckVisibilityByBounds(*rScene.CameraCurrent()->TargetObject()))
			{
				glPushMatrix();
				{
					rScene.CameraCurrent()->TargetObject()->PosittionApply();
					rScene.CameraCurrent()->TargetObject()->BoundsRender(1, 1, 1);
				}
				glPopMatrix();
			}
		}

		if (CRendererDebug::Scene::ShadowCones())    { CRendererDebug::Scene::RenderShadowCones(rScene); }
		if (CRendererDebug::Scene::Lights())         { CRendererDebug::Scene::RenderLights(rScene); }
		if (CRendererDebug::Scene::Cameras())        { CRendererDebug::Scene::RenderCameras(rScene); }
		if (CRendererDebug::Scene::Bounds())         { CRendererDebug::Scene::BoundsRender(rScene); }
		if (CRendererDebug::Scene::TBNs())           { CRendererDebug::Scene::RenderTBNs(rScene); }
	}
	glPopAttrib();
}

void CRendererDebug::Scene::BoundsRender(_IN CScene& rScene)
{
	for (UINT i = 0; i < rScene.ModelCount(); i++)
	{
		if (rScene.CameraCurrent()->CheckVisibilityByBounds(rScene.Model(i)))
		{
			glPushMatrix();
			{
				rScene.Model(i).PosittionApply();
				rScene.Model(i).BoundsRender(1, 1, 1);
			}
			glPopMatrix();
		}
	}

	for (UINT i = 0; i < rScene.ParticleSystemCount(); i++)
	{
		if (rScene.CameraCurrent()->CheckVisibilityByBounds(rScene.ParticleSystem(i)))
		{
			glPushMatrix();
			{
				rScene.ParticleSystem(i).PosittionApply();
				rScene.ParticleSystem(i).BoundsRender(1, 1, 1);
			}
			glPopMatrix();
		}
	}
}

void CRendererDebug::Scene::RenderTBNs(_IN CScene& rScene)
{
	for (UINT i = 0; i < rScene.ModelCount(); i++)
	{
		_PENDING; //rScene.Model(i).RenderTBNs();
		_PENDING; //rScene.Model(i).RenderSeam();
	}
}

void CRendererDebug::Scene::RenderCameras(_IN CScene& rScene)
{
	for (UINT i = 0; i < rScene.CameraCount(); i++)
	{
		if (&rScene.Camera(i) == rScene.CameraCurrent()) { continue; }

		rScene.Camera(i).Render(); 

		if (CRendererDebug::Scene::Bounds())
		{
			//Apply transformations
			glPushMatrix();
			{
				rScene.Camera(i).PosittionApply();
				rScene.Camera(i).BoundsRender(1, 1, 1);
			}
			glPopMatrix();
		}
	}
}

void CRendererDebug::Scene::RenderWireFrame(_IN CScene& rScene, _IN bool bFillDepthBuffer)
{
	SCF_UNREFERENCED_PARAMETER(rScene);
	SCF_UNREFERENCED_PARAMETER(bFillDepthBuffer);
	_PENDING;

	/*	if (bFillDepthBuffer)
	{
	//Prepare Renderer
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDepthMask(1);
	glStencilMask(0x00);
	glColorMask(0, 0, 0, 0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);	
	glDisable(GL_LIGHTING);	
	glShadeModel(GL_FLAT); 
	glDisable(GL_STENCIL_TEST);

	glDepthFunc(GL_LESS);

	glDisable(GL_FRAGMENT_PROGRAM_ARB);
	glDisable(GL_VERTEX_PROGRAM_ARB);

	CMaterial::ApplyDefault();
	glColor4f(1, 1, 1, 1);

	for (UINT i = 0; i < rScene.ModelCount(); i++)
	{
	if (rScene.CameraCurrent()->CheckVisibilityByBounds(rScene.Model(i)))
	{
	glPushMatrix();
	{
	rScene.Model(i).PosittionApply();
	rScene.Model(i).RotationApply();
	rScene.Model(i).Render();
	}
	glPopMatrix();
	}
	}
	}

	//Prepare Renderer
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDepthMask(0);
	glStencilMask(0x00);
	glColorMask(1, 1, 1, 1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);	
	glDisable(GL_LIGHTING);	
	glShadeModel(GL_FLAT); 
	glDisable(GL_STENCIL_TEST);

	glCullFace(GL_BACK);
	glDepthFunc(GL_LEQUAL);

	glDisable(GL_FRAGMENT_PROGRAM_ARB);
	glDisable(GL_VERTEX_PROGRAM_ARB);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnable(GL_POLYGON_OFFSET_LINE);
	{
	glPolygonOffset(-1.0f, 0);

	CMaterial::ApplyDefault();
	glColor4f(1, 1, 1, 1);

	for (UINT i = 0; i < rScene.ModelCount(); i++)
	{
	if (rScene.CameraCurrent()->CheckVisibilityByBounds(rScene.Model(i)))
	{
	glPushMatrix();
	{
	rScene.Model(i).PosittionApply();
	rScene.Model(i).RotationApply();
	rScene.Model(i).Render();
	}
	glPopMatrix();
	}
	}
	}
	glDisable(GL_POLYGON_OFFSET_LINE);	*/
}

void CRendererDebug::Scene::RenderShadowCones(_IN CScene& rScene)
{
	glPushAttrib(GL_ENABLE_BIT);
	{
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glColor4f(1, 1, 1, 0.1f);

		Float3 TempVector;
		Float3 ConeDirection;
		Float3 FinalVector;
		Float3 NormalVector;

		//Go through all lights
		for (UINT i = 0; i < rScene.LightCount(); i++)
		{
			//Skip light that are currently turned off
			if (!rScene.Light(i).Enabled()) { continue; }

			if (rScene.Light(i).Shadows()) 
			{ 
				for (UINT j = 0; j < rScene.ModelCount(); j++)
				{
//					if (!(rScene.CameraCurrent()->ClassifyShadowRendering(rScene.Light(i), rScene.Model(j)) & SVO_RENDER_BACK_CAP)) { continue; }
					if (MeasureDistance3((Float3&)rScene.Model(j).Position(), (Float3&)rScene.Light(i).Position()) < rScene.Model(j).BoundsRadius()) { continue; }

					//Compute direction of the light to object-sphere cone
					_PENDING;
					CopyVector3     (FinalVector, (Float3&)rScene.Model(j).Position());
					MakeVector3     (TempVector,  (Float3&)rScene.Light(i).Position(), FinalVector);
					NormalizeVector3(ConeDirection, TempVector);

					//Use sinus formula to compute
					float fDistance = MeasureDistance3((Float3&)rScene.Light(i).Position(), FinalVector);

					if (fDistance < (rScene.Model(j).BoundsRadius() * 1.1)) { continue; }

					float fHalfAngle = SCFRadToDeg * asinf(rScene.Model(j).BoundsRadius() / fDistance);

					SetVector3      (FinalVector, 0, 1, 0);
					CrossVectors3   (NormalVector, FinalVector, ConeDirection);
					NormalizeVector3(NormalVector, NormalVector);

					Float3 BinormalVector;

					CrossVectors3   (BinormalVector, NormalVector, ConeDirection);
					NormalizeVector3(BinormalVector, BinormalVector);

					AxisRotateVector3N(TempVector, TempVector, NormalVector, fHalfAngle);

					glBegin(GL_TRIANGLES);
					{
						for (UINT k = 0; k < 36; k++)
						{
							glVertex3fv(&(rScene.Light(i).Position()[0]));

							AxisRotateVector3N(FinalVector, TempVector, ConeDirection, 10.0f * k);
							AddVectors3(FinalVector, FinalVector, (Float3&)rScene.Light(i).Position());
							glVertex3fv(FinalVector);

							AxisRotateVector3N(FinalVector, TempVector, ConeDirection, 10.0f * (k + 1));
							AddVectors3(FinalVector, FinalVector, (Float3&)rScene.Light(i).Position());
							glVertex3fv(FinalVector);
						}
					}
					glEnd();
				}
			}
		}
	}
	glPopAttrib();
}

void CRendererDebug::Scene::RenderLights(_IN CScene& rScene)
{
	glColor3d(1, 1, 0);

	for (UINT i = 0; i < rScene.LightCount(); i++)
	{
		//Skip light that are currently turned off
		if (!rScene.Light(i).Enabled()) { continue; }

		glPushMatrix();
		{
			rScene.Light(i).PosittionApply();

			_PENDING; //CHelper::RenderSphereWF(0, 0, 0, 0, 0, 0, 50); 

			if (CRendererDebug::Scene::Bounds()) { rScene.Light(i).BoundsRender(1, 1, 0); }
		}
		glPopMatrix();
	}
}

/*
void Resources::CMesh::RenderTBNs() const
{
	if (!Attribute(1) || !Attribute(2)) { return; }

	//Fetch texture coordinates
	Float3* pTangentCoord  = (Float3*)Attribute(1)->vpData;
	Float3* pBinormalCoord = (Float3*)Attribute(2)->vpData;

	Float3 Normal = { 0, 0, 0 };
	Float3 Temp   = { 0, 0, 0 };

	glPushAttrib(GL_LIGHTING_BIT | GL_ENABLE_BIT);
	{
		CMaterial::ApplyDefault();

		glBegin(GL_LINES);
		{
			for (UINT i = 0; i < m_usVertexCount; i++)
			{
				ScaleVector3(Temp, pTangentCoord[i], 20);
				AddVectors3 (Temp, Temp, *(Float3*)&m_pVertices[i]);

				glColor3f(1, 0, 0);
				glVertex3fv(&m_pVertices[i][0]);
				glVertex3fv(&Temp[0]);

				ScaleVector3(Temp, pBinormalCoord[i], 20);
				AddVectors3 (Temp, Temp, *(Float3*)&m_pVertices[i]);

				glColor3f(0, 1, 0);
				glVertex3fv(&m_pVertices[i][0]);
				glVertex3fv(&Temp[0]);

				CrossVectors3(Normal, pTangentCoord[i], pBinormalCoord[i]);

				ScaleVector3(Temp, Normal, 20);
				AddVectors3 (Temp, Temp, *(Float3*)&m_pVertices[i]);

				glColor3f(0, 0, 1);
				glVertex3fv(&m_pVertices[i][0]);
				glVertex3fv(&Temp[0]);
			}
		}
		glEnd();
	}
	glPopAttrib();
}

void Resources::CMesh::RenderSeam() const
{
	glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_LIGHTING_BIT | GL_ENABLE_BIT);
	{
		glDisable(GL_VERTEX_PROGRAM_ARB);
		glDisable(GL_FRAGMENT_PROGRAM_ARB);

		glDisable(GL_STENCIL_TEST);
		glDisable(GL_DEPTH_TEST);

		//glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LEQUAL);

		CMaterial::ApplyDefault();

		glPointSize(3);
		glColor3f(1, 1, 1);

		glBegin(GL_POINTS);
		{
			for (UINT i = 0; i < m_usSeamVertexListCount; i++)
			{
				for (UINT j = 0; j < m_pSeamVertices[i].uiCount; j++)
				{
					glVertex3fv(m_pVertices[m_pSeamVertices[i].uipTriangles[j]]);
				}
			}
		}
		glEnd();
	}
	glPopAttrib();
}*/
