#pragma once

#include "CglParticle.h"
#include "gl\glut.h"

class AFX_API_EXPORT CglParticleCtrl {
public:
	CglParticleCtrl();
	~CglParticleCtrl();

	//尾焰部分
	void DrawTail();
	void UpdateTail();							
	void InitTail(int num, char* materialName);

private:
	void ResetTail(int id, bool bFirst = false);	//设置尾焰的初始化状态
	CglParticle particleTail[1001];	//预生成尾焰粒子
	int m_numParticle;//仿真用的粒子个数
	GLuint tailTextureid;//尾焰纹理
	GLuint patchTextureid;//碎片纹理
	int m_lasttime;//记录粒子运行的上次时间
};

