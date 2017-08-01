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
//float ship_max_mspeed = 1;							//�ɻ���ʼ����ƶ��ٶ�
CglVector3 startAt(10, 20, 20);						//�趨�ӵ���ʼλ��

int view_mode = 0;									//��������ʱ���ӵ�״̬��0ŷ���ӽ� 1�����ӵ����
int view_pos = 1;									//��������ʱ�ӵ�λ��: 0���ɱ任 1����ɻ�

//=========================
//ȫ�ֱ������岿��(���ɸ���)
//=========================

CglVector3 stars[STAR_NUM];				//�ǿ�
cycleQueue<CglVector3> tail(TAIL_LEN);	//�ɰ���Сβ��

int usetime;
int step = SMOOTH;						//����ƽ����ֵ����
double t[SMOOTH];						//ƽ����������
CglQuaternion slerpResult[SMOOTH];		//ƽ����ֵ�����Ԫ������
CglVector3 posResult[SMOOTH];			//ƽ����ֵλ�ý������
GLuint texturePlane;					//�ɻ���ͼ
CglMaterial materialPlane;				//�ɻ�����
GLuint textureSky;						//�����ͼ
CglMaterial materialSky;				//��ղ���
GLuint textureGarbage;					//�����ͼ
CglMaterial materialGarbage;			//��ղ���
GLuint textureTail;						//β����ͼ
CglMaterial materialTail;				//β�����
GLuint texturePatch;					//��Ƭ��ͼ
CglMaterial materialPatch;	    		//��Ƭ����


SkyBox mybox;							//��պ�
CglPlane airPlane;
list<CglGarbage> garbageList;			//����������
CglPatch *patch;						//������Ƭ��
vector<CglPatch> patchList;				//������Ƭ����

//==============
//���ƺ���ʵ�ֲ���
//==============

//���û���
void SetRC() {

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glShadeModel(GL_FLAT);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);

	srand((unsigned)time(NULL));

	glEnable(GL_LIGHTING);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, sunambient);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, sunpos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, suncolor);
	glLightfv(GL_LIGHT0, GL_AMBIENT, sunambient);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

//==================
//Garbage������
//==================

//�б�����������
void addGarbage(int num) {
	while (num--) {
		CglGarbage* garbage = new CglGarbage();
		garbageList.push_back(*garbage);
	}
}

//�������б�
void drawGarbage() {
	if (!garbageList.size()) return;
	list<CglGarbage>::iterator it;
	for (it = garbageList.begin(); it != garbageList.end(); it++) {
		it->draw(2, *patch, &patchList);
	}
}

//�ж�������ɴ�����, Զ�뷵��1, ��������0
int garbageJudge(CglVector3 garbagePos, CglVector3 airPlanePos) {
	float disX = garbagePos[0] - airPlanePos[0];
	float disY = garbagePos[1] - airPlanePos[1];
	float disZ = garbagePos[2] - airPlanePos[2];
	float dis = disX * disX + disY * disY + disZ * disZ;
	if (dis > airPlane.range) {
		return 1;
	}
	return 0;
}

//���������б�, ����ÿһ��������λ��, �Լ��Ƿ����
void updateGarbage(CglVector3 airPlanePos) {
	list<CglGarbage>::iterator it;
	for (it = garbageList.begin(); it != garbageList.end(); it++) {
		int usetime = it->GetUseTime();
		it->Move(2, 1, usetime);
		it->Rotate(it->r_dir, it->r_v, 10);
		if (!garbageJudge(it->m_pos, airPlanePos)) {
			it->status = 0;
			patch->Reset(&patchList, it->m_pos, airPlane.m_dir, airPlane.GetMSpeed());
			airPlane.speed_Max += 1;
			//����ɻ�����
			airPlane.score += it->point;
			if ((airPlane.score) % 10 == 0) {
				airPlane.range += 1;
			}
		}
	}
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
	//�����ӵ��ʼ�ٶ�
	m_pControl->SetSpeed(view_mspeed, view_rspeed);
	//��ʼ��β��ƽ������
	for (int i = 0; i < SMOOTH; i++) {
		t[i] = i / SMOOTH;
	}
	glClearColor(0, 0, 0, 1);
	m_pCamere->SetCamera(startAt, CglVector3(0, 0, 0), true);
	//���û���
	SetRC();

	//������պ�����
	textureSky = materialSky.LoadGLTextures("sky.bmp");

	//��ʼ���ɻ�����
	texturePlane = materialPlane.LoadGLTextures("plane.bmp"); 

	//��ʼ����������
	textureGarbage = materialGarbage.LoadGLTextures("garbage.bmp");
	//��ʼ������
	addGarbage(1000);

	//��ʼ��β��
	//textureTail = materialTail.LoadGLTextures("flame.bmp");

	//��ʼ����Ƭ
	patch = new CglPatch();
	patch->Init(&patchList, 100);

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
	CString str;
	glColor3f(1, 0, 0);

	str.Format("Score : %d", airPlane.score);
	m_pFont->Font2D(str.GetBuffer(0), -0.9, 0.9, 7);
	
	str.Format("AirPlane Speed : %0.2f", airPlane.GetMSpeed()*10);
	m_pFont->Font2D(str.GetBuffer(0), -0.9, 0.82, 7);

	str.Format("AirPlane Max Speed : %0.2f", airPlane.speed_Max);
	m_pFont->Font2D(str.GetBuffer(0), -0.9, 0.74, 7);

	str.Format("AirPlane Pos : (%0.2f,%0.2f,%0.2f)",airPlane.m_pos.x, airPlane.m_pos.y, airPlane.m_pos.z);
	m_pFont->Font2D(str.GetBuffer(0), -0.9, 0.66, 7);

	str.Format("AirPlane Range : %0.2f", airPlane.range);
	m_pFont->Font2D(str.GetBuffer(0), -0.9, 0.58, 7);
}

//����ÿһ֡��ͼ������
void CmyOpenGL::Update() {

	//���·ɻ�״̬
	usetime = airPlane.GetUseTime();
	float movdis = airPlane.Move(2, 1, usetime);

	//��������״̬
	updateGarbage(airPlane.m_pos);
	patch->Update(&patchList);

	//�ӵ��������ɻ�
	if (view_pos == 1) {
		CglVector3 follow_pos = airPlane.m_pos - airPlane.m_dir * 7 + airPlane.m_updir * 3;
		m_pCamere->followCamera(follow_pos, airPlane.m_pos, airPlane.m_updir); 
	}

	//β�͸���
	for (int i = 0; i < movdis; i++) {
		if (tail.isFull()) {
			tail.pop();
		}
		tail.push(airPlane.m_pos);
	}
}

bool CmyOpenGL::OnKey(unsigned int nChar, bool bDown) {
	if (bDown)
		switch (nChar) {
		//�任�ӵ�ģʽ, 0Ϊŷ������ͼ, 1Ϊ���ɱ任�ӽ�
		case VK_F2:
			view_mode = 1 - view_mode;
			m_pCamere->m_type = view_mode;
			break;

		//�����ӵ�任, [space]��
		case VK_SPACE:
			view_pos = 1 - view_pos;
			break;

		//�ɴ��Զ�����, ����, [,<]��
		case VK_OEM_COMMA:
			if (ship_mspeed < airPlane.speed_Max) {
				ship_mspeed += 0.1;
			}
			airPlane.SetSpeed(ship_mspeed, ship_rspeed);
			break;
		//�ɴ��Զ�����, ����, [.>]��
		case VK_OEM_PERIOD:
			ship_mspeed -= 0.1;
			if (ship_mspeed <= 0.1) {
				ship_mspeed = 0.1;
			}
			airPlane.SetSpeed(ship_mspeed, ship_rspeed);
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
	drawTail();//��β��

	//����պУ����뾶�����ȣ�γ�ȣ�
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureSky);
	mybox.DrawBall(3000, 30, 30);
	glDisable(GL_TEXTURE_2D);

	//���ɻ�
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturePlane);
	//����ɻ������ĵ�������2000���ɻ��ٶ�ȡ0
	if ((airPlane.m_pos.len() - CglVector3(0, 0, 0).len()) > 2000) {
		airPlane.SetSpeed(0,0.01);
		//airPlane.m_dir = -airPlane.m_dir;
		//airPlane.m_pos = CglVector3(0, 0, 0);
	}
	airPlane.Draw();
	glDisable(GL_TEXTURE_2D);

	//������
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureGarbage);
	drawGarbage();
	glDisable(GL_TEXTURE_2D);
}
