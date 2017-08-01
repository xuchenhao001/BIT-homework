#include "stdafx.h"
#include "CglParticleCtrl.h"
#include "OpenGL.h"


CglParticleCtrl::CglParticleCtrl() {

}


CglParticleCtrl::~CglParticleCtrl() {

}

//================
//尾焰粒子控制部分
//================

void CglParticleCtrl::InitTail(int num, char* materialName) {
	m_numParticle = num;
	int i;
	m_lasttime = GetTickCount();
	srand(m_lasttime);
	for (i = 0; i<m_numParticle; i++) {
		ResetTail(i,true);
	}
	//加入粒子纹理
	CglMaterial material;
	tailTextureid = material.LoadGLTextures(materialName);
}

void CglParticleCtrl::UpdateTail() {
	double mat[16], imat[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, mat);
	CglMath::InverseMatrix(mat, imat);
	CglVector3 eyepos = imat*CglVector3(0, 0, 0);
	int i;
	int curtime = GetTickCount();
	float usetime = curtime - m_lasttime;
	m_lasttime = curtime;
	for (i = 0; i<m_numParticle; i++) {
		if (particleTail[i].m_bActive) {//活的粒子
			particleTail[i].m_lifetime -= 1;
			if (particleTail[i].m_lifetime <= 0)
				particleTail[i].m_bActive = false;
			particleTail[i].m_dSpeed += usetime*particleTail[i].m_acc;
			if (particleTail[i].m_dSpeed<0)
				particleTail[i].m_dSpeed = 0;
			particleTail[i].m_pos += usetime*particleTail[i].m_particleMoveDir*particleTail[i].m_dSpeed / 1000;
			particleTail[i].m_color[3] -= particleTail[i].m_colorfade;
		} else {//重新激活粒子
			ResetTail(i);
		}
		particleTail[i].BillBoard(eyepos);
	}
}

void CglParticleCtrl::DrawTail() {
	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//打开混合
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDepthMask(GL_FALSE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tailTextureid);
	for (int i = 0; i<m_numParticle; i++) {
		if (particleTail[i].m_bActive)//活的粒子
			particleTail[i].Draw();
	}
	glPopAttrib();
}

void CglParticleCtrl::ResetTail(int id, bool bFirst) {
	particleTail[id].m_bActive = true;
	particleTail[id].m_pos.Set(0, 0, 0);//初始位置

	particleTail[id].m_lifetime = (double(rand()) / RAND_MAX) * 20;
	double seta = double(rand()) * 2 * PAI / RAND_MAX;
	double beta = double(rand())*PAI / 30 / RAND_MAX;
	particleTail[id].m_particleMoveDir[0] = sin(beta)*cos(seta);
	particleTail[id].m_particleMoveDir[1] = cos(beta);
	particleTail[id].m_particleMoveDir[2] = sin(beta)*sin(seta);

	particleTail[id].m_dSpeed = double(rand()) / RAND_MAX * 90 + 20;		//粒子运动速度
	particleTail[id].m_acc = -double(rand()) / RAND_MAX*0.2 - 0.4;
	particleTail[id].m_size = 0.5;
	particleTail[id].m_color[3] = 0.1;
	particleTail[id].m_colorfade = particleTail[id].m_color[3] / particleTail[id].m_lifetime;
}