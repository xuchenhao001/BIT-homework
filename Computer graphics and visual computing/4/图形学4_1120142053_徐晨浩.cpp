// 图形学4_1120142053_徐晨浩.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <fstream>
#include <string>
#include <GL/glew.h>
#include <GL/glut.h>
#pragma comment(lib,"glew32.lib ")
#include "MyOpenGlClass.h"

using namespace std;



//====================================
//全局变量定义部分(可依据情况适当修改)
//====================================
int width = 1280, height = 720;	//星星绘制窗口默认大小为1280x720，运行后可任意更改
const int STAR_NUM = 10000;		//星星总数量，默认为10000个
const int TAIL_LEN = 121;		//小尾巴长度
const int VIEW_REC = 3;			//视点记录最大数量
float mspeed = 5, rspeed = 1;	//移动速度和旋转速度

//==========================
//全局变量定义部分(不可更改)
//==========================

//平移，旋转，缩放
CVector3 mpos(0, 500, 1000), rpos(-25, 0, 0), spos(1, 1, 1);

//可爱的小尾巴
cycleQueue<CVector3> tail(TAIL_LEN);

//视点记录循环堆栈
cycleStack<CQuaternion> view(VIEW_REC);

//计算输出out.txt结果最大长度
const int MAX_RESAULT_LEN = 200;

//使用自己编写的矩阵类,调用默认的构造函数，构造单位矩阵
CMatrix g_IEyeMat, g_EyeMat;

//视点变换方式,1为欧拉视角,0为自由视点变换
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
			g1 = g_EyeMat.GetInverse();
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
		if (mode == 0) {
			CMatrix g1, g2, g3, g4;
			g_EyeMat = g1.SetRotate(-rpos.z, CVector3(0, 0, 1))*g2.SetRotate(-rpos.x, CVector3(1, 0, 0))*g3.SetRotate(-rpos.y, CVector3(0, 1, 0))*g4.SetTrans(CVector3(-mpos.x, -mpos.y, -mpos.z));
		}
		printf("mode:%d\n", mode);
		break;
	}
	if (bChange)//计算视点矩阵的逆矩阵
	{
		CMatrix g1, g2, g3;
		g_IEyeMat = g1.SetRotate(rpos.y, CVector3(0, 1, 0))*g2.SetRotate(rpos.x, CVector3(1, 0, 0))*g3.SetRotate(rpos.z, CVector3(0, 0, 1));
	}
}

//F2键控制
void mySpecialKeyboardFunc(int key, int x, int y) {
	switch (key) {
	//切换视点模式
	case GLUT_KEY_F2:
		mode = 1 - mode;
		if (mode == 0) {
			CMatrix g1, g2, g3, g4;
			g_EyeMat = g1.SetRotate(-rpos.z, CVector3(0, 0, 1))*g2.SetRotate(-rpos.x, CVector3(1, 0, 0))*g3.SetRotate(-rpos.y, CVector3(0, 1, 0))*g4.SetTrans(CVector3(-mpos.x, -mpos.y, -mpos.z));
		}
		printf("mode:%d\n", mode);
		break;
	//保存当前视点
	case GLUT_KEY_F3:
		if (mode == 0) {
			view.push(g_EyeMat.ToQuaternion());
		}
		else {
			CMatrix g, g1, g2, g3, g4;
			g = g1.SetRotate(-rpos.z, CVector3(0, 0, 1))*g2.SetRotate(-rpos.x, CVector3(1, 0, 0))*g3.SetRotate(-rpos.y, CVector3(0, 1, 0))*g4.SetTrans(CVector3(-mpos.x, -mpos.y, -mpos.z));
			view.push(g.ToQuaternion());
		}
		printf("View has been saved\n");
		break;
	case GLUT_KEY_F4:
		if (mode == 0)
		{
			g_EyeMat = view.pop().ToMatrix();
		}
		else {
			CMatrix matrix = view.pop().ToMatrix();
			mpos.Set(matrix.m03, matrix.m13, matrix.m23);
		}
		printf("View has been restored\n");
	case GLUT_KEY_F5:;

	}
}

//设置环境
void SetRC() {

	//星星是圆的(与控制大小变换冲突所以注视掉了)
	//glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glShadeModel(GL_FLAT);
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_BACK, GL_LINE);

	//控制星星随着视点远近变换大小
	glewInit();
	float Quadratic[] = { 0,0.1,0 };
	glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, Quadratic);
	glPointParameterf(GL_POINT_SIZE_MIN, 1);//设置最小点
	glPointParameterf(GL_POINT_SIZE_MAX, 100);//设置最大点
}

//设置视点
void SetView() {
	if (mode == 0){
		glLoadMatrixf(g_EyeMat);
	}
	else{
		CMatrix g1, g2, g3, g4;
		(g1.SetRotate(-rpos.z, CVector3(0, 0, 1))*g2.SetRotate(-rpos.x, CVector3(1, 0, 0))*g3.SetRotate(-rpos.y, CVector3(0, 1, 0))*g4.SetTrans(CVector3(-mpos.x, -mpos.y, -mpos.z))).run();
	}
}

//画星星
void drawStar() {
	glPointSize(20);
	glBegin(GL_POINTS);
	for (int i = 0; i < STAR_NUM; i++) {
		glVertex3f(stars[i].x, stars[i].y, stars[i].z);
	}
	glEnd();
}

//画尾巴
void drawTail() {
	glPushMatrix();
	tail.reset();
	for (int i = 0; i < tail.size() - 1; i++) {
		float color;
		glLineWidth(1);
		//当尾巴长度达到120的时候，控制最后20米的尾巴渐渐消失
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

//图形总绘制
void myDisplay(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	SetView();
	//画星星
	drawStar();
	//画尾巴
	drawTail();

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

//读取test.txt文本到字符串数组operate数组里
void readTxt(string *operate, int &lineNum) {
	ifstream in("test.txt");
	string line;
	int i = 0;
	in.imbue(locale("chs"));
	if (in) {
		while (getline(in, line)) {
			operate[i] = line;
			i++;
		}
		lineNum = i;
	}
	else {
		printf("未找到test.txt文件！\n");
		system("Pause");
		exit(1);
	}
}

//将string数组operate中的字符串写入out.txt文本
void writeTxt(string *operate, int lineNum) {
	ofstream out("out.txt", ios::trunc);
	for (int i = 0; i < lineNum; i++) {
		out << operate[i] << endl;
	}
	out.close();
}

//从字符串中读取矩阵
void readMatrix(string value, CMatrix & matrix) {
	sscanf_s(value.c_str(), "%f,%f,%f,%f,%f,%f,%f,%f,"
			"%f,%f,%f,%f,%f,%f,%f,%f",
		&matrix.m00, &matrix.m01, &matrix.m02, &matrix.m03,
		&matrix.m10, &matrix.m11, &matrix.m12, &matrix.m13,
		&matrix.m20, &matrix.m21, &matrix.m22, &matrix.m23,
		&matrix.m30, &matrix.m31, &matrix.m32, &matrix.m33);
}

//向result字符串中写入矩阵
void writeMatrix(char* result, CMatrix & matrix) {
	snprintf(result, MAX_RESAULT_LEN, "\t%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,"
			"%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f",
		matrix.m00, matrix.m01, matrix.m02, matrix.m03,
		matrix.m10, matrix.m11, matrix.m12, matrix.m13,
		matrix.m20, matrix.m21, matrix.m22, matrix.m23,
		matrix.m30, matrix.m31, matrix.m32, matrix.m33);
}

//计算矩阵、向量、欧拉角
string compute(string command, string value) {
	char result[MAX_RESAULT_LEN];
	float angle, t;
	CVector3 vect;
	CEuler euler;
	CMatrix matrix;
	CQuaternion quater, quater1, quater2;

	if (command == "欧拉角转换向量") {
		sscanf_s(value.c_str(), "%f,%f,%f", &euler.h, &euler.p, &euler.b);
		vect = euler.ToVector3();
		snprintf(result, MAX_RESAULT_LEN, "\t%.2f,%.2f,%.2f", vect.x, vect.y, vect.z);
	}
	else if (command == "向量转换欧拉角") {
		sscanf_s(value.c_str(), "%f,%f,%f", &vect.x, &vect.y, &vect.z);
		euler = vect.ToEuler();
		snprintf(result, MAX_RESAULT_LEN, "\t%.2f,%.2f,%.2f", euler.h, euler.p, euler.b);
	}
	else if (command == "欧拉角转换四元数") {
		sscanf_s(value.c_str(), "%f,%f,%f", &euler.h, &euler.p, &euler.b);
		quater = euler.ToQuaternion();
		snprintf(result, MAX_RESAULT_LEN, "\t%.2f,%.2f,%.2f,%.2f", quater.x, quater.y, quater.z, quater.w);
	}
	else if (command == "四元数转换欧拉角") {
		sscanf_s(value.c_str(), "%f,%f,%f,%f", &quater.x, &quater.y, &quater.z, &quater.w);
		euler = quater.ToEuler();
		snprintf(result, MAX_RESAULT_LEN, "\t%.2f,%.2f,%.2f", euler.h, euler.p, euler.b);
	}
	else if (command == "欧拉角转换矩阵") {
		sscanf_s(value.c_str(), "%f,%f,%f", &euler.h, &euler.p, &euler.b);
		matrix = euler.ToMatrix();
		writeMatrix(result, matrix);
	}
	else if (command == "矩阵转换欧拉角") {
		readMatrix(value, matrix);
		euler = matrix.ToEuler();
		snprintf(result, MAX_RESAULT_LEN, "\t%.2f,%.2f,%.2f", euler.h, euler.p, euler.b);
	}
	else if (command == "矩阵转换四元数") {
		readMatrix(value, matrix);
		quater = matrix.ToQuaternion();
		snprintf(result, MAX_RESAULT_LEN, "\t%.2f,%.2f,%.2f,%.2f", quater.x, quater.y, quater.z, quater.w);
	}
	else if (command == "四元数转换矩阵") {
		sscanf_s(value.c_str(), "%f,%f,%f,%f", &quater.x, &quater.y, &quater.z, &quater.w);
		matrix = quater.ToMatrix();
		writeMatrix(result, matrix);
	}
	else if (command == "欧拉角标准化") {
		sscanf_s(value.c_str(), "%f,%f,%f", &euler.h, &euler.p, &euler.b);
		euler.Normal();
		snprintf(result, MAX_RESAULT_LEN, "\t%.2f,%.2f,%.2f", euler.h, euler.p, euler.b);
	}
	else if (command == "四元数标准化") {
		sscanf_s(value.c_str(), "%f,%f,%f,%f", &quater.x, &quater.y, &quater.z, &quater.w);
		quater.Normalize();
		snprintf(result, MAX_RESAULT_LEN, "\t%.2f,%.2f,%.2f,%.2f", quater.x, quater.y, quater.z, quater.w);
	}
	else if (command == "四元数相乘") {
		sscanf_s(value.c_str(), "%f,%f,%f,%f\t%f,%f,%f,%f",\
			&quater.x, &quater.y, &quater.z, &quater.w, \
			&quater1.x, &quater1.y, &quater1.z, &quater1.w);
		quater2 = quater*quater1;
		snprintf(result, MAX_RESAULT_LEN, "\t%.2f,%.2f,%.2f,%.2f", quater2.x, quater2.y, quater2.z, quater2.w);
	}
	else if (command == "四元数求差") {
		sscanf_s(value.c_str(), "%f,%f,%f,%f\t%f,%f,%f,%f", \
			&quater.x, &quater.y, &quater.z, &quater.w, \
			&quater1.x, &quater1.y, &quater1.z, &quater1.w);
		quater2 = quater.Div(quater1);
		snprintf(result, MAX_RESAULT_LEN, "\t%.2f,%.2f,%.2f,%.2f", quater2.x, quater2.y, quater2.z, quater2.w);
	}
	else if (command == "四元数点乘") {
		sscanf_s(value.c_str(), "%f,%f,%f,%f\t%f,%f,%f,%f", \
			&quater.x, &quater.y, &quater.z, &quater.w, \
			&quater1.x, &quater1.y, &quater1.z, &quater1.w);
		float dotResult = quater.dotMul(quater1);
		snprintf(result, MAX_RESAULT_LEN, "\t%.2f", dotResult);
	}
	else if (command == "四元数求逆") {
		sscanf_s(value.c_str(), "%f,%f,%f,%f", &quater.x, &quater.y, &quater.z, &quater.w);
		quater.Inverse();
		snprintf(result, MAX_RESAULT_LEN, "\t%.2f,%.2f,%.2f,%.2f", quater.x, quater.y, quater.z, quater.w);
	}
	else if (command == "四元数求角度和旋转轴") {
		sscanf_s(value.c_str(), "%f,%f,%f,%f", &quater.x, &quater.y, &quater.z, &quater.w);
		quater.GetAngle(angle, vect);
		snprintf(result, MAX_RESAULT_LEN, "\t%.2f\t%.2f,%.2f,%.2f", angle, vect.x, vect.y, vect.z);
	}
	else if (command == "四元数插值") {
		sscanf_s(value.c_str(), "%f,%f,%f,%f\t%f,%f,%f,%f\t%f", \
			&quater.x, &quater.y, &quater.z, &quater.w, \
			&quater1.x, &quater1.y, &quater1.z, &quater1.w, &t);
		quater2 = quater.Slerp(quater1, t);
		snprintf(result, MAX_RESAULT_LEN, "\t%.2f,%.2f,%.2f,%.2f", quater2.x, quater2.y, quater2.z, quater2.w);
	}
	return value.append(result);
}

//==========
//主控制函数
//==========

//调度输入输出和计算过程
void schedule() {
	string operate[MAX_RESAULT_LEN];
	int lineNum;
	readTxt(operate, lineNum);
	for (int i = 0; i < lineNum; i += 2) {
		operate[i + 1] = compute(operate[i], operate[i + 1]);
	}
	writeTxt(operate, lineNum);
}

int main(int argc, char *argv[])
{
	//调度计算test.txt中的数据
	//schedule();
	//初始化星星
	initStar();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("图形学4_1120142053_徐晨浩");
	glutDisplayFunc(&myDisplay);
	glutTimerFunc(1, myTimerFunc, 0);
	glutReshapeFunc(&myReshape);
	glutKeyboardFunc(&myKeyboardFunc);
	glutSpecialFunc(&mySpecialKeyboardFunc);
	SetRC();
	glutMainLoop();
    return 0;
}

