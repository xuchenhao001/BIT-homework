
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
	GLfloat sx = 0, sy = 1, sz = 1;		//本例子中 sx,sy作为调节贴图位置的变量
	GLfloat(*vert)[3] = 0, (*norm)[3] = 0;
	//使用三角形绘制，填充绘制，使用光照，计算点法线
	int nVert = 0;
	int visit[10001];
	GLfloat z[3][10010];
	GLuint texture[1] = { 0 };//定义纹理
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
	void DrawBall(float radius, int lon, int lat);//使用经度分割lon,维度分割lat,半径radius绘制一个球。
};