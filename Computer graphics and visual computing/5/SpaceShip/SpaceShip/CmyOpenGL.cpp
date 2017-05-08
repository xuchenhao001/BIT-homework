#include "stdafx.h"
#include <GL/glew.h>
#include "CmyOpenGL.h"
#pragma comment(lib,"glew32.lib ")

//====================================
//ȫ�ֱ������岿��(����������ʵ��޸�)
//====================================
int width = 1280, height = 720;					//���ǻ��ƴ���Ĭ�ϴ�СΪ1280x720
const int STAR_NUM = 10000;						//������������Ĭ��Ϊ10000��
const int TAIL_LEN = 121;						//Сβ�ͳ���+1��Ĭ��Ϊ120��
const int SMOOTH = 100;							//�ӵ����ƽ���̶ȣ���ֵԽ��Խƽ����Ĭ��Ϊ100
const double AIR_PLANE_SIZE = 3;				//�ɻ���С
float view_mspeed = 0.1, view_rspeed = 0.05;	//�ӵ��ʼ�ƶ��ٶȺ���ת�ٶ�
float ship_mspeed = 0.1, ship_rspeed = 0.5;		//�ɻ���ʼ�ƶ��ٶȺ���ת�ٶ�
CglVector3 startAt(10, 20, 20);					//�趨�ӵ���ʼλ��

int view_mode = 0;								//��������ʱ���ӵ�״̬��0ŷ���ӽ� 1�����ӵ����
int view_pos = 0;								//��������ʱ�ӵ�λ��: 0���ɱ任 1����ɻ�
int fly_mode = 0;								//��������ʱ�ķ���״̬��0�ֶ����� 1�������Ǻ���

//=========================
//ȫ�ֱ������岿��(���ɸ���)
//=========================

CglVector3 stars[STAR_NUM];				//�ǿ�
cycleQueue<CglVector3> tail(TAIL_LEN);	//�ɰ���Сβ��
CglVector3 *now_star = stars;			//��ǰ����ָ��

int step = SMOOTH;						//����ƽ����ֵ����
double t[SMOOTH];						//ƽ����������
CglQuaternion slerpResult[SMOOTH];		//ƽ����ֵ�����Ԫ������
CglVector3 posResult[SMOOTH];			//ƽ����ֵλ�ý������

CglVector3 follow_pos;
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

//���߶�
void drawLine() {
	glPushMatrix();
	glTranslatef(0, 0, -1000);
	glBegin(GL_LINE_STRIP);
	glVertex3f(now_star->x, now_star->y, now_star->z);
	now_star++;
	glVertex3f(now_star->x, now_star->y, now_star->z);
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
	m_pControl->SetSpeed(view_mspeed, view_rspeed);//�����ӵ��ʼ�ٶ�
	//��ʼ��ƽ������
	for (int i = 0; i < SMOOTH; i++) {
		t[i] = i / SMOOTH;
	}

	glClearColor(0, 0, 0, 1);
	m_pCamere->SetCamera(startAt, CglVector3(0, 0, 0), true);

	//���û���
	SetRC();
	//��ʼ������
	initStar();
	//��ʼ���ɻ�
	airPlane.Init();
	airPlane.SetSpeed(ship_mspeed, ship_rspeed);//���÷ɴ���ʼ�ٶ�
}

//�ܻ��ƺ���
void CmyOpenGL::InDraw() {
	//������������
	m_pShape->DrawCoord(1000);
	//��ģ��
	DrawModel();
	
	CString str;
	glColor3f(1, 0, 0);
	str.Format("%.2f, %.2f, %.2f", follow_pos.x, follow_pos.y, follow_pos.z);
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

		//�任����ģʽ, 0Ϊ�ֶ�����, 1Ϊ�������Ǻ���
		case VK_F1:
			//fly_mode = 1 - fly_mode;

			break;

		//�任�ӵ�ģʽ, 0Ϊŷ������ͼ, 1Ϊ���ɱ任�ӽ�
		case VK_F2:
			view_mode = 1 - view_mode;
			m_pCamere->SaveCamera();
			m_pCamere->m_type = view_mode;
			m_pCamere->LoadCamera();
			break;

		//�ɴ��Զ�����, ����
		case VK_OEM_COMMA:
			ship_mspeed += 0.1;
			airPlane.SetSpeed(ship_mspeed, ship_rspeed);
			break;
		//�ɴ��Զ�����, ����
		case VK_OEM_PERIOD:
			ship_mspeed -= 0.1;
			airPlane.SetSpeed(ship_mspeed, ship_rspeed);
			break;

		//�����ӵ�任
		case VK_SPACE:
			view_pos = 1 - view_pos;
			break;

		//�ɴ����з������
		case VK_LEFT:
			airPlane.Rotate(0, 1);//����������
			break;
		case VK_RIGHT:
			airPlane.Rotate(0, -1);//���󣬸��������ң�
			break;
		case VK_UP:
			airPlane.Rotate(1, 1);//���ϣ�������
			break;
		case VK_DOWN:
			airPlane.Rotate(1, -1);//���ϣ����������£�
			break;
		}
	return false;
}

void CmyOpenGL::DrawModel() {
	drawStar();//������
	drawTail();//��β��

	//��ǰ���켣
	/*if (view_mode == 1 && step == 0) {
		drawLine();
	}*/

	//���ɻ�
	airPlane.Draw(AIR_PLANE_SIZE);
	//airPlane.Move(2, 1);//��ǰ��������

	//�ӵ��Ƿ����ɻ�
	if (view_pos == 1) {
		follow_pos = airPlane.m_pos - airPlane.m_dir * 30 + airPlane.m_updir * 15;
		m_pCamere->followCamera(follow_pos, airPlane.m_pos, airPlane.m_matrix);
	}
	
}
