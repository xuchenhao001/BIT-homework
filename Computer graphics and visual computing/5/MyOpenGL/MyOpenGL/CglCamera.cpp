#include "stdafx.h"
#include "CglCamera.h"
#include "CglMath.h"
#include "CglQuaternion.h"
#include "CglMatrix.h"
#include "CglVector3.h"
#include "CglEuler.h"
#include "gl\glut.h"

CglCamera::CglCamera() {
	Init();
}

CglCamera::~CglCamera() {

}

void CglCamera::Init() {
	m_type = 0;
	m_pos = CglVector3(0, 0, 0);
	m_eyedir = CglVector3(0, 0, -1);
	m_updir = CglVector3(0, 1, 0);
	m_hpr = CglVector3(0, 0, 0);
	int i;
	for (i = 0; i<16; i++) {
		if (i == 0 || i == 5 || i == 10 || i == 15) {
			m_viewMatrix[i] = 1;
			m_viewMatrixSave[i] = 1;
		} else {
			m_viewMatrixSave[i] = 0;
			m_viewMatrix[i] = 0;
		}
	}
}

void CglCamera::Move(int dir, double len) {//dir=0��ʾ�����ƣ�dir=1��ʾ�����ƣ�dir=2��ʾǰ����
	if (m_type == 0) {
		CglVector3 mov;
		if (dir == 0) {
			mov.Set(&m_viewMatrixInverse[0]);
		} else if (dir == 1) {
			mov.Set(&m_viewMatrixInverse[4]);
		} else if (dir == 2) {
			mov.Set(&m_viewMatrixInverse[8]);
		}
		m_pos += mov*len;
	} else if (m_type == 1) {
		glPushMatrix();
		glLoadIdentity();
		if (dir == 0)
			glTranslatef(-len, 0, 0);
		else if (dir == 1)
			glTranslatef(0, -len, 0);
		else
			glTranslatef(0, 0, -len);
		glMultMatrixd(m_viewMatrix);
		glGetDoublev(GL_MODELVIEW_MATRIX, m_viewMatrix);
		CglMath::InverseMatrix(m_viewMatrix, m_viewMatrixInverse);
		glPopMatrix();
		m_pos = &m_viewMatrixInverse[12];
	}
}

void CglCamera::Rotate(int dir, double len) {//dir=0��ʾ����ת��dir=1��ʾ����ת��dir=2��ʾ����ת
	if (m_type == 0) {
		m_hpr[dir] += len;
	} else if (m_type == 1) {
		glPushMatrix();
		glLoadIdentity();
		if (dir == 0)
			glRotated(-len, 0, 1, 0);	//h
		else if (dir == 1)
			glRotated(-len, 1, 0, 0);	//p
		else
			glRotated(-len, 0, 0, 1);	//r
		glMultMatrixd(m_viewMatrix);
		glGetDoublev(GL_MODELVIEW_MATRIX, m_viewMatrix);
		CglMath::InverseMatrix(m_viewMatrix, m_viewMatrixInverse);
		glPopMatrix();

		m_eyedir = m_viewMatrixInverse*CglVector3(0, 0, -1);
		m_eyedir -= m_pos;

		m_updir = m_viewMatrixInverse*CglVector3(0, 1, 0);
		m_updir -= m_pos;
	}
}

void CglCamera::ShowView() {
	if (m_type == 0) {
		glRotated(-m_hpr[2], 0, 0, 1);	//r
		glRotated(-m_hpr[1], 1, 0, 0);	//p
		glRotated(-m_hpr[0], 0, 1, 0);	//h
		glTranslatef(-m_pos[0], -m_pos[1], -m_pos[2]);
	} else if (m_type == 1) {
		gluLookAt(m_pos.x, m_pos.y, m_pos.z,
			m_pos.x + m_eyedir.x, m_pos.y + m_eyedir.y, m_pos.z + m_eyedir.z,
			m_updir.x, m_updir.y, m_updir.z
			);
	}
	glGetDoublev(GL_MODELVIEW_MATRIX, m_viewMatrix);
	CglMath::InverseMatrix(m_viewMatrix, m_viewMatrixInverse);
}

void CglCamera::SetCamera(const CglVector3 &pos, const CglVector3 &obj_dir, bool bObj, int type) {
	m_pos = pos;
	//����ڶ��������ǹ۲�Ķ���λ��
	if (bObj) {
		m_type = 1;
		m_eyedir = obj_dir - pos;
	}
	//�ڶ��������ǹ۲�ķ���,ˮƽ����hpr
	else if (m_type == 0) {
		m_type = 0;
		m_hpr = obj_dir;
	}
	//�ڶ��������ǹ۲�ķ���
	else if (m_type == 1) {
		m_type = 1;
		m_eyedir = obj_dir;
	}
	glPushMatrix();
	glLoadIdentity();
	ShowView();
	glPopMatrix();
	if (type >= 0) m_type = type;
	CglMath::Dir2HPR(m_eyedir, m_updir, m_hpr);
}

//�����������
void CglCamera::followCamera(const CglVector3& pos, const CglVector3& obj_pos, CglVector3& updir) {
	m_pos = pos;
	m_updir = updir; 
	m_updir.Normalize();
	m_eyedir = obj_pos - pos; 
	m_eyedir.Normalize();
	
	CglMatrix eyeCm;
	CglVector3 xdir = m_eyedir.crossMul(m_updir); 
	xdir.Normalize();
	CglVector3 ydir = xdir.crossMul(m_eyedir); 
	ydir.Normalize();
	eyeCm.SetCol(0, xdir);
	eyeCm.SetCol(1, ydir);
	eyeCm.SetCol(2, -m_eyedir);
	m_hpr = eyeCm.ToEuler();
}

void CglCamera::SaveCamera() {
	int i;
	for (i = 0; i<16; i++) {
		m_viewMatrixSave[i] = m_viewMatrix[i];
	}
}

double* CglCamera::Slerp(double m_toMatrix[16], int n, double *t, CglQuaternion *Result, CglVector3 *pos) {
	double m_toMatrixInverse[16];
	//��������,��¼�����յ�λ��
	CglVector3 startPos(m_viewMatrixInverse[12], m_viewMatrixInverse[13], m_viewMatrixInverse[14]);
	CglMath::InverseMatrix(m_toMatrix, m_toMatrixInverse);
	CglVector3 targetPos(m_toMatrixInverse[12], m_toMatrixInverse[13], m_toMatrixInverse[14]);
	//����ת��Ԫ��
	CglQuaternion startQuater = CglMatrix(m_viewMatrixInverse).ToQuaternion();
	CglQuaternion targetQuater = CglMatrix(m_toMatrixInverse).ToQuaternion();
	//���ֵ
	startQuater.Slerp(targetQuater, n, t, Result);
	//�ָ��ӵ�λ��
	for (int i = 0; i < n; i++) {
		pos[i] = (targetPos - startPos)*t[i] + startPos;
	}

	return m_toMatrix;
}

void CglCamera::LoadCamera() {
	int i;
	for (i = 0; i<16; i++) {
		m_viewMatrix[i] = m_viewMatrixSave[i];
	}
	CglMath::InverseMatrix(m_viewMatrix, m_viewMatrixInverse);
	m_pos = m_viewMatrixInverse*CglVector3(0, 0, 0);
	m_eyedir = m_viewMatrixInverse*CglVector3(0, 0, -1) - m_pos;
	m_updir = m_viewMatrixInverse*CglVector3(0, 1, 0) - m_pos;
	CglMath::Dir2HPR(m_eyedir, m_updir, m_hpr);
}

void CglCamera::SetType(int type) {
	m_type = type;
	m_eyedir = m_viewMatrixInverse*CglVector3(0, 0, -1) - m_pos;
	m_updir = m_viewMatrixInverse*CglVector3(0, 1, 0) - m_pos;
	CglMath::Dir2HPR(m_eyedir, m_updir, m_hpr);
}
