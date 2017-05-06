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
	m_fStep[0] = 0.01;
	m_fStep[1] = 0.01;
	m_fKeyStep = 3;
}

void CglPlane::Draw(double size) {
	//飞机前后长度和左右宽度的比例
	double ship_prop = 10;

	glPushMatrix();
	//载入模型矩阵变化
	glMultMatrixd(m_matrix);

	//简易飞机当前位置变化
	//glTranslatef(m_pos.x, m_pos.y, m_pos.z);
	//简易飞机(箭头)初始方向为y轴正方向，调整与视点方向负z轴一致
	glRotatef(-90, 1, 0, 0);
	//四个侧面
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
	//底面
	glBegin(GL_QUADS);
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(0 - size, 0 - size, 0 + size);
	glVertex3f(0 - size, 0 - size, 0 - size);
	glVertex3f(0 + size, 0 - size, 0 - size);
	glVertex3f(0 + size, 0 - size, 0 + size);
	glEnd();

	glPopMatrix();
}

void CglPlane::Move(int dir, double plus_or_minus) {
	CglMatrix m_tr;
	if (dir == 0)
		m_tr.SetTrans(CglVector3(-m_fKeyStep*m_fStep[0] * plus_or_minus, 0, 0));
	else if (dir == 1) 
		m_tr.SetTrans(CglVector3(0, -m_fKeyStep*m_fStep[0] * plus_or_minus, 0));
	else
		m_tr.SetTrans(CglVector3(0, 0, -m_fKeyStep*m_fStep[0] * plus_or_minus));
	m_matrix = m_matrix*m_tr;
	m_pos = &m_matrix[12];
}

//dir=0表示左右转，dir=1表示上下转，dir=2表示滚动转
void CglPlane::Rotate(int dir, double plus_or_minus) {
	CglMatrix m_ro;
	if (dir == 0)
		m_ro.SetRotate(m_fKeyStep*m_fStep[1]*plus_or_minus, CglVector3(0, 1, 0));//h
	else if (dir == 1)
		m_ro.SetRotate(m_fKeyStep*m_fStep[1]*plus_or_minus, CglVector3(1, 0, 0));//p
	else
		m_ro.SetRotate(m_fKeyStep*m_fStep[1]*plus_or_minus, CglVector3(0, 0, 1));//r
	
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
	m_fStep[0] = mspeed;
	m_fStep[1] = rspeed;
}

//if (mode == 0) {
//	//设置飞机与视点前后距离
//	CglVector3 fore_aft_dis;
//	fore_aft_dis.Set(&m_pCamere->m_viewMatrixInverse[8]);
//	fore_aft_dis = fore_aft_dis * 8;
//	//设置飞机与视点上下距离
//	CglVector3 up_down_dis;
//	up_down_dis.Set(&m_pCamere->m_viewMatrixInverse[4]);
//	up_down_dis = up_down_dis * 5;
//	//完成最终调整向量
//	CglVector3 plane_pos;
//	plane_pos = m_pCamere->m_pos - fore_aft_dis - up_down_dis;
//	//调整飞机位置姿势
//	glTranslatef(plane_pos.x, plane_pos.y, plane_pos.z);
//	glRotatef(m_pCamere->m_hpr.x, 0, 1, 0);
//	glRotatef(m_pCamere->m_hpr.y, 1, 0, 0);
//	glRotatef(m_pCamere->m_hpr.z, 0, 0, 1);
//} else if (step != SMOOTH) {
//	CglEuler slerpNowEuler = slerpResult[step].ToEuler();
//	CglVector3 slerpNowPos = posResult[step];

//	//调整飞机位置姿势
//	glTranslatef(slerpNowPos.x, slerpNowPos.y, slerpNowPos.z);
//	glRotatef(slerpNowEuler.h, 0, 1, 0);
//	glRotatef(slerpNowEuler.p, 1, 0, 0);
//	glRotatef(slerpNowEuler.b, 0, 0, 1);
//	step++;
//}