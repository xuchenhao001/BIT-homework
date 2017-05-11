#include "stdafx.h"
#include "CglPlane.h"

CglPlane::CglPlane() {
	
}

CglPlane::~CglPlane() {

}

void CglPlane::Init() {
	m_dir = CglVector3(0, 0, -1);
	m_pos = CglVector3(0, 0, 0);
	m_updir = CglVector3(0, 1, 0);
	m_matrix.Identity();
	scale_step[0] = 0.01;
	scale_step[1] = 0.01;
	key_step = 3;
}

void CglPlane::Draw(double size, double prop) {
	//�ɻ�ǰ�󳤶Ⱥ����ҿ�ȵı���
	double ship_prop = prop;

	glPushMatrix();
	//����ģ�;���仯
	glMultMatrixd(m_matrix);

	//���׷ɻ�(��ͷ)��ʼ����Ϊy�������򣬵������ӵ㷽��z��һ��
	glRotatef(-90, 1, 0, 0);
	//�ĸ�����
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0, 0 + size * ship_prop, 0);
	glVertex3f(0 - size, 0 - size, 0 + size);
	glVertex3f(0 + size, 0 - size, 0 + size);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0, 0 + size * ship_prop, 0);
	glVertex3f(0 + size, 0 - size, 0 + size);
	glVertex3f(0 + size, 0 - size, 0 - size);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0, 0 + size * ship_prop, 0);
	glVertex3f(0 + size, 0 - size, 0 - size);
	glVertex3f(0 - size, 0 - size, 0 - size);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0, 0 + size * ship_prop, 0);
	glVertex3f(0 - size, 0 - size, 0 - size);
	glVertex3f(0 - size, 0 - size, 0 + size);
	glEnd();
	//����
	glBegin(GL_QUADS);
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(0 - size, 0 - size, 0 + size);
	glVertex3f(0 - size, 0 - size, 0 - size);
	glVertex3f(0 + size, 0 - size, 0 - size);
	glVertex3f(0 + size, 0 - size, 0 + size);
	glEnd();

	glPopMatrix();
}
int lasttime = -1;
void CglPlane::Move(int dir, double plus_or_minus) {
	int curtime = GetTickCount();
	int usetime = 0;
	if (lasttime > 0) 
	{
		usetime = curtime - lasttime;
	}
	lasttime = curtime;
	float movedis = usetime*key_step / 30;
	CglMatrix m_tr;
	if (dir == 0)
		m_tr.SetTrans(CglVector3(-movedis * scale_step[0] * plus_or_minus, 0, 0));
	else if (dir == 1) 
		m_tr.SetTrans(CglVector3(0, -movedis * scale_step[0] * plus_or_minus, 0));
	else
		m_tr.SetTrans(CglVector3(0, 0, -movedis * scale_step[0] * plus_or_minus));
	m_matrix = m_matrix * m_tr;
	m_pos = &m_matrix[12];
}

//dir=0��ʾ����ת��dir=1��ʾ����ת��dir=2��ʾ����ת
void CglPlane::Rotate(int dir, double plus_or_minus) {
	CglMatrix m_ro;
	if (dir == 0)
		m_ro.SetRotate(key_step*scale_step[1]*plus_or_minus, CglVector3(0, 1, 0));//h
	else if (dir == 1)
		m_ro.SetRotate(key_step*scale_step[1]*plus_or_minus, CglVector3(1, 0, 0));//p
	else
		m_ro.SetRotate(key_step*scale_step[1]*plus_or_minus, CglVector3(0, 0, 1));//r
	
	m_matrix[12] = 0;
	m_matrix[13] = 0;
	m_matrix[14] = 0;
	m_matrix = m_matrix*m_ro;
	m_matrix[12] += m_pos.x;
	m_matrix[13] += m_pos.y;
	m_matrix[14] += m_pos.z;
	
	m_dir = m_matrix.vecMul(CglVector3(0, 0, -1));
	m_updir = m_matrix.vecMul(CglVector3(0, 1, 0));
}

void CglPlane::SetSpeed(float mspeed, float rspeed) {
	scale_step[0] = mspeed;
	scale_step[1] = rspeed;
}

float CglPlane::getMSpeed() {
	return scale_step[0] * key_step;
}

float CglPlane::getRSpeed() {
	return scale_step[1] * key_step;
}