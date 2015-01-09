#include "Carret.h"

using namespace Resources;

//if (CDirectInput::KeyPressed(DIK_ESCAPE))
//{														   
//	Simulation::CSimulation::Pause();
//	CSystem::Shutdown();
//}

CCarret::CCarret()
{
	m_Size[0] = 2;
	m_Size[1] = 14;

	m_uiTimer = 0;
	m_uiUpTime = 30;
}

CCarret::~CCarret()
{

}

void __fastcall CCarret::Update()
{
	if (m_uiTimer == (2 * m_uiUpTime)) { m_uiTimer = 0; }
	m_uiTimer++;
}

void __fastcall CCarret::RenderAt(_IN Float2& rPosition) const
{
	if (m_uiTimer > m_uiUpTime) { return; }

	glBindTexture(GL_TEXTURE_2D, 0);
	glColor4f(1, 1, 1, 0.8f);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex2f(rPosition[0], rPosition[1] + m_Size[1]);
		glTexCoord2f(1, 0);
		glVertex2f(rPosition[0] + m_Size[0], rPosition[1] + m_Size[1]);
		glTexCoord2f(1, 1);
		glVertex2f(rPosition[0] + m_Size[0], rPosition[1]);
		glTexCoord2f(0, 1);
		glVertex2f(rPosition[0], rPosition[1]);
	}
	glEnd();
}