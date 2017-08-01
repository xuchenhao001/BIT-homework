#include "stdafx.h"
#include "CglPatch.h"

CglPatch::CglPatch() {
	m_color[0] = m_color[1] = m_color[2] = m_color[3] = 1;
	m_particleDir[1].Set(0, 1, 0);
	m_size = 1;
	m_XYratio = 2;
	//m_numParticle = num;
	m_bActive = false;
	m_particleMoveDir.Set(0, 1, 0);
	m_dSpeed = 1;

	m_lasttime = GetTickCount();
}

CglPatch::~CglPatch() {

}

void CglPatch::Init(vector<CglPatch>* patchList, int num) {
	m_numParticle = num;
	while (num--) {
		CglPatch* patch = new CglPatch();
		patchList->push_back(*patch);
	}
}

void CglPatch::Update(vector<CglPatch>* patchList) {
	int curtime = GetTickCount();
	float usetime = curtime - m_lasttime;
	m_lasttime = curtime;

	patchPos += patchDir * usetime * patchSpeed;
	m_matrix.SetTrans(patchPos);
	vector<CglPatch>::iterator it;
	for (it = patchList->begin(); it != patchList->end();it++) {
		if (it->m_bActive) {//活的粒子
			it->m_lifetime -= 1;
			if (it->m_lifetime <= 0)
				it->m_bActive = false;
			it->m_dSpeed += usetime*it->m_acc;
			if (it->m_dSpeed<0)
				it->m_dSpeed = 0;
			it->m_pos += CglVector3((double(rand()) / RAND_MAX) - 0.8, (double(rand()) / RAND_MAX) - 0.8, (double(rand()) / RAND_MAX) - 0.8) * usetime * it->m_dSpeed / 1000;
			it->m_color[3] -= it->m_colorfade;
		}
		it->BillBoard(CglVector3(0, 0, 0));
	}
}

void CglPatch::Draw(vector<CglPatch>* patchList) {
	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//打开混合
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDepthMask(GL_FALSE);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glMultMatrixd(m_matrix);
	glColor3f(1, 1, 1);

	vector<CglPatch>::iterator it;
	for (it = patchList->begin(); it != patchList->end(); it++) {
		if (it->m_bActive)//活的粒子
			it->DrawPatch();
	}

	glPopMatrix();
	glPopAttrib();
}

void CglPatch::Reset(vector<CglPatch>* patchList, CglVector3 pos, CglVector3 dir, double speed) {
	patchPos = pos;
	patchDir = dir;
	patchSpeed = speed;
	m_matrix.SetTrans(patchPos);
	vector<CglPatch>::iterator it;
	for (it = patchList->begin(); it != patchList->end(); it++) {
		it->m_bActive = true;
		it->m_pos.Set(0, 0, 0);//初始位置
		it->m_lifetime = 20;//粒子生命周期
		double seta = double(rand()) * 2 * PAI / RAND_MAX;
		double beta = double(rand())*PAI / 30 / RAND_MAX;
		it->m_particleMoveDir[0] = sin(beta)*cos(seta);
		it->m_particleMoveDir[1] = cos(beta);
		it->m_particleMoveDir[2] = sin(beta)*sin(seta);

		//粒子运动速度
		it->m_dSpeed = 110;
		//粒子运动加速度
		it->m_acc = -0.6;
		//粒子的运动方向
		it->m_particleDir[1].Set((double(rand()) / RAND_MAX) - 0.5, (double(rand()) / RAND_MAX) - 0.5, (double(rand()) / RAND_MAX) - 0.5);
		it->m_size = 0.5;
		it->m_color[3] = 0.1;
		it->m_colorfade = it->m_color[3] / it->m_lifetime;
	}
}

//============
//单个粒子部分
//============

void CglPatch::BillBoard(const CglVector3 &eyepos) {
	m_particleDir[0] = eyepos - m_pos;
	m_particleDir[0].Normalize();
	m_particleDir[2] = m_particleDir[0].crossMul(m_particleDir[1]);
	m_particleDir[3] = m_particleDir[2].crossMul(m_particleDir[0]);
	CglVector3 posleft = m_pos + m_particleDir[2] * m_size;
	CglVector3 posright = m_pos - m_particleDir[2] * m_size;
	m_posCorner[0] = posleft + m_particleDir[3] * m_size*m_XYratio;
	m_posCorner[1] = posleft - m_particleDir[3] * m_size*m_XYratio;
	m_posCorner[2] = posright + m_particleDir[3] * m_size*m_XYratio;
	m_posCorner[3] = posright - m_particleDir[3] * m_size*m_XYratio;
}

void CglPatch::DrawPatch() {
	glColor4fv(m_color);
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(0, 0);	glVertex3dv(m_posCorner[0]);
	glTexCoord2f(0, 1);	glVertex3dv(m_posCorner[1]);
	glTexCoord2f(1, 0);	glVertex3dv(m_posCorner[2]);
	glTexCoord2f(1, 1);	glVertex3dv(m_posCorner[3]);
	glEnd();
}
