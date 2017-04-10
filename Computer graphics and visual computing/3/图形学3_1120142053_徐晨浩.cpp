// ͼ��ѧ3_1120142053_�쳿��.cpp : �������̨Ӧ�ó������ڵ㡣
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
#include <queue>
#include "math.h"
#include "MyMatrixVector.h"
#include "ͼ��ѧ3_1120142053_�쳿��.h"

using namespace std;

//====================================
//ȫ�ֱ������岿��(����������ʵ��޸�)
//====================================

int width = 1280, height = 720;//���ǻ��ƴ���Ĭ�ϴ�СΪ1280x720�����к���������
const int STAR_NUM = 10000;//������������Ĭ��Ϊ10000��

//==========================
//ȫ�ֱ������岿��(���ɸ���)
//==========================

//ƽ�ƣ���ת������
CVector3 mpos(0, 500, 1000), rpos(-25, 0, 0), spos(1, 1, 1);

//�ɰ���Сβ��
cycleQueue<CVector3> tail(121);

float mspeed = 5, rspeed = 1;

//ʹ���Լ���д�ľ�����,����Ĭ�ϵĹ��캯�������쵥λ����
CMatrix g_IEyeMat, g_EyeMat;

//�ӵ�任��ʽ,1Ϊŷ���ӽ�,0Ϊ�����ӵ�任
int mode = 1;

CVector3 stars[STAR_NUM];

//================
//���ƺ���ʵ�ֲ���
//================

//��ʼ�����Ǻ���
void initStar() {
	for (int i = 0; i < STAR_NUM; i++) {
		stars[i].x = rand() / double(RAND_MAX) * width * 32 - width * 16;
		stars[i].y = rand() / double(RAND_MAX) * height * 32 - height * 16;
		stars[i].z = rand() / double(RAND_MAX) * 32000 - 16000;
	}
}

//���̿��ƺ���
void myKeyboardFunc(unsigned char key, int x, int y) {
	bool bChange = false;

	if (key == 'w' || key == 's' || key == 'a' || key == 'd' || key == 'q' || key == 'e') {
		if (tail.isFull()) {
			tail.pop();
		}
		tail.push(mpos);
		//printf("%f\t%f\t%f\t%d\n", mpos.x, mpos.y, mpos.z, tail.size());
	}

	switch (key)
	{
	case 'w':
		if (mode == 0) {
			CMatrix g, g1;
			g_EyeMat = g.SetTrans(CVector3(0, -mspeed, 0))*g_EyeMat;
			g1 =g_EyeMat.GetInverse();
			mpos.Set(g1.m03, g1.m13, g1.m23);
		}
		else {
			CVector3 EyeDir(g_IEyeMat[4], g_IEyeMat[5], g_IEyeMat[6]);
			mpos = mpos + EyeDir*mspeed;
		}
		break;
	case 's':
		if (mode == 0) {
			CMatrix g, g1;
			g_EyeMat = g.SetTrans(CVector3(0, mspeed, 0))*g_EyeMat;
			g1 = g_EyeMat.GetInverse();
			mpos.Set(g1.m03, g1.m13, g1.m23);
		}
		else {
			CVector3 EyeDir(g_IEyeMat[4], g_IEyeMat[5], g_IEyeMat[6]);
			mpos = mpos - EyeDir*mspeed;
		}
		break;
	case 'a':
		if (mode == 0) {
			CMatrix g, g1;
			g_EyeMat = g.SetTrans(CVector3(mspeed, 0, 0))*g_EyeMat;
			g1 = g_EyeMat.GetInverse();
			mpos.Set(g1.m03, g1.m13, g1.m23);
		}
		else {
			CVector3 EyeDir(g_IEyeMat[0], g_IEyeMat[1], g_IEyeMat[2]);
			mpos = mpos - EyeDir*mspeed;
		}
		break;
	case 'd':
		if (mode == 0) {
			CMatrix g, g1;
			g_EyeMat = g.SetTrans(CVector3(-mspeed, 0, 0))*g_EyeMat;
			g1 = g_EyeMat.GetInverse();
			mpos.Set(g1.m03, g1.m13, g1.m23);
		}
		else {
			CVector3 EyeDir(g_IEyeMat[0], g_IEyeMat[1], g_IEyeMat[2]);
			mpos = mpos + EyeDir*mspeed;
		}
		break;
	case 'q':
		if (mode == 0) {
			CMatrix g, g1;
			g_EyeMat = g.SetTrans(CVector3(0, 0, mspeed))*g_EyeMat;
			g1 = g_EyeMat.GetInverse();
			mpos.Set(g1.m03, g1.m13, g1.m23);
		}
		else {
			CVector3 EyeDir(g_IEyeMat[8], g_IEyeMat[9], g_IEyeMat[10]);
			mpos = mpos + EyeDir*mspeed;
		}
		break;
	case 'e':
		if (mode == 0) {
			CMatrix g, g1;
			g_EyeMat = g.SetTrans(CVector3(0, 0, -mspeed))*g_EyeMat;
			g1 = g_EyeMat.GetInverse();
			mpos.Set(g1.m03, g1.m13, g1.m23);
		}
		else {
			CVector3 EyeDir(g_IEyeMat[8], g_IEyeMat[9], g_IEyeMat[10]);
			mpos = mpos - EyeDir*mspeed;
		}
		break;
	case 'i':
		if (mode == 0) {
			CMatrix g;
			g_EyeMat = g.SetRotate(-rspeed, CVector3(1, 0, 0))*g_EyeMat;
		}
		else {
			rpos.x += rspeed;
			bChange = true;
		}
		break;
	case 'k':
		if (mode == 0) {
			CMatrix g;
			g_EyeMat = g.SetRotate(rspeed, CVector3(1, 0, 0))*g_EyeMat;
		}
		else {
			rpos.x -= rspeed;
			bChange = true;
		}
		break;
	case 'j':
		if (mode == 0) {
			CMatrix g;
			g_EyeMat = g.SetRotate(-rspeed, CVector3(0, 1, 0))*g_EyeMat;
		}
		else {
			rpos.y += rspeed;
			bChange = true;
		}
		break;
	case 'l':
		if (mode == 0) {
			CMatrix g;
			g_EyeMat = g.SetRotate(rspeed, CVector3(0, 1, 0))*g_EyeMat;
		}
		else {
			rpos.y -= rspeed;
			bChange = true;
		}
		break;
	case 'u':
		if (mode == 0) {
			CMatrix g;
			g_EyeMat = g.SetRotate(rspeed, CVector3(0, 0, 1))*g_EyeMat;
		}
		else {
			rpos.z += rspeed;
			bChange = true;
		}
		break;
	case 'o':
		if (mode == 0) {
			CMatrix g;
			g_EyeMat = g.SetRotate(-rspeed, CVector3(0, 0, 1))*g_EyeMat;
		}
		else {
			rpos.z -= rspeed;
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
		if (mode == 0){
			CMatrix g1, g2, g3, g4;
			g_EyeMat = g1.SetRotate(-rpos.z, CVector3(0, 0, 1))*g2.SetRotate(-rpos.x, CVector3(1, 0, 0))*g3.SetRotate(-rpos.y, CVector3(0, 1, 0))*g4.SetTrans(CVector3(-mpos.x, -mpos.y, -mpos.z));
		}
		printf("mode:%d\n", mode);
		break;
	}
	if (bChange)//�����ӵ����������
	{
		CMatrix g1, g2, g3;
		g_IEyeMat = g1.SetRotate(rpos.y, CVector3(0, 1, 0))*g2.SetRotate(rpos.x, CVector3(1, 0, 0))*g3.SetRotate(rpos.z, CVector3(0, 0, 1));
	}
}

//F2������
void mySpecialKeyboardFunc(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_F2:
		mode = 1 - mode;
		if (mode == 0) {
			CMatrix g1, g2, g3, g4;
			g_EyeMat = g1.SetRotate(-rpos.z, CVector3(0, 0, 1))*g2.SetRotate(-rpos.x, CVector3(1, 0, 0))*g3.SetRotate(-rpos.y, CVector3(0, 1, 0))*g4.SetTrans(CVector3(-mpos.x, -mpos.y, -mpos.z));
		}
		printf("mode:%d\n", mode);
		break;
	}
}

//���û���
void SetRC() {

	//������Բ��(����ƴ�С�任��ͻ����ע�ӵ���)
	//glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glShadeModel(GL_FLAT);
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_BACK, GL_LINE);

	//�������������ӵ�Զ���任��С
	glewInit();
	float Quadratic[] = { 0,0.1,0 };
	glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, Quadratic);
	glPointParameterf(GL_POINT_SIZE_MIN, 1);//������С��
	glPointParameterf(GL_POINT_SIZE_MAX, 100);//��������
}

//�����ӵ�
void SetView() {
	if (mode == 0)
	{
		glLoadMatrixf(g_EyeMat);
	}
	else
	{
		CMatrix g1, g2, g3, g4;
		(g1.SetRotate(-rpos.z, CVector3(0, 0, 1))*g2.SetRotate(-rpos.x, CVector3(1, 0, 0))*g3.SetRotate(-rpos.y, CVector3(0, 1, 0))*g4.SetTrans(CVector3(-mpos.x, -mpos.y, -mpos.z))).run();
	}
}

//������
void drawStar() {
	glPointSize(20);
	glBegin(GL_POINTS);
	for (int i = 0; i < STAR_NUM; i++) {
		glVertex3f(stars[i].x, stars[i].y, stars[i].z);
	}
	glEnd();
}

//��β��
void drawTail() {
	glPushMatrix();
	tail.reset();
	for (int i = 0; i < tail.size() - 1; i++) {
		float color;
		glLineWidth(1);
		//��β�ͳ��ȴﵽ120��ʱ�򣬿������20�׵�β�ͽ�����ʧ
		if (tail.size() == 120 && i < 20) {
			color = i / 20.0;
		}
		else {
			color = 1.0;
		}
		glColor3f(color, color, color);
		glBegin(GL_LINES);
		glVertex3f(tail.now().x, tail.now().y, tail.now().z);
		tail.next();
		glVertex3f(tail.now().x, tail.now().y, tail.now().z);
		glEnd();
	}
	glPopMatrix();
}

//ͼ���ܻ���
void myDisplay(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	SetView();
	//������
	drawStar();
	//��β��
	drawTail();

	//����������
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(1000, 0, 0);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1000, 0);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1000);
	glEnd();
	glPopMatrix();
	glutSwapBuffers();
}

//�������ڴ�С����
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

//ʱ�����
void myTimerFunc(int val)
{
	myDisplay();
	glutTimerFunc(1, myTimerFunc, 0);
}

//================
//�ļ���д���㲿��
//================

//�����ȡ����
void readMatrix(CMatrix & cm, string line) {
	sscanf_s(line.c_str(), "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f", \
		&cm[0], &cm[1], &cm[2], &cm[3], &cm[4], &cm[5], &cm[6], &cm[7], \
		&cm[8], &cm[9], &cm[10], &cm[11], &cm[12], &cm[13], &cm[14], &cm[15]);
}

//����д�뺯��
void readVector(CVector3 & cv, string line) {
	sscanf_s(line.c_str(), "%f,%f,%f", &cv[0], &cv[1], &cv[2]);
}

//��test�ı��Ķ�ȡ�����㣬д��out.txt����
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
			if (line == "�������") {
				out << line << endl;
				getline(in, line);
				out << line;
				readMatrix(cma, line);
				readMatrix(cmb, line.substr(line.find("\t")));
				cmc = cma*cmb;
				out << "\t" << cmc.toString() << endl;
			}
			else if (line == "���������") {
				out << line << endl;
				getline(in, line);
				out << line;
				readMatrix(cma, line);
				readVector(cva, line.substr(line.find("\t")));
				cvb = cma.vecMul(cva);
				out << "\t" << cvb.toString() << endl;
			}
			else if (line == "�����λ��") {
				out << line << endl;
				getline(in, line);
				out << line;
				readMatrix(cma, line);
				readVector(cva, line.substr(line.find("\t")));
				cvb = cma.posMul(cva);
				out << "\t" << cvb.toString() << endl;
			}
			else if (line == "����������ת") {
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
			else if (line == "��������ƽ��") {
				out << line << endl;
				getline(in, line);
				out << line;
				readMatrix(cma, line);
				readVector(cva, line.substr(line.find("\t")));
				cmb = cma.SetTrans(cva);
				out << "\t" << cmb.toString() << endl;
			}
			else if (line == "������������") {
				out << line << endl;
				getline(in, line);
				out << line;
				readMatrix(cma, line);
				line = line.substr(line.find("\t") + 1);
				readVector(cva, line);
				cmb = cma.SetScale(cva);
				out << "\t" << cmb.toString() << endl;
			}
			else if (line == "��������") {
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
		printf("δ�ҵ�test.txt�ļ���\n");
		system("Pause");
		exit(1);
	}
}

//==========
//�����ƺ���
//==========
int main(int argc, char *argv[])
{
	//����test.txt�е�����
	compute();
	//��ʼ������
	initStar();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("ͼ��ѧ3_1120142053_�쳿��");
	glutDisplayFunc(&myDisplay);
	glutTimerFunc(1, myTimerFunc, 0);
	glutReshapeFunc(&myReshape);
	glutKeyboardFunc(&myKeyboardFunc);
	glutSpecialFunc(&mySpecialKeyboardFunc);
	SetRC();
	glutMainLoop();
	return 0;
}