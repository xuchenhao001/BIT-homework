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
#include <queue>
#include "math.h"
#include "MyMatrixVector.h"

using namespace std;

//====================================
//全局变量定义部分(可依据情况适当修改)
//====================================

int width = 1280, height = 720;//星星绘制窗口默认大小为1280x720，运行后可任意更改
const int STAR_NUM = 10000;//星星总数量，默认为10000个

//============
//循环队列定义
//============
template <class T>
class cycleQueue
{
private:
	unsigned int m_size;
	int m_front;
	int m_now;//定义遍历变量
	int m_rear;
	int now_size;
	T*  m_data;
public:
	cycleQueue(unsigned size):m_size(size),m_front(0),m_rear(0){
		m_data = new T[size];
	}

	~cycleQueue(){
		delete[] m_data;
	}

	void reset() {
		m_now = m_front;
	}

	T now() throw(bad_exception) {
		if (this->isEmpty()) {
			throw bad_exception();
		}
		return m_data[m_now];
	}

	T next() throw(bad_exception) {
		if (m_now == m_rear) {
			throw bad_exception();
		}
		return m_data[++m_now];
	}

	int size() {
		return now_size;
	}

	bool isEmpty(){
		return m_front == m_rear;
	}

	bool isFull(){
		return m_front == (m_rear + 1) % m_size;
	}

	void push(T ele)throw(bad_exception){
		if (isFull()){
			throw bad_exception();
		}
		now_size++;
		m_data[m_rear] = ele;
		m_rear = (m_rear + 1) % m_size;
	}

	T pop() throw(bad_exception){
		if (isEmpty()){
			throw bad_exception();
		}
		now_size--;
		T tmp = m_data[m_front];
		m_front = (m_front + 1) % m_size;
		return tmp;
	}
};

//==========================
//全局变量定义部分(不可更改)
//==========================

//平移，旋转，缩放
CVector3 mpos(0, 500, 1000), rpos(-25, 0, 0), spos(1, 1, 1);

//可爱的小尾巴
cycleQueue<CVector3> tail(121);

float mspeed = 0.1, rspeed = 1;

//使用自己编写的矩阵类,调用默认的构造函数，构造单位矩阵
CMatrix g_IEyeMat, g_EyeMat;
//视点变换方式
int mode = 0;

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
void myKeyboardFunc(unsigned char key, int x, int y) {
	bool bChange = false;

	switch (key)
	{
	case 'w':
		//my+=mspeed;
		if (mode == 0) {
			/*glPushMatrix();
			glLoadIdentity();
			glTranslatef(0, -mspeed, 0);
			glMultMatrixf(g_EyeMat);
			glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			glPopMatrix();*/
			CMatrix g;
			g_EyeMat = g.SetTrans(CVector3(0, -mspeed, 0))*g_EyeMat;
		}
		else {
			CVector3 EyeDir(g_IEyeMat[4], g_IEyeMat[5], g_IEyeMat[6]);
			mpos = mpos + EyeDir*mspeed;
		}
		break;
	case 's':
		//my-=mspeed;	
		if (mode == 0) {
			/*glPushMatrix();
			glLoadIdentity();
			glTranslatef(0, mspeed, 0);
			glMultMatrixf(g_EyeMat);
			glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			glPopMatrix();*/
			CMatrix g;
			g_EyeMat = g.SetTrans(CVector3(0, mspeed, 0))*g_EyeMat;
		}
		else {
			CVector3 EyeDir(g_IEyeMat[4], g_IEyeMat[5], g_IEyeMat[6]);
			mpos = mpos - EyeDir*mspeed;
		}
		break;
	case 'a':
		//mx-=mspeed;
		if (mode == 0) {
			//glPushMatrix();
			//glLoadIdentity();
			//glTranslatef(mspeed, 0, 0);
			//glMultMatrixf(g_EyeMat);
			//glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			//glPopMatrix();
			CMatrix g;
			g_EyeMat = g.SetTrans(CVector3(mspeed, 0, 0))*g_EyeMat;
		}
		else {
			CVector3 EyeDir(g_IEyeMat[0], g_IEyeMat[1], g_IEyeMat[2]);
			mpos = mpos - EyeDir*mspeed;
		}
		break;
	case 'd':
		//mx+=mspeed;
		if (mode == 0) {
			//glPushMatrix();
			//glLoadIdentity();
			//glTranslatef(-mspeed, 0, 0);
			//glMultMatrixf(g_EyeMat);
			//glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			//glPopMatrix();
			CMatrix g;
			g_EyeMat = g.SetTrans(CVector3(-mspeed, 0, 0))*g_EyeMat;
		}
		else {
			CVector3 EyeDir(g_IEyeMat[0], g_IEyeMat[1], g_IEyeMat[2]);
			mpos = mpos + EyeDir*mspeed;
		}
		break;
	case 'q':
		if (mode == 0) {
			//glPushMatrix();
			//glLoadIdentity();
			//glTranslatef(0, 0, mspeed);
			//glMultMatrixf(g_EyeMat);
			//glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			//glPopMatrix();
			CMatrix g;
			g_EyeMat = g.SetTrans(CVector3(0, 0, mspeed))*g_EyeMat;
		}
		else {
			CVector3 EyeDir(g_IEyeMat[8], g_IEyeMat[9], g_IEyeMat[10]);
			mpos = mpos + EyeDir*mspeed;
		}
		//mz-=mspeed;
		break;
	case 'e':
		if (mode == 0) {
			//glPushMatrix();
			//glLoadIdentity();
			//glTranslatef(0, 0, -mspeed);
			//glMultMatrixf(g_EyeMat);
			//glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			//glPopMatrix();
			CMatrix g;
			g_EyeMat = g.SetTrans(CVector3(0, 0, -mspeed))*g_EyeMat;
		}
		else {
			CVector3 EyeDir(g_IEyeMat[8], g_IEyeMat[9], g_IEyeMat[10]);
			mpos = mpos - EyeDir*mspeed;
		}
		//mz+=mspeed;
		break;
	case 'i':
		if (mode == 0) {
			//glPushMatrix();
			//glLoadIdentity();
			//glRotatef(-rspeed, 1, 0, 0);
			//glMultMatrixf(g_EyeMat);
			//glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			//glPopMatrix();
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
			//glPushMatrix();
			//glLoadIdentity();
			//glRotatef(rspeed, 1, 0, 0);
			//glMultMatrixf(g_EyeMat);
			//glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			//glPopMatrix();
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
			//glPushMatrix();
			//glLoadIdentity();
			//glRotatef(-rspeed, 0, 1, 0);
			//glMultMatrixf(g_EyeMat);
			//glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			//glPopMatrix();
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
			//glPushMatrix();
			//glLoadIdentity();
			//glRotatef(rspeed, 0, 1, 0);
			//glMultMatrixf(g_EyeMat);
			//glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			//glPopMatrix();
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
			//glPushMatrix();
			//glLoadIdentity();
			//glRotatef(rspeed, 0, 0, 1);
			//glMultMatrixf(g_EyeMat);
			//glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			//glPopMatrix();
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
			//glPushMatrix();
			//glLoadIdentity();
			//glRotatef(-rspeed, 0, 0, 1);
			//glMultMatrixf(g_EyeMat);
			//glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			//glPopMatrix();
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
		if (mode == 0)
		{
			CMatrix g;
			g_EyeMat = g.SetRotate(-rpos.z, CVector3(0, 0, 1))*g.SetRotate(-rpos.x, CVector3(1, 0, 0))*g.SetRotate(-rpos.y, CVector3(0, 1, 0))*g.SetTrans(CVector3(-mpos.x, -mpos.y, -mpos.z));
		}
		printf("mode:%d\n", mode);
		break;
	}
	if (bChange)//计算视点矩阵的逆矩阵
	{
		//glPushMatrix();
		//glLoadIdentity();
		//glRotatef(rpos.y, 0, 1, 0);
		//glRotatef(rpos.x, 1, 0, 0);
		//glRotatef(rpos.z, 0, 0, 1);
		//glGetFloatv(GL_MODELVIEW_MATRIX, g_IEyeMat);
		//glPopMatrix();
		CMatrix g;
		g_IEyeMat = g.SetRotate(rpos.y, CVector3(0, 1, 0))*g.SetRotate(rpos.x, CVector3(1, 0, 0))*g.SetRotate(rpos.z, CVector3(0, 0, 1));
	}
	if (mode == 0)
	{
		g_IEyeMat = g_EyeMat.GetInverse();
	}
}

//设置环境
void SetRC() {

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
	float Quadratic[] = { 0,0.12,0 };
	//glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, Quadratic);
	glPointParameterf(GL_POINT_SIZE_MIN, 1);//设置最小点
	glPointParameterf(GL_POINT_SIZE_MAX, 100);//设置最大点
}

//设置视点
void SetView() {
	if (mode == 0)
	{
		glLoadMatrixf(g_EyeMat);
	}
	else
	{
		glRotatef(-rpos.z, 0, 0, 1);
		glRotatef(-rpos.x, 1, 0, 0);
		glRotatef(-rpos.y, 0, 1, 0);
		glTranslatef(-mpos.x, -mpos.y, -mpos.z);
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

//画尾巴
void drawTail() {
	glPushMatrix();
	for (int i = 0; i < tail.size() - 1; i++) {
		glLineWidth(1);
		tail.reset();
		glColor3f(1, 1, 1);
		glBegin(GL_LINES);
		glVertex3f(tail.now().x, tail.now().y, tail.now().z);
		tail.next();
		glVertex3f(tail.now().x, tail.now().y, tail.now().z);
		glEnd();
	}
	glPopMatrix();
}

//图形总绘制
void myDisplay(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	SetView();
	drawStar();	
	
	//绘制坐标轴
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