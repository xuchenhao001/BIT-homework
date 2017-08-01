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
	double step[2];		//�ƶ�����ת�������ű���
	float key_step;		//���̰���һ���˶���������ƶ�����

	int r_dir;		//��ת����
	int r_v;		//��ת�ٶ�
	int point;		//����
	int status;		//0״̬����Ƭ��1״̬��������
	int visible;    //0��ʾ����Ƭ��1��ʾ��������
	CglVector3 m_pos, m_dir, m_updir;
	CglMatrix m_matrix;
};
