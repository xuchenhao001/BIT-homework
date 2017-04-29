#include "stdafx.h"
#include <GL/glew.h>
#include "CmyOpenGL.h"
#pragma comment(lib,"glew32.lib ")

//====================================
//ȫ�ֱ������岿��(����������ʵ��޸�)
//====================================
int width = 1280, height = 720;	//���ǻ��ƴ���Ĭ�ϴ�СΪ1280x720
const int STAR_NUM = 10000;		//������������Ĭ��Ϊ10000��
const int TAIL_LEN = 121;		//Сβ�ͳ���+1��Ĭ��Ϊ120��
const int SMOOTH = 100;			//�ӵ����ƽ���̶ȣ���ֵԽ��Խƽ����Ĭ��Ϊ100
float mspeed = 0.01, rspeed = 0.05;	//�ƶ��ٶȺ���ת�ٶ�

CglVector3 stars[STAR_NUM];

//=========================
//ȫ�ֱ������岿��(���ɸ���)
//=========================

//�ɰ���Сβ��
cycleQueue<CglVector3> tail(TAIL_LEN);

//==============
//���ƺ���ʵ�ֲ���
//==============

//��ʼ�����Ǻ���
void initStar() {
	for (int i = 0; i < STAR_NUM; i++) {
		stars[i].x = rand() / double(RAND_MAX) * width * 32 - width * 16;
		stars[i].y = rand() / double(RAND_MAX) * height * 32 - height * 16;
		stars[i].z = rand() / double(RAND_MAX) * 32000 - 16000;
	}
}

//���û���
void SetRC() {

	//������Բ��(����ƴ�С�任��ͻ����ע�ӵ���)
	//glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glShadeModel(GL_FLAT);
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_BACK, GL_LINE);

	//�������������ӵ�Զ���任��С
	glewInit();
	float Quadratic[] = { 0,0.1,0 };
	glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, Quadratic);
	glPointParameterf(GL_POINT_SIZE_MIN, 1);//������С��
	glPointParameterf(GL_POINT_SIZE_MAX, 100);//��������
}

//������
void drawStar() {
	glPointSize(20);
	glBegin(GL_POINTS);
	for (int i = 0; i < STAR_NUM; i++) {
		glVertex3f(stars[i].x, stars[i].y, stars[i].z);
	}
	glEnd();
}

//��β��
void drawTail() {
	glPushMatrix();
	tail.reset();
	for (int i = 0; i < tail.size() - 1; i++) {
		float color;
		glLineWidth(1);
		//��β�ͳ��ȴﵽ120��ʱ�򣬿������20�׵�β�ͽ�����ʧ
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
//COpenGL����ʵ�ֲ���
//==================

CmyOpenGL::CmyOpenGL() {
	m_bWired = false;
}

CmyOpenGL::~CmyOpenGL() {

}

//�����ʼ��ʱ����
void CmyOpenGL::PostInit() {
	m_pControl->SetSpeed(mspeed, rspeed);

	glClearColor(0, 0, 0, 1);
	m_pCamere->SetCamera(CglVector3(100, 200, 200), CglVector3(0, 0, 0), true);
	m_pCamere->SaveCamera();

	//���û���
	SetRC();
	//��ʼ������
	initStar();
}

//�ܻ��ƺ���
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
	//������
	drawStar();
	//��β��
	drawTail();
}
