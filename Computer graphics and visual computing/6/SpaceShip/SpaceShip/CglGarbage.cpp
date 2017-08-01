#include "stdafx.h"
#include <GL/glew.h>
#include "CglGarbage.h"
#pragma comment(lib,"glew32.lib ")

CglGarbage::CglGarbage() {
	r_dir = rand() % 3;
	r_v = rand() % 50;
	point = 1;
	m_dir = CglVector3(0, 0, -1);
	m_pos = CglVector3(rand() % 3000, rand() % 3000, rand() % 3000);
	m_updir = CglVector3(0, 1, 0);
	m_matrix.SetTrans(m_pos);
	step[0] = 0.01;
	step[1] = 0.01;
	key_step = 3;
	status = 1;
}

CglGarbage::~CglGarbage() {

}

void CglGarbage::draw(float size, CglPatch patch, vector<CglPatch> *patchList) {
	//垃圾
	if (status == 1) {
		glPushMatrix();
		glMultMatrixd(m_matrix);
		glColor3f(1, 1, 1);
		GLUquadricObj *quadPlanet;
		quadPlanet = gluNewQuadric();
		gluQuadricTexture(quadPlanet, GL_TRUE);
		gluSphere(quadPlanet, size, 20, 16);
		glPopMatrix();
	}
	//碎片
	else {
		patch.Draw(patchList);
	}	
}

//dir=0表示左右移动，dir=1表示上下移动，dir=2表示前后移动
float CglGarbage::Move(int dir, double plus_or_minus, float usetime) {
	float movedis = usetime * key_step / 30;
	CglMatrix m_tr;
	if (dir == 0)
		m_tr.SetTrans(CglVector3(-movedis * step[0] * plus_or_minus, 0, 0));
	else if (dir == 1)
		m_tr.SetTrans(CglVector3(0, -movedis * step[0] * plus_or_minus, 0));
	else
		m_tr.SetTrans(CglVector3(0, 0, -movedis * step[0] * plus_or_minus));
	m_matrix = m_matrix * m_tr;
	m_pos = &m_matrix[12];
	return movedis * step[0];
}

//dir=0表示左右转，dir=1表示上下转，dir=2表示滚动转
void CglGarbage::Rotate(int dir, double plus_or_minus, float usetime) {
	float rotdis = usetime * key_step / 30;
	CglMatrix m_ro;
	if (dir == 0)
		m_ro.SetRotate(rotdis*step[1] * plus_or_minus, CglVector3(0, 1, 0));//h
	else if (dir == 1)
		m_ro.SetRotate(rotdis*step[1] * plus_or_minus, CglVector3(1, 0, 0));//p
	else
		m_ro.SetRotate(rotdis*step[1] * plus_or_minus, CglVector3(0, 0, 1));//r

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

void CglGarbage::SetDir(CglVector3 dir) {
	dir.Normalize();
	CglVector3 xdir = dir.crossMul(m_updir);	//确定子坐标系x方向
	xdir.Normalize();
	CglVector3 ydir = xdir.crossMul(dir);		//确定子坐标系y方向
	ydir.Normalize();
	m_matrix.SetCol(0, xdir);
	m_matrix.SetCol(1, ydir);
	m_matrix.SetCol(2, -dir);
	m_updir = ydir;
	m_dir = dir;
}

float CglGarbage::GetMSpeed() {
	return step[0] * key_step;
}

float CglGarbage::GetRSpeed() {
	return step[1] * key_step;
}

int CglGarbage::GetUseTime() {
	int curtime = GetTickCount();
	int usetime = 0;
	if (lasttime > 0) {
		usetime = curtime - lasttime;
	}
	lasttime = curtime;
	return usetime;
}
