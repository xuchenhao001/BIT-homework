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

using namespace std;

//====================================
//ȫ�ֱ������岿��(����������ʵ��޸�)
//====================================

int width = 1280, height = 720;//���ǻ��ƴ���Ĭ�ϴ�СΪ1280x720�����к���������
const int STAR_NUM = 10000;//������������Ĭ��Ϊ10000��

//============
//ѭ�����ж���
//============
template <class T>
class cycleQueue
{
private:
	unsigned int m_size;
	int m_front;
	int m_now;//�����������
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
//ȫ�ֱ������岿��(���ɸ���)
//==========================

//ƽ�ƣ���ת������
CVector3 mpos(0, 500, 1000), rpos(-25, 0, 0), spos(1, 1, 1);

//�ɰ���Сβ��
cycleQueue<CVector3> tail(121);

float mspeed = 0.1, rspeed = 1;

//ʹ���Լ���д�ľ�����,����Ĭ�ϵĹ��캯�������쵥λ����
CMatrix g_IEyeMat, g_EyeMat;
//�ӵ�任��ʽ
int mode = 0;

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
	if (bChange)//�����ӵ����������
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

//���û���
void SetRC() {

	//������Բ��
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
		glRotatef(-rpos.z, 0, 0, 1);
		glRotatef(-rpos.x, 1, 0, 0);
		glRotatef(-rpos.y, 0, 1, 0);
		glTranslatef(-mpos.x, -mpos.y, -mpos.z);
	}
}

//һ��һ��������
void drawStar() {
	//glTranslatef(0, 0, -1000);

	glPointSize(2);
	glBegin(GL_POINTS);
	for (int i = 0; i < STAR_NUM; i++) {
		glVertex3f(stars[i].x, stars[i].y, stars[i].z);
	}
	glEnd();
}

//��β��
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

//ͼ���ܻ���
void myDisplay(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	SetView();
	drawStar();	
	
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
	//compute();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("ͼ��ѧ3_1120142053_�쳿��");
	initStar();
	glutDisplayFunc(&myDisplay);
	glutTimerFunc(1, myTimerFunc, 0);
	glutReshapeFunc(&myReshape);
	glutKeyboardFunc(&myKeyboardFunc);
	SetRC();
	glutMainLoop();
	return 0;
}