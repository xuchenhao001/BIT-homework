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
	//程序初始化时调用
	m_pCamere->SetCamera(CglVector3(0, 0, 10), CglVector3(0, 0, 0), true);
	m_pCamere->SaveCamera();
}


void CmyOpenGL::InDraw()
{
	//每一帧绘图内容
	glutSolidTeapot(5);
}


bool CmyOpenGL::OnKey(unsigned char nChar, bool bDown)
{
	return false;
}


void CmyOpenGL::DrawModel()
{
}
