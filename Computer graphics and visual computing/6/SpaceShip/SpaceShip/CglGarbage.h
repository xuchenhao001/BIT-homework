#pragma once

#include "stdafx.h"
#include "opengl/OpenGL.h"
#pragma comment(lib,"opengl/MyOpenGL")
#include"opengl/CglParticle.h"
#include "CglPatch.h"
#include <vector>

using namespace std;

class CglGarbage {
public:
	CglGarbage::CglGarbage();
	CglGarbage::~CglGarbage();
	void draw(float size, CglPatch patch, vector<CglPatch> *patchList);

	float Move(int dir, double plus_or_minus, float usetime);
	void Rotate(int dir, double plus_or_minus, float usetime);
	void SetDir(CglVector3 dir);
	float GetMSpeed();
	float GetRSpeed();
	int GetUseTime();

	int lasttime = -1;
	double step[2];		//移动和旋转步长缩放倍率
	float key_step;		//键盘按下一个运动命令代表移动步长

	int r_dir;		//旋转方向
	int r_v;		//旋转速度
	int point;		//分数
	int status;		//0状态是碎片，1状态是垃圾块
	int visible;    //0显示是碎片，1显示是垃圾块
	CglVector3 m_pos, m_dir, m_updir;
	CglMatrix m_matrix;
};
