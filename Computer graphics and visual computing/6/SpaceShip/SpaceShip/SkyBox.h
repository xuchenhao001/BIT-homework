
#pragma once
#include "stdafx.h"
#include "stdio.h"
#include "math.h"
#include <gl\glaux.h>
#include <GL/glut.h>
#include "opengl/CglShape.h"

#define PI 3.14159

#pragma comment(lib,"glaux.lib ")
#include "string.h"

class SkyBox {
public:
	GLfloat sx = 0, sy = 1, sz = 1;		//�������� sx,sy��Ϊ������ͼλ�õı���
	GLfloat(*vert)[3] = 0, (*norm)[3] = 0;
	//ʹ�������λ��ƣ������ƣ�ʹ�ù��գ�����㷨��
	int nVert = 0;
	int visit[10001];
	GLfloat z[3][10010];
	GLuint texture[1] = { 0 };//��������
	GLfloat mspeed = 0.5, rspeed = 1;
public:
	SkyBox();
	~SkyBox();

	void Initial();
	void scatter();
	void Font2D(char * str, double x, double y, int type);
	void SetLight();
	void CalculateNormal(GLfloat v1[], GLfloat v2[], GLfloat v3[], GLfloat normal[]);
	void normalize(GLfloat *v);
	void DrawBall(float radius, int lon, int lat);//ʹ�þ��ȷָ�lon,ά�ȷָ�lat,�뾶radius����һ����
};