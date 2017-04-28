#include "stdafx.h"
#include "CmyOpenGL.h"

CmyOpenGL::CmyOpenGL() {
	m_bWired = false;
}

CmyOpenGL::~CmyOpenGL() {

}

void CmyOpenGL::PostInit() {
	//程序初始化时调用
	/*m_pCamere->SetCamera(CglVector3(0, 0, 10), CglVector3(0, 0, 0), true);
	m_pCamere->SaveCamera();*/

	//glClearColor(0.5,0.5,0.5,1);
	m_pControl->m_fKeyStep = 1;
	glClearColor(1, 1, 1, 1);
	m_pCamere->SetCamera(CglVector3(5, 10, 10), CglVector3(0, 0, 0), true);
	m_pCamere->SaveCamera();
	m_pControl->SetSpeed(0.1, 0.5);
}

void CmyOpenGL::InDraw() {
	if (m_bWired)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	m_pShape->DrawCoord(1000);
	DrawModel();
	float t = 1000;
	CString str;
	glColor3f(1, 0, 0);
	str.Format("%d", m_bWired);
	m_pFont->Font2D(str.GetBuffer(0), -0.9, 0.9, 7);
}

bool CmyOpenGL::OnKey(unsigned char nChar, bool bDown) {
	if (bDown)
		switch (nChar) {
		case VK_F1:
			m_bWired = !m_bWired;
			break;
		}
	return false;
}

double seta = 0;
void CmyOpenGL::DrawModel() {
	seta += 0.01;
	glPushAttrib(GL_CURRENT_BIT);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT1);
	glColor3f(0, 0, 0);
	glPushMatrix();

	//	glRotated(seta,1,0,0);
	//	glTranslated(0,0,10);
	glutSolidTeapot(2);
	glPopMatrix();
	
	glPopAttrib();
}
