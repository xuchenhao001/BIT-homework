// 图形学3_1120142053_徐晨浩.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#pragma comment(lib,"glew32.lib ")
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "math.h"
#include "MyMatrixVector.h"

using namespace std;

//====================================
//全局变量定义部分(可依据情况适当修改)
//====================================

int width = 1280, height = 720;//星星绘制窗口默认大小为1280x720，运行后可任意更改
const int STAR_NUM = 10000;//星星总数量，默认为10000个

//==========================
//全局变量定义部分(不可更改)
//==========================

//视点位置和方向
float mx = 0, my = 500, mz = 1000, rx = -25, ry = 0, rz = 0;//平移和旋转
float sx = 1, sy = 1, sz = 1;//缩放
float mspeed = 5, rspeed = 1;
float g_IEyeMat[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 }, g_EyeMat[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
int mode = 1;

CVector3 stars[STAR_NUM];

//================
//绘制函数实现部分
//================

//初始化星星函数
void initStar() {
	for (int i = 0; i < STAR_NUM; i++) {
		stars[i].x = rand() / double(RAND_MAX) * width * 32 - width * 16;
		stars[i].y = rand() / double(RAND_MAX) * height * 32 - height * 16;
		stars[i].z = rand() / double(RAND_MAX) * 32000 - 16000;
	}
}

//键盘控制函数
void myKeyboardFunc(unsigned char key, int x, int y)
{
	bool bChange = false;
	switch (key)
	{
	case 'w':
		//my+=mspeed;
		if (mode == 0)
		{
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(0, -mspeed, 0);
			glMultMatrixf(g_EyeMat);
			glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			glPopMatrix();
		}
		else
		{
			mx += g_IEyeMat[4] * mspeed;
			my += g_IEyeMat[5] * mspeed;
			mz += g_IEyeMat[6] * mspeed;
		}
		break;
	case 's':
		//my-=mspeed;	
		if (mode == 0)
		{
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(0, mspeed, 0);
			glMultMatrixf(g_EyeMat);
			glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			glPopMatrix();
		}
		else
		{
			mx -= g_IEyeMat[4] * mspeed;
			my -= g_IEyeMat[5] * mspeed;
			mz -= g_IEyeMat[6] * mspeed;
		}

		break;
	case 'a':
		//mx-=mspeed;
		if (mode == 0)
		{
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(mspeed, 0, 0);
			glMultMatrixf(g_EyeMat);
			glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			glPopMatrix();
		}
		else
		{
			mx -= g_IEyeMat[0] * mspeed;
			my -= g_IEyeMat[1] * mspeed;
			mz -= g_IEyeMat[2] * mspeed;
		}

		break;
	case 'd':
		//mx+=mspeed;
		if (mode == 0)
		{
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(-mspeed, 0, 0);
			glMultMatrixf(g_EyeMat);
			glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			glPopMatrix();
		}
		else
		{
			mx += g_IEyeMat[0] * mspeed;
			my += g_IEyeMat[1] * mspeed;
			mz += g_IEyeMat[2] * mspeed;
		}

		break;
	case 'q':
		if (mode == 0)
		{
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(0, 0, mspeed);
			glMultMatrixf(g_EyeMat);
			glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			glPopMatrix();
		}
		else
		{
			mx -= g_IEyeMat[8] * mspeed;
			my -= g_IEyeMat[9] * mspeed;
			mz -= g_IEyeMat[10] * mspeed;
		}
		//mz-=mspeed;

		break;
	case 'e':
		if (mode == 0)
		{
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(0, 0, -mspeed);
			glMultMatrixf(g_EyeMat);
			glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			glPopMatrix();
		}
		else
		{
			mx += g_IEyeMat[8] * mspeed;
			my += g_IEyeMat[9] * mspeed;
			mz += g_IEyeMat[10] * mspeed;
		}
		//mz+=mspeed;

		break;
	case 'i':
		if (mode == 0)
		{
			glPushMatrix();
			glLoadIdentity();
			glRotatef(-rspeed, 1, 0, 0);
			glMultMatrixf(g_EyeMat);
			glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			glPopMatrix();
		}
		else
		{
			rx += rspeed;
			bChange = true;
		}
		break;
	case 'k':
		if (mode == 0)
		{
			glPushMatrix();
			glLoadIdentity();
			glRotatef(rspeed, 1, 0, 0);
			glMultMatrixf(g_EyeMat);
			glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			glPopMatrix();
		}
		else
		{
			rx -= rspeed;
			bChange = true;
		}

		break;
	case 'j':
		if (mode == 0)
		{
			glPushMatrix();
			glLoadIdentity();
			glRotatef(-rspeed, 0, 1, 0);
			glMultMatrixf(g_EyeMat);
			glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			glPopMatrix();
		}
		else
		{
			ry += rspeed;
			bChange = true;
		}

		break;
	case 'l':
		if (mode == 0)
		{
			glPushMatrix();
			glLoadIdentity();
			glRotatef(rspeed, 0, 1, 0);
			glMultMatrixf(g_EyeMat);
			glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			glPopMatrix();
		}
		else
		{
			ry -= rspeed;
			bChange = true;
		}

		break;
	case 'u':
		if (mode == 0)
		{
			glPushMatrix();
			glLoadIdentity();
			glRotatef(rspeed, 0, 0, 1);
			glMultMatrixf(g_EyeMat);
			glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			glPopMatrix();
		}
		else
		{
			rz += rspeed;
			bChange = true;
		}

		break;
	case 'o':
		if (mode == 0)
		{
			glPushMatrix();
			glLoadIdentity();
			glRotatef(-rspeed, 0, 0, 1);
			glMultMatrixf(g_EyeMat);
			glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			glPopMatrix();
		}
		else
		{
			rz -= rspeed;
			bChange = true;
		}

		break;
	case '=':
		mspeed *= 1.1;
		printf("mspeed:%.1f\n", mspeed);
		break;
	case '-':
		mspeed *= 0.9;
		printf("mspeed:%.1f\n", mspeed);
		break;
	case ' ':
		mode = 1 - mode;
		if (mode == 0)
		{
			glPushMatrix();
			glLoadIdentity();
			glRotatef(-rz, 0, 0, 1);
			glRotatef(-rx, 1, 0, 0);
			glRotatef(-ry, 0, 1, 0);
			glTranslatef(-mx, -my, -mz);
			glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			glPopMatrix();
		}
		printf("mode:%d\n", mode);
		break;
	}
	if (bChange)//计算视点矩阵的逆矩阵
	{
		glPushMatrix();
		glLoadIdentity();
		glRotatef(ry, 0, 1, 0);
		glRotatef(rx, 1, 0, 0);
		glRotatef(rz, 0, 0, 1);
		glGetFloatv(GL_MODELVIEW_MATRIX, g_IEyeMat);
		glPopMatrix();
	}
}

//设置视点
void SetView()
{
	if (mode == 0)
	{
		glLoadMatrixf(g_EyeMat);
	}
	else
	{
		glRotatef(-rz, 0, 0, 1);
		glRotatef(-rx, 1, 0, 0);
		glRotatef(-ry, 0, 1, 0);
		glTranslatef(-mx, -my, -mz);
	}
}

//一闪一闪画星星
void drawStar() {
	//glTranslatef(0, 0, -1000);
	
	glPointSize(2);
	glBegin(GL_POINTS);
	for (int i = 0; i < STAR_NUM; i++) {
		glVertex3f(stars[i].x, stars[i].y, stars[i].z);
	}
	glEnd();
}

//绘制图形
void RenderWorld()
{
	//绘制坐标轴
	//glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(1000, 0, 0);
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1000, 0);
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1000);
	glEnd();
	glColor3f(1, 1, 1);
	//画星星
	drawStar();
}

//图形总绘制
void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	SetView();
	RenderWorld();
	glPopMatrix();
	glutSwapBuffers();
}

//调整窗口大小函数
void myReshape(int w, int h)
{
	width = w;
	height = h;
	GLfloat nRange = 100.0f;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, GLfloat(w) / h, 1, 1000000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//设置环境
void SetRC()
{
	//星星是圆的
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glShadeModel(GL_FLAT);
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_BACK, GL_LINE);

	glewInit();
	float Quadratic[] = { 0,0,0.00001 };
	//glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, Quadratic);
	glPointParameterf(GL_POINT_SIZE_MIN, 1);//设置最小点
	glPointParameterf(GL_POINT_SIZE_MAX, 100);//设置最大点
}

//时间控制
void myTimerFunc(int val)
{
	myDisplay();
	glutTimerFunc(1, myTimerFunc, 0);
}

//================
//文件读写计算部分
//================

//矩阵读取函数
void readMatrix(CMatrix & cm, string line) {
	sscanf_s(line.c_str(), "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f", \
		&cm[0], &cm[1], &cm[2], &cm[3], &cm[4], &cm[5], &cm[6], &cm[7], \
		&cm[8], &cm[9], &cm[10], &cm[11], &cm[12], &cm[13], &cm[14], &cm[15]);
}

//矩阵写入函数
void readVector(CVector3 & cv, string line) {
	sscanf_s(line.c_str(), "%f,%f,%f", &cv[0], &cv[1], &cv[2]);
}

//对test文本的读取，计算，写入out.txt操作
void compute() {
	ifstream in("test.txt");
	ofstream out("out.txt", ios::trunc);
	string line;
	CMatrix cma, cmb, cmc;
	CVector3 cva, cvb;

	in.imbue(locale("chs"));
	out << setiosflags(ios::fixed);

	if (in) {
		while (getline(in, line)) {
			if (line == "矩阵相乘") {
				out << line << endl;
				getline(in, line);
				out << line;
				readMatrix(cma, line);
				readMatrix(cmb, line.substr(line.find("\t")));
				cmc = cma*cmb;
				out << "\t" << cmc.toString() << endl;
			}
			else if (line == "矩阵乘向量") {
				out << line << endl;
				getline(in, line);
				out << line;
				readMatrix(cma, line);
				readVector(cva, line.substr(line.find("\t")));
				cvb = cma.vecMul(cva);
				out << "\t" << cvb.toString() << endl;
			}
			else if (line == "矩阵乘位置") {
				out << line << endl;
				getline(in, line);
				out << line;
				readMatrix(cma, line);
				readVector(cva, line.substr(line.find("\t")));
				cvb = cma.posMul(cva);
				out << "\t" << cvb.toString() << endl;
			}
			else if (line == "矩阵设置旋转") {
				out << line << endl;
				getline(in, line);
				out << line;
				float seta;
				readMatrix(cma, line);
				line = line.substr(line.find("\t") + 1);
				sscanf_s(line.c_str(), "%f", &seta);
				line = line.substr(line.find("\t") + 1);
				readVector(cva, line);
				cmb = cma.SetRotate(seta, cva);
				out << "\t" << cmb.toString() << endl;
			}
			else if (line == "矩阵设置平移") {
				out << line << endl;
				getline(in, line);
				out << line;
				readMatrix(cma, line);
				readVector(cva, line.substr(line.find("\t")));
				cmb = cma.SetTrans(cva);
				out << "\t" << cmb.toString() << endl;
			}
			else if (line == "矩阵设置缩放") {
				out << line << endl;
				getline(in, line);
				out << line;
				readMatrix(cma, line);
				line = line.substr(line.find("\t") + 1);
				readVector(cva, line);
				cmb = cma.SetScale(cva);
				out << "\t" << cmb.toString() << endl;
			}
			else if (line == "矩阵求逆") {
				out << line << endl;
				getline(in, line);
				out << line;
				readMatrix(cma, line);
				cma.Inverse();
				out << "\t" << cma.toString() << endl;
			}
		}
		out.close();
	}
	else {
		printf("未找到test.txt文件！\n");
		system("Pause");
		exit(1);
	}
}

//==========
//主控制函数
//==========
int main(int argc, char *argv[])
{
	//compute();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("图形学3_1120142053_徐晨浩");
	initStar();
	glutDisplayFunc(&myDisplay);
	glutTimerFunc(1, myTimerFunc, 0);
	glutReshapeFunc(&myReshape);
	glutKeyboardFunc(&myKeyboardFunc);
	SetRC();
	glutMainLoop();
	return 0;
}

