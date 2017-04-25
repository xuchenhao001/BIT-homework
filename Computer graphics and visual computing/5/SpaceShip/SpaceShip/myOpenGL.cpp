// myOpenGL.cpp: implementation of the CmyOpenGL class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SpaceShip.h"
#include "myOpenGL.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CmyOpenGL::CmyOpenGL()
{
	m_bWired = false;
}

CmyOpenGL::~CmyOpenGL()
{

}

void CmyOpenGL::PostInit()
{
	glClearColor(0.5, 0.5, 0.5, 1);
	//	glClearColor(1,1,1,1);
	m_pCamere->SetCamera(CglVector3(5, 10, 10), CglVector3(0, 0, 0), true);
	m_pCamere->SaveCamera();
	m_pControl->SetSpeed(0.1, 0.2);
	//////////////////////////////////////////////////////////////////////////
	m_particle.Init(1000);
}

void CmyOpenGL::InDraw()
{
	if (m_bWired)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	m_pShape->DrawCoord(100);
	DrawModel();
}

bool CmyOpenGL::OnKey(unsigned int nChar, bool bDown)
{
	if (bDown)
		switch (nChar)
		{
		case VK_F1:
			m_bWired = !m_bWired;
			break;
		}
	return false;
}
void CmyOpenGL::DrawModel()
{
	m_particle.UpdatePos();
	m_particle.Draw();
}
