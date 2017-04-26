#pragma once

#include "CglParticle.h"
#include "gl\glut.h"

class AFX_API_EXPORT CglParticleCtrl {
public:
	CglParticleCtrl();
	~CglParticleCtrl();

	void Draw();
	void UpdatePos();	//更新粒子的位置
	void Init(int num);

private:
	void ResetParticle(int id, bool bFirst = false);	//设置粒子的初始化状态
	CglParticle m_particle[10000];	//预生成一定的粒子
	int m_numParticle;//仿真用的粒子个数
	GLuint m_textureid;//粒子纹理
	int m_lasttime;//记录粒子运行的上次时间
};

