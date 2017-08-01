#pragma once
#include "stdafx.h"
#include "opengl/OpenGL.h"
#include "CglGarbage.h"
#pragma comment(lib,"opengl/MyOpenGL")
#include <GL/glut.h>
#ifndef M_PI
#  define M_PI	(double)3.14159265358979323846
#endif /* !M_PI */

static GLfloat sunpos[4] = { 0.7071, 0.7071, 0.0, 0.0 };
static GLfloat suncolor[4] = { 0.5, 0.5, 0.4, 1.0 };
static GLfloat sunambient[4] = { 0.5, 0.5, 0.4, 1.0 };

class CglPlane {
public:
	CglPlane::CglPlane();
	CglPlane::~CglPlane();
	void   Init(void);
	void   Draw(void);

	float Move(int dir, double plus_or_minus, float usetime);
	void Rotate(int dir, double plus_or_minus, float usetime);
	void SetSpeed(float mspeed, float rspeed);
	void SetDir(CglVector3 dir);
	float GetMSpeed();
	float GetRSpeed();
	int GetUseTime();

	int lasttime = -1;
	double scale_step[2];		//�ƶ�����ת�������ű���
	float key_step;				//���̰���һ���˶���������ƶ�����
	int score;				//�ɻ���ǰ�ķ���
	float speed_Max;			//�ɻ�������ٶ�
	float range;				//�ɻ����ֻ���Χ

public:
	CglFont *m_pFont;
	CString str;
	GLuint     F16Body,           /* F-16 body */
		F16Rolleron[2];    /* F-16 rollerons */

	CglVector3 m_pos;
	CglVector3 m_dir;
	CglVector3 m_updir;
	CglMatrix  m_matrix;
	CglParticleCtrl m_particle;
};
