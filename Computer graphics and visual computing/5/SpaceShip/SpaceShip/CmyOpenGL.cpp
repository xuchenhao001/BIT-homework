#include "stdafx.h"
#include <GL/glew.h>
#include "CmyOpenGL.h"
#pragma comment(lib,"glew32.lib ")

//====================================
//ȫ�ֱ������岿��(����������ʵ��޸�)
//====================================
int width = 1280, height = 720;		//���ǻ��ƴ���Ĭ�ϴ�СΪ1280x720
const int STAR_NUM = 10000;			//������������Ĭ��Ϊ10000��
const int TAIL_LEN = 121;			//Сβ�ͳ���+1��Ĭ��Ϊ120��
const int SMOOTH = 100;				//�ӵ����ƽ���̶ȣ���ֵԽ��Խƽ����Ĭ��Ϊ100
float mspeed = 0.01, rspeed = 0.05;	//�ƶ��ٶȺ���ת�ٶ�
CglVector3 startAt(100, 200, 200);	//�趨�ӵ���ʼλ��
float size = 3;						//�ɻ���С
int mode = 0;						//��������ʱ���ӵ�״̬��0�����ɱ任 1�����沢���Ʒɻ�

//=========================
//ȫ�ֱ������岿��(���ɸ���)
//=========================

CglVector3 stars[STAR_NUM];
//�ɰ���Сβ��
cycleQueue<CglVector3> tail(TAIL_LEN);

//==============
//���ƺ���ʵ�ֲ���
//==============

//�������Ǿ��뺯��
float starDis(CglVector3 a, CglVector3 b) {
	return (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) + (a.z - b.z)*(a.z - b.z);;
}

//�����ǽ�������
void sortStars() {
	stars[0] = startAt;
	CglVector3 tmp_star;
	float tmp_dis = FLT_MAX;
	int tmp_j;
	for (int i = 0; i < STAR_NUM - 1; i++) {
		tmp_dis = FLT_MAX;
		for (int j = i + 1; j < STAR_NUM; j++) {
			float dis = starDis(stars[i], stars[j]);
			if (dis < tmp_dis) {
				tmp_dis = dis;
				tmp_j = j;
			}
		}
		tmp_star = stars[tmp_j];
		stars[tmp_j] = stars[i + 1];
		stars[i + 1] = tmp_star;
	}
}

//��ʼ�����Ǻ���
void initStar() {
	for (int i = 0; i < STAR_NUM; i++) {
		stars[i].x = rand() / double(RAND_MAX) * width * 32 - width * 16;
		stars[i].y = rand() / double(RAND_MAX) * height * 32 - height * 16;
		stars[i].z = rand() / double(RAND_MAX) * 32000 - 16000;
	}
	
	//sortStars();
}

//���û���
void SetRC() {

	//������Բ��(����ƴ�С�任��ͻ����ע�ӵ���)
	//glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glShadeModel(GL_FLAT);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_BACK, GL_LINE);

	srand((unsigned)time(NULL));

	//�������������ӵ�Զ���任��С
	glewInit();
	float Quadratic[] = { 0,0.1,0 };
	glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, Quadratic);
	glPointParameterf(GL_POINT_SIZE_MIN, 1);//������С��
	glPointParameterf(GL_POINT_SIZE_MAX, 100);//��������
}

//������
void drawStar() {
	glColor3f(1.0, 1.0, 1.0);
	glPointSize(3);
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

//��һ������С�ɻ�
void drawPlane(CglCamera* m_pCamere) {
	//�ɻ�ǰ�󳤶Ⱥ����ҿ�ȵı���
	double ship_prop = 10;

	glPushMatrix();
	//�����ɻ����ӵ�ǰ�����
	CglVector3 fore_aft_dis;
	fore_aft_dis.Set(&m_pCamere->m_viewMatrixInverse[8]);
	fore_aft_dis = fore_aft_dis * 8;
	//�����ɻ����ӵ����¾���
	CglVector3 up_down_dis;
	up_down_dis.Set(&m_pCamere->m_viewMatrixInverse[4]);
	up_down_dis = up_down_dis * 5;
	//���յ�������
	CglVector3 plane_pos;
	plane_pos = m_pCamere->m_pos - fore_aft_dis - up_down_dis;
	//�����ɻ�λ������
	glTranslatef(plane_pos.x, plane_pos.y, plane_pos.z);
	glRotatef(m_pCamere->m_hpr.x, 0, 1, 0);
	glRotatef(m_pCamere->m_hpr.y, 1, 0, 0);
	glRotatef(m_pCamere->m_hpr.z, 0, 0, 1);
	
	//���׷ɻ�(��ͷ)��ʼ����Ϊy�������򣬵������ӵ㷽��z��һ��
	glRotatef(-90, 1, 0, 0);
	//�ĸ�����
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, size * ship_prop, 0.0f);
	glVertex3f(-size, -size, size);
	glVertex3f(size, -size, size);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, size * ship_prop, 0.0f);
	glVertex3f(size, -size, size);
	glVertex3f(size, -size, -size);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, size * ship_prop, 0.0f);
	glVertex3f(size, -size, -size);
	glVertex3f(-size, -size, -size);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, size * ship_prop, 0.0f);
	glVertex3f(-size, -size, -size);
	glVertex3f(-size, -size, size);
	glEnd();
	//����
	glBegin(GL_QUADS);
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(-size, -size, size);
	glVertex3f(-size, -size, -size);
	glVertex3f(size, -size, -size);
	glVertex3f(size, -size, size);
	glEnd();

	glPopMatrix();
}

//==================
//COpenGL����ʵ�ֲ���
//==================

CmyOpenGL::CmyOpenGL() {

}

CmyOpenGL::~CmyOpenGL() {

}

//�����ʼ��ʱ����
void CmyOpenGL::PostInit() {
	m_pControl->SetSpeed(mspeed, rspeed);

	glClearColor(0, 0, 0, 1);
	m_pCamere->SetCamera(startAt, CglVector3(0, 0, 0), true);
	m_pCamere->SaveCamera();

	//���û���
	SetRC();
	//��ʼ������
	initStar();
}

//�ܻ��ƺ���
void CmyOpenGL::InDraw() {
	//������������
	m_pShape->DrawCoord(1000);
	//��ģ��
	DrawModel();
	
	CString str;
	glColor3f(1, 0, 0);
	str.Format("%.2f,%.2f,%.2f", m_pCamere->m_pos.x, m_pCamere->m_pos.y, m_pCamere->m_pos.z);
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
			//���ü��̿��ƺ���������״̬
			mode = (mode + 1) % 2;
			break;
			
		}
	return false;
}

void CmyOpenGL::DrawModel() {
	//SetView();
	//������
	drawStar();
	//���ɻ�
	drawPlane(m_pCamere);
	//��β��
	drawTail();
}
