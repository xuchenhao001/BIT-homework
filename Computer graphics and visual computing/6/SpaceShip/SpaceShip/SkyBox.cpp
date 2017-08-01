// 001.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include "math.h"
#include"SkyBox.h"

SkyBox::SkyBox() {
	Initial();
}

SkyBox::~SkyBox() {
	delete[]vert;
	delete[]norm;
}


void SkyBox::Initial()
{
	int i;
	for (i = 0; i < 10010; i++)
	{
		z[0][i] = 2.0*(0.5 - rand() / double(RAND_MAX));//生成10 000个点的x轴坐标
		z[1][i] = 2.0*(0.5 - rand() / double(RAND_MAX));//生成10 000个点的y轴坐标
		z[2][i] = 2.0*(0.5 - rand() / double(RAND_MAX));//生成10 000个点的z轴坐标
	}

	memset(visit, 0, sizeof(visit));
	visit[0] = 1;

}
void SkyBox::scatter()
{
	int i;
	GLfloat pointSize = 5.4f;//设置点的大小为5.4f
	glPointSize(pointSize);
	glBegin(GL_POINTS);
	for (i = 0; i < 10011; i++)
	{
		float temp = rand() / double(RAND_MAX);//随机亮度
		glColor4f(0.0f, 0.0f, 1.0f, temp);
		glVertex3f(z[0][i], z[1][i], z[2][i]);//生成点
	}
	glEnd();
}
void SkyBox::Font2D(char * str, double x, double y, int type)
{
	//设置投影方式：平行投影
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	//输出字符串
	int len = strlen(str);
	glRasterPos2f(x, y);
	for (int i = 0; i<len; ++i)
	{
		switch (type) {
		case 1:
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str[i]);
			break;
		case 2:
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
			break;
		case 3:
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, str[i]);
			break;
		case 4:
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
			break;
		case 5:
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, str[i]);
			break;
		case 6:
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i]);
			break;
		case 7:
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
			break;
		}
	}
	//恢复投影方式
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void SkyBox::SetLight()
{
	GLfloat amb[4] = { 0.2, 0.2, 0.2, 1 };
	GLfloat dif[4] = { 1, 1, 1, 1 };
	GLfloat spe[4] = { 0.2, 0.2, 0.2, 1 };
	GLfloat pos[4] = { 10, 10, 10, 1 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spe);
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);
	glColor3f(1, 0, 0);
	glutSolidSphere(0.5, 10, 10);
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void SkyBox::CalculateNormal(GLfloat v1[], GLfloat v2[], GLfloat v3[], GLfloat normal[])
{
	GLfloat dirv2_v1[3], dirv2_v3[3];
	for (int i = 0; i<3; i++)
	{
		dirv2_v1[i] = v1[i] - v2[i];
		dirv2_v3[i] = v3[i] - v2[i];
	}
	//叉乘计算法线方向
	normal[0] = dirv2_v1[1] * dirv2_v3[2] - dirv2_v1[2] * dirv2_v3[1];
	normal[1] = dirv2_v1[2] * dirv2_v3[0] - dirv2_v1[0] * dirv2_v3[2];
	normal[2] = dirv2_v1[0] * dirv2_v3[1] - dirv2_v1[1] * dirv2_v3[0];
}
void SkyBox::normalize(GLfloat *v)
{
	GLfloat dis = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	if (dis>0)
	{
		v[0] /= dis;
		v[1] /= dis;
		v[2] /= dis;
	}
}

//使用经度分割lon,维度分割lat,半径radius绘制一个球。
void SkyBox::DrawBall(float radius, int lon, int lat) {
	glPushMatrix();
	glColor3f(1, 1, 1);
	GLUquadricObj *quadPlanet;
	quadPlanet = gluNewQuadric();
	gluQuadricTexture(quadPlanet, GL_TRUE);
	gluSphere(quadPlanet, radius, 30, 30);
	glPopMatrix();
}
