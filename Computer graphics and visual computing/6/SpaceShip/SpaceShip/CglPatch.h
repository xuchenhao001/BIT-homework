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
	double m_size;//���ӵĴ�С
	double m_XYratio;//���ӵĳ������
	CglVector3 m_posCorner[4];//���ӽǵ������
	float m_color[4];//���ӵ���ɫ
	double m_colorfade;
	bool m_bActive;//�����Ƿ���
	double m_lifetime;//��������ֵ
	CglVector3 m_pos;//����λ��
	CglVector3 m_particleMoveDir;//���ӵ��˶�����
	double m_dSpeed;//�����˶��ٶ�
	double m_acc;//���ٶ�
	CglVector3 m_particleDir[4];

	//��Ƭ�������
	CglMatrix m_matrix;
	CglVector3 patchPos, patchDir;
	double patchSpeed;

	void BillBoard(const CglVector3 &eyepos);
	void DrawPatch();
};
