#include "stdafx.h"
#include "CmyOpenGL.h"


CmyOpenGL::CmyOpenGL()
{
}


CmyOpenGL::~CmyOpenGL()
{
}


void CmyOpenGL::PostInit()
{
	//�����ʼ��ʱ����
	m_pCamere->SetCamera(CglVector3(0, 0, 10), CglVector3(0, 0, 0), true);
	m_pCamere->SaveCamera();
}


void CmyOpenGL::InDraw()
{
	//ÿһ֡��ͼ����
	glutSolidTeapot(5);
}


bool CmyOpenGL::OnKey(unsigned char nChar, bool bDown)
{
	return false;
}


void CmyOpenGL::DrawModel()
{
}
