#pragma once

#include "CglVector3.h"

class CglParticleCtrl;

class AFX_API_EXPORT CglParticle {
public:
	CglParticle();
	~CglParticle();

	void BillBoard(const CglVector3 &eyepos);
	void Draw();

	friend class CglParticleCtrl;

private:
	CglVector3 m_pos;	//粒子中心位置
	double m_size;			//粒子的大小
	double m_XYratio;		//粒子的长宽比例
	CglVector3 m_particleDir[4];	//计算粒子billboard位置时用到的方向
	CglVector3 m_posCorner[4];		//粒子角点的坐标
	float m_color[4];	//粒子的颜色
	bool m_bActive;		//粒子是否存活
	CglVector3 m_particleMoveDir;//粒子的运动方向
	double m_dSpeed;		//粒子运动速度
	double m_lifetime;		//粒子生命值
	double m_acc;			//加速度
	double m_colorfade;
};

