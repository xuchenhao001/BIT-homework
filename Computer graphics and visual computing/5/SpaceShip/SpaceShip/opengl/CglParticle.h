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
	CglVector3 m_pos;	//��������λ��
	double m_size;			//���ӵĴ�С
	double m_XYratio;		//���ӵĳ������
	CglVector3 m_particleDir[4];	//��������billboardλ��ʱ�õ��ķ���
	CglVector3 m_posCorner[4];		//���ӽǵ������
	float m_color[4];	//���ӵ���ɫ
	bool m_bActive;		//�����Ƿ���
	CglVector3 m_particleMoveDir;//���ӵ��˶�����
	double m_dSpeed;		//�����˶��ٶ�
	double m_lifetime;		//��������ֵ
	double m_acc;			//���ٶ�
	double m_colorfade;
};

