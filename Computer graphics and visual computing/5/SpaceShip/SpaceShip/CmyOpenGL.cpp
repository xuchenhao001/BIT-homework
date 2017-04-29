#include "stdafx.h"
#include <GL/glew.h>
#include "CmyOpenGL.h"
#pragma comment(lib,"glew32.lib ")

//====================================
//全局变量定义部分(可依据情况适当修改)
//====================================
int width = 1280, height = 720;	//星星绘制窗口默认大小为1280x720
const int STAR_NUM = 10000;		//星星总数量，默认为10000个
const int TAIL_LEN = 121;		//小尾巴长度+1，默认为120米
const int SMOOTH = 100;			//视点过渡平滑程度，数值越大越平滑，默认为100
float mspeed = 0.01, rspeed = 0.05;	//移动速度和旋转速度

CglVector3 stars[STAR_NUM];

//=========================
//全局变量定义部分(不可更改)
//=========================

//可爱的小尾巴
cycleQueue<CglVector3> tail(TAIL_LEN);

//==============
//绘制函数实现部分
//==============

//初始化星星函数
void initStar() {
	for (int i = 0; i < STAR_NUM; i++) {
		stars[i].x = rand() / double(RAND_MAX) * width * 32 - width * 16;
		stars[i].y = rand() / double(RAND_MAX) * height * 32 - height * 16;
		stars[i].z = rand() / double(RAND_MAX) * 32000 - 16000;
	}
}

//设置环境
void SetRC() {

	//星星是圆的(与控制大小变换冲突所以注视掉了)
	//glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glShadeModel(GL_FLAT);
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_BACK, GL_LINE);

	//控制星星随着视点远近变换大小
	glewInit();
	float Quadratic[] = { 0,0.1,0 };
	glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, Quadratic);
	glPointParameterf(GL_POINT_SIZE_MIN, 1);//设置最小点
	glPointParameterf(GL_POINT_SIZE_MAX, 100);//设置最大点
}

//画星星
void drawStar() {
	glPointSize(20);
	glBegin(GL_POINTS);
	for (int i = 0; i < STAR_NUM; i++) {
		glVertex3f(stars[i].x, stars[i].y, stars[i].z);
	}
	glEnd();
}

//画尾巴
void drawTail() {
	glPushMatrix();
	tail.reset();
	for (int i = 0; i < tail.size() - 1; i++) {
		float color;
		glLineWidth(1);
		//当尾巴长度达到120的时候，控制最后20米的尾巴渐渐消失
		if (tail.size() == 120 && i < 20) {
			color = i / 20.0;
		} else {
			color = 1.0;
		}
		glColor3f(color, color, color);
		glBegin(GL_LINES);
		glVertex3f(tail.now().x, tail.now().y, tail.now().z);
		tail.next();
		glVertex3f(tail.now().x, tail.now().y, tail.now().z);
		glEnd();
	}
	glPopMatrix();
}



//==================
//COpenGL重载实现部分
//==================

CmyOpenGL::CmyOpenGL() {
	m_bWired = false;
}

CmyOpenGL::~CmyOpenGL() {

}

//程序初始化时调用
void CmyOpenGL::PostInit() {
	m_pControl->SetSpeed(mspeed, rspeed);

	glClearColor(0, 0, 0, 1);
	m_pCamere->SetCamera(CglVector3(100, 200, 200), CglVector3(0, 0, 0), true);
	m_pCamere->SaveCamera();

	//设置环境
	SetRC();
	//初始化星星
	initStar();
}

//总绘制函数
void CmyOpenGL::InDraw() {
	m_pShape->DrawCoord(1000);
	DrawModel();

	float t = 1000;
	CString str;
	glColor3f(1, 0, 0);
	str.Format("%d", m_bWired);
	m_pFont->Font2D(str.GetBuffer(0), -0.9, 0.9, 7);
}

bool CmyOpenGL::OnKey(unsigned int nChar, bool bDown) {
	if (nChar == 'w' || nChar == 's' || nChar == 'a' || nChar == 'd' || nChar == 'q' || nChar == 'e'||
		nChar == 'W' || nChar == 'S' || nChar == 'A' || nChar == 'D' || nChar == 'Q' || nChar == 'E') {
		if (tail.isFull()) {
			tail.pop();
		}
		tail.push(m_pCamere->m_pos);
	}
	if (bDown)
		switch (nChar) {
		case VK_F1:
			m_bWired = !m_bWired;
			break;
		}
	return false;
}

void CmyOpenGL::DrawModel() {
	glColor3f(1.0, 1.0, 1.0);
	//SetView();
	//画星星
	drawStar();
	//画尾巴
	drawTail();
}
