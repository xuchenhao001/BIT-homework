#include "stdafx.h"
#include <GL/glew.h>
#include "CmyOpenGL.h"
#pragma comment(lib,"glew32.lib ")

//====================================
//ȫ�ֱ������岿��(����������ʵ��޸�)
//====================================
int width = 1280, height = 720;						//���ǻ��ƴ���Ĭ�ϴ�СΪ1280x720
const int STAR_NUM = 10000;							//������������Ĭ��Ϊ10000��
const int SMOOTH = 100;								//�ӵ����ƽ���̶ȣ���ֵԽ��Խƽ����Ĭ��Ϊ100
const double AIR_PLANE_SIZE = 3;					//�ɻ���С
const double AIR_PLANE_PROP = 10;					//�ɻ��������
int TAIL_LEN = AIR_PLANE_SIZE*AIR_PLANE_PROP * 100;	//Сβ�ͳ���+1��Ĭ��Ϊ�ɻ�����100��
float view_mspeed = 0.1, view_rspeed = 0.05;		//�ӵ��ʼ�ƶ��ٶȺ���ת�ٶ�
float ship_mspeed = 0.1, ship_rspeed = 0.5;			//�ɻ���ʼ�ƶ��ٶȺ���ת�ٶ�
CglVector3 startAt(10, 20, 20);						//�趨�ӵ���ʼλ��

int view_mode = 0;									//��������ʱ���ӵ�״̬��0ŷ���ӽ� 1�����ӵ����
int view_pos = 0;									//��������ʱ�ӵ�λ��: 0���ɱ任 1����ɻ�
int flyMode = 0;									//��������ʱ�ķ���״̬��0�ֶ����� 1�������Ǻ���

//=========================
//ȫ�ֱ������岿��(���ɸ���)
//=========================

CglVector3 stars[STAR_NUM];				//�ǿ�
cycleQueue<CglVector3> tail(TAIL_LEN);	//�ɰ���Сβ��
CglVector3 *nowStar = stars;			//��ǰ����ָ��

int usetime;
int step = SMOOTH;						//����ƽ����ֵ����
double t[SMOOTH];						//ƽ����������
CglQuaternion slerpResult[SMOOTH];		//ƽ����ֵ�����Ԫ������
CglVector3 posResult[SMOOTH];			//ƽ����ֵλ�ý������
bool trick = false;						//��Ч����״̬����
bool toNextStar = false;				//ת����һ�����ǿ���

//==============
//���ƺ���ʵ�ֲ���
//==============

//����������뺯��
float posDis(CglVector3 a, CglVector3 b) {
	return (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) + (a.z - b.z)*(a.z - b.z);;
}

//�����ǽ�������
void sortStars() {
	stars[0] = CglVector3(0, 0, 0);
	CglVector3 tmp_star;
	float tmp_dis = FLT_MAX;
	int tmp_j;
	for (int i = 0; i < STAR_NUM - 1; i++) {
		tmp_dis = FLT_MAX;
		tmp_j = i + 1;
		for (int j = i + 1; j < STAR_NUM; j++) {
			float dis = posDis(stars[i], stars[j]);
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
	
	sortStars();
}

//���û���
void SetRC() {

	//������Բ��(�����ҵ��Կ����������⣬��������Ǵ�С�任��ͻ����ע�ӵ���)
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
	for (int i = 0; i < tail.nowSize() - 1; i++) {
		float color;
		glLineWidth(1);
		//��β�ͳ��ȴﵽ���ʱ�򣬿������20%��β�ͽ�����ʧ
		if (tail.nowSize() == tail.size() - 1 && i < tail.size() / 5.0) {
			color = i / (tail.size() / 5.0);
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

//���Ǽ���
void drawStarLine() {
	CglVector3 *pStar = stars;

	glPushMatrix();
	glLineWidth(1);
	glColor3f(1.0, 1.0, 0);
	glBegin(GL_LINE_STRIP);
	do{
		glVertex3f(pStar->x, pStar->y, pStar->z);
		glVertex3f((pStar + 1)->x, (pStar + 1)->y, (pStar + 1)->z);
		pStar++;
	} while (pStar != nowStar);
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
	//���÷ɴ���ʼ�ٶ�
	airPlane.SetSpeed(ship_mspeed, ship_rspeed);
}

//�ܻ��ƺ���
void CmyOpenGL::InDraw() {
	//������������
	m_pShape->DrawCoord(1000);
	//��ģ��
	DrawModel();
	
	//������ʾ
	/*CString str;
	glColor3f(1, 0, 0);
	str.Format("%lf, %lf, %lf", (nowStar + 1)->x, (nowStar + 1)->y, (nowStar + 1)->z);
	m_pFont->Font2D(str.GetBuffer(0), -0.9, 0.9, 7);*/
}

//����ÿһ֡��ͼ������
void CmyOpenGL::Update() {
	usetime = airPlane.GetUseTime();

	//��ǰ, ������, ��β��
	float movdis = airPlane.Move(2, 1, usetime);

	//�����Ǽ��߷���
	if (flyMode == 1) {
		if (toNextStar == true) {
			toNextStar = false;
			airPlane.SetDir(*nowStar - airPlane.m_pos);
		}
		//�ɻ�����Ŀ������0.1ʱ����Ϊ�ɻ��Ѿ��ﵽĿ������
		if (posDis(*nowStar, airPlane.m_pos) <= movdis*2) {
			toNextStar = true;
			nowStar++;
		}
	}
	
	//�ӵ��������ɻ�
	if (view_pos == 1) {
		CglVector3 follow_pos = airPlane.m_pos - airPlane.m_dir * 30 + airPlane.m_updir * 15;
		m_pCamere->followCamera(follow_pos, airPlane.m_pos, airPlane.m_updir);
	}

	//β�͸���
	for (int i = 0; i < movdis; i++) {
		if (tail.isFull()) {
			tail.pop();
		}
		tail.push(airPlane.m_pos);
	}

	//�����ؼ�
	if (trick == true) {
		airPlane.Trick(usetime);
	}
}

bool CmyOpenGL::OnKey(unsigned int nChar, bool bDown) {
	if (bDown)
		switch (nChar) {

		//�任����ģʽ, 0Ϊ�ֶ�����, 1Ϊ�������Ǻ���
		case VK_F1:
			flyMode = 1 - flyMode;
			toNextStar = true;
			nowStar++;
			break;

		//�任�ӵ�ģʽ, 0Ϊŷ������ͼ, 1Ϊ���ɱ任�ӽ�
		case VK_F2:
			view_mode = 1 - view_mode;
			m_pCamere->m_type = view_mode;
			break;

		//�ɴ��Զ�����, ����, [,<]��
		case VK_OEM_COMMA:
			ship_mspeed += 0.1;
			airPlane.SetSpeed(ship_mspeed, ship_rspeed);
			break;
		//�ɴ��Զ�����, ����, [.>]��
		case VK_OEM_PERIOD:
			ship_mspeed -= 0.1;
			airPlane.SetSpeed(ship_mspeed, ship_rspeed);
			break;

		//�ɴ��ؼ�[/?]��
		case VK_OEM_2:
			trick = (trick == true) ? false : true;
			break;

		//�����ӵ�任, [space]��
		case VK_SPACE:
			view_pos = 1 - view_pos;
			break;

		//�ɴ����з������
		case VK_LEFT:
			airPlane.Rotate(0, 1, usetime);//����������
			break;
		case VK_RIGHT:
			airPlane.Rotate(0, -1, usetime);//���󣬸��������ң�
			break;
		case VK_UP:
			airPlane.Rotate(1, 1, usetime);//���ϣ�������
			break;
		case VK_DOWN:
			airPlane.Rotate(1, -1, usetime);//���ϣ����������£�
			break;
		}
	return false;
}

void CmyOpenGL::DrawModel() {
	drawStar();//������
	drawTail();//��β��
	//���ɻ�
	airPlane.Draw(AIR_PLANE_SIZE, AIR_PLANE_PROP);
	//���Ǽ���
	if (flyMode == 1) {
		drawStarLine();
	}
}
