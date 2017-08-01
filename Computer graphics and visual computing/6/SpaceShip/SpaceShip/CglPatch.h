#pragma once
#include "stdafx.h"
#include "opengl/OpenGL.h"
#pragma comment(lib,"opengl/MyOpenGL")
#include"opengl/CglParticle.h"
#include<vector>

using namespace std;

class CglPatch {
public:
	CglPatch::CglPatch();
	CglPatch::~CglPatch();

	void Init(vector<CglPatch>* patchList, int num);
	void Update(vector<CglPatch>* patchList);
	void Draw(vector<CglPatch>* patchList);
	void CglPatch::Reset(vector<CglPatch>* patchList, CglVector3 pos, CglVector3 dir, double speed);

//private:
	int m_lasttime, m_numParticle;
	double m_size;//粒子的大小
	double m_XYratio;//粒子的长宽比例
	CglVector3 m_posCorner[4];//粒子角点的坐标
	float m_color[4];//粒子的颜色
	double m_colorfade;
	bool m_bActive;//粒子是否存活
	double m_lifetime;//粒子生命值
	CglVector3 m_pos;//粒子位置
	CglVector3 m_particleMoveDir;//粒子的运动方向
	double m_dSpeed;//粒子运动速度
	double m_acc;//加速度
	CglVector3 m_particleDir[4];

	//碎片整体参量
	CglMatrix m_matrix;
	CglVector3 patchPos, patchDir;
	double patchSpeed;

	void BillBoard(const CglVector3 &eyepos);
	void DrawPatch();
};
