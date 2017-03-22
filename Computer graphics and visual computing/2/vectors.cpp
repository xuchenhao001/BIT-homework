
#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <GL/glut.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "math.h"

using namespace std;

//==========================
//全局变量定义部分(不可更改)
//==========================

string operate[20];//按行存储txt中的操作命令和操作数。
int command = 0;//记录当前执行的命令位置
int cmd_num = 0;//记录命令总数

const float deviation = 0.00001;//误差设定

float mx = 15, my = 5, mz = 15, tx = 0, ty = 0, tz = 0;//平移和旋转
float mspeed = 0.5, rspeed = 1;

//==============
//向量类定义部分
//==============

class CVector3 {
public:
	float x, y, z;

	friend CVector3 operator+(const CVector3 &a, const CVector3 &b);		//重载向量'+'
	friend CVector3 operator-(const CVector3 &a, const CVector3 &b);		//重载向量'-'
	friend CVector3 operator*(const CVector3 &a, double &b);				//重载向量数乘操作
	friend bool operator == (const CVector3 &a, const CVector3 &b);			//重载'=='比较操作
	friend bool operator != (const CVector3 &a, const CVector3 &b);			//重载'!='比较操作					

	//构造函数
	CVector3() {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	//带参数的构造函数
	CVector3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	void Set(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	//向量点乘操作
	float dotMul(CVector3 &n) {
		return this->x*n.x + this->y*n.y + this->z*n.z;
	}

	//向量叉乘操作
	CVector3 crossMul(CVector3 &n) {
		return CVector3(this->y*n.z - n.y*this->z, this->z*n.x - n.z*this->x, this->x*n.y - n.x*this->y);
	}

	//向量标准化操作
	void Normalize() {
		float length = this->len();
		if (length < deviation) {
			printf("零向量无法标准化！");
			return ;
		}
		this->x /= length;
		this->y /= length;
		this->z /= length;
	}

	//向量求模操作
	float len() {
		return sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
	}

	//向量投影操作
	CVector3 project(CVector3 &n) {
		float length = n.len();
		if (length < deviation) {
			return CVector3();
		}
		float proportion = this->dotMul(n) / length / length;
		return CVector3(n.x*proportion, n.y*proportion, n.z*proportion);
	}

	//重载赋值操作
	CVector3 operator=(const CVector3 &p) {
		x = p.x;
		y = p.y;
		z = p.z;
		return *this;
	}

	//仿照Java中toString,将向量xyz的值存入string
	string toString(){
		ostringstream buffer;
		buffer << setiosflags(ios::fixed);
		buffer << setprecision(2) << this->x << "," << this->y << "," << this->z;
		return string(buffer.str());
	}

	//类型转换
	operator float*() {
		return &x;
	}
};

//重载向量'+'
CVector3 operator+(const CVector3 &a, const CVector3 &b) {
	return CVector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

//重载向量'-'
CVector3 operator-(const CVector3 &a, const CVector3 &b) {
	return CVector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

//重载向量数乘操作'*'
CVector3 operator*(const CVector3 &a, float b) {
	return CVector3(a.x*b, a.y*b, a.z*b);
}

//重载'=='比较操作
bool operator==(const CVector3 &a, const CVector3 &b) {
	if (a.x == b.x&&a.y == b.y&&a.z == b.z) {
		return true;
	}
	return false;
}

//重载'!='比较操作
bool operator!=(const CVector3 &a, const CVector3 &b) {
	if (a.x == b.x&&a.y == b.y&&a.z == b.z) {
		return false;
	}
	return true;
}

//============
//全局运算变量
//============

struct Results{
	CVector3 vect1,vect2,vect3;
	float result;
	int cmd_type;
}vects[50];
int result_num;//记录计算结果总数
int result_p = 0;

//============
//函数声明部分
//============

void myTimerFunc(int val);//计时器函数
void myKeyboardFunc(unsigned char key, int x, int y);//键盘控制函数
void SetRC();//环境设置
void SetView();//视点设置
void DrawAxis();//画坐标系
void DrawVect(CVector3 vect, int color);//画向量
void DrawCompute();//画整个计算过程
void selectFont(int size, int charset, const char* face);//选择字体
void drawCNString(const char* str);//在OpenGL界面绘制汉字
void myDisplay(void);//总绘制图像
void myReshape(int w, int h);//窗口调整
void vectOperation();//向量计算
void readTxt();//读取test.txt文本
void writeTxt();//写入out.txt文本
void Pretreatment();//预处理

//============
//绘图函数部分
//============

//计时器函数
void myTimerFunc(int val){
	myDisplay();
	glutTimerFunc(1, myTimerFunc, 0);
}

//键盘控制函数,支持wasdqeikjluo移动视角和空格切换显示结果
void myKeyboardFunc(unsigned char key, int x, int y){
	switch (key){
		case 'w':my += mspeed;break;
		case 's':my -= mspeed;break;
		case 'a':mx -= mspeed;break;
		case 'd':mx += mspeed;break;
		case 'q':mz += mspeed;break;
		case 'e':mz -= mspeed;break;
		case 'i':ty += rspeed;break;
		case 'k':ty -= rspeed;break;
		case 'j':tx -= rspeed;break;
		case 'l':tx += rspeed;break;
		case 'u':tz += rspeed;break;
		case 'o':tz -= rspeed;break;
		case ' ':
			result_p = (result_p + 1) >= result_num ? 0 : (result_p + 1);
			break;
	}
}

//环境设置
void SetRC(){
	glShadeModel(GL_FLAT);
	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_BACK, GL_LINE);

	selectFont(37, DEFAULT_CHARSET, "华文仿宋");//设置字体格式
}

//视点设置
void SetView(){
	gluLookAt(mx, my, mz, tx, ty, tz, 0, 1, 0);
}

//OpenGL界面字体选择
void selectFont(int size, int charset, const char* face) {
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}

//在OpenGL界面绘制汉字
void drawCNString(const char* str)
{
	int len, i;
	wchar_t* wstring;
	HDC hDC = wglGetCurrentDC();
	GLuint list = glGenLists(1);
	// 计算字符的个数
	// 如果是双字节字符的（比如中文字符），两个字节才算一个字符
	// 否则一个字节算一个字符
	len = 0;
	for (i = 0; str[i] != '\0'; ++i)
	{
		if (IsDBCSLeadByte(str[i]))
			++i;
		++len;
	}
	// 将混合字符转化为宽字符
	wstring = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, -1, wstring, len);
	wstring[len] = L'\0';
	// 逐个输出字符
	for (i = 0; i<len; ++i)
	{
		wglUseFontBitmapsW(hDC, wstring[i], 1, list);
		glCallList(list);
	}
	// 回收所有临时资源
	free(wstring);
	glDeleteLists(list, 1);
}

//画坐标系,三个端点是三个圆锥
void DrawAxis(){
	glLineWidth(3);
	
	//绘制X轴
	glPushMatrix();
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(10, 0, 0);	
	glEnd();
	glTranslatef(10.0f, 0.0f, 0.0f);
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glutWireCone(0.25, 0.5, 10, 10);
	glPopMatrix();

	//绘制Y轴
	glPushMatrix();
	glBegin(GL_LINES);
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 10, 0);	
	glEnd();
	glTranslatef(0.0f, 10.0f, 0.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glutWireCone(0.25, 0.5, 10, 10);
	glPopMatrix();

	//绘制Z轴
	glPushMatrix();
	glBegin(GL_LINES);
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 10);	
	glEnd();
	glTranslatef(0.0f, 0.0f, 10.0f);
	glutWireCone(0.25, 0.5, 10, 10);
	glPopMatrix();

}

//画向量,参数分别为:要绘制的向量,向量颜色,向量宽度
void DrawVect(CVector3 vect, int color, float width) {
	glPushMatrix();
	glLineWidth(width);
	glBegin(GL_LINES);
	switch (color){
		case 0:glColor3f(0, 1, 1); break;
		case 1:glColor3f(1, 0, 1); break;
		case 2:glColor3f(1, 1, 0); break;
	}
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vect.x, vect.y, vect.z);
	glEnd();
	glPopMatrix();
}

//画整个计算过程的向量和说明文字
void DrawCompute() {
	string str;
	int type = vects[result_p].cmd_type;
	if (type == 1) {
		DrawVect(vects[result_p].vect1, 0, 2);
		DrawVect(vects[result_p].vect2, 1, 2);
		DrawVect(vects[result_p].vect3, 2, 2);

		str = "向量相加：(";
		str.append(vects[result_p].vect1.toString());
		str.append(") + (");
		str.append(vects[result_p].vect2.toString());
		str.append(") = (");
		str.append(vects[result_p].vect3.toString());
		str.append(")");

		glColor3f(1.0f, 1.0f, 1.0f);
		glRasterPos3f(-60, -30, 0);
		drawCNString(str.c_str());

	}
	if (type == 2) {
		DrawVect(vects[result_p].vect1, 0, 2);
		DrawVect(vects[result_p].vect2, 1, 2);

		str = "向量点乘：(";
		str.append(vects[result_p].vect1.toString());
		str.append(") · (");
		str.append(vects[result_p].vect2.toString());
		str.append(") = (");
		str.append(vects[result_p].vect3.toString());
		str.append(")");

		glColor3f(1.0f, 1.0f, 1.0f);
		glRasterPos3f(-60, -30, 0);
		drawCNString(str.c_str());
	}
	if (type == 3) {
		DrawVect(vects[result_p].vect1, 0, 2);
		DrawVect(vects[result_p].vect2, 1, 2);
		DrawVect(vects[result_p].vect3, 2, 2);

		str = "向量叉乘：(";
		str.append(vects[result_p].vect1.toString());
		str.append(") × (");
		str.append(vects[result_p].vect2.toString());
		str.append(") = (");
		str.append(vects[result_p].vect3.toString());
		str.append(")");

		glColor3f(1.0f, 1.0f, 1.0f);
		glRasterPos3f(-60, -30, 0);
		drawCNString(str.c_str());
	}
	if (type == 4) {
		DrawVect(vects[result_p].vect1, 0, 2);
		DrawVect(vects[result_p].vect3, 1, 4);

		str = "向量标准化：(";
		str.append(vects[result_p].vect1.toString());
		str.append(") -> (");
		str.append(vects[result_p].vect3.toString());
		str.append(")");

		glColor3f(1.0f, 1.0f, 1.0f);
		glRasterPos3f(-60, -30, 0);
		drawCNString(str.c_str());
	}
	if (type == 5) {
		DrawVect(vects[result_p].vect3, 0, 2);

		str = "向量求模：mod(";
		str.append(vects[result_p].vect3.toString());
		str.append(") = ");
		ostringstream buffer;
		buffer << setiosflags(ios::fixed);
		buffer << setprecision(2) << vects[result_p].result;
		str.append(string(buffer.str()));

		glColor3f(1.0f, 1.0f, 1.0f);
		glRasterPos3f(-60, -30, 0);
		drawCNString(str.c_str());
	}
	if (type == 6) {
		DrawVect(vects[result_p].vect1, 0, 2);
		DrawVect(vects[result_p].vect2, 1, 2);
		DrawVect(vects[result_p].vect3, 2, 4);

		str = "向量投影：(";
		str.append(vects[result_p].vect1.toString());
		str.append(") -> (");
		str.append(vects[result_p].vect2.toString());
		str.append(") = (");
		str.append(vects[result_p].vect3.toString());
		str.append(")");

		glColor3f(1.0f, 1.0f, 1.0f);
		glRasterPos3f(-60, -30, 0);
		drawCNString(str.c_str());
	}
}

//总绘制图像
void myDisplay(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	SetView();
	DrawAxis();
	DrawCompute();
	glPopMatrix();
	glutSwapBuffers();
}

//窗口调整
void myReshape(int w, int h){
	GLfloat nRange = 100.0f;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, GLfloat(w) / h, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//向量计算
void vectOperation(int num) {

	if (operate[command] == "向量相加") {
		vects[num].cmd_type = 1;
		sscanf_s(operate[command + 1].c_str(), "%f,%f,%f %f,%f,%f",\
			&vects[num].vect1.x, &vects[num].vect1.y, &vects[num].vect1.z, &vects[num].vect2.x, &vects[num].vect2.y, &vects[num].vect2.z);
		vects[num].vect3 = vects[num].vect1 + vects[num].vect2;
	}
	else if (operate[command] == "向量点乘") {
		vects[num].cmd_type = 2;
		sscanf_s(operate[command + 1].c_str(), "%f,%f,%f %f,%f,%f", \
			&vects[num].vect1.x, &vects[num].vect1.y, &vects[num].vect1.z, &vects[num].vect2.x, &vects[num].vect2.y, &vects[num].vect2.z);
		vects[num].result = vects[num].vect1.dotMul(vects[num].vect2);
	}
	else if (operate[command] == "向量叉乘") {
		vects[num].cmd_type = 3;
		sscanf_s(operate[command + 1].c_str(), "%f,%f,%f %f,%f,%f", \
			&vects[num].vect1.x, &vects[num].vect1.y, &vects[num].vect1.z, &vects[num].vect2.x, &vects[num].vect2.y, &vects[num].vect2.z);
		vects[num].vect3 = vects[num].vect1.crossMul(vects[num].vect2);
	}
	else if (operate[command] == "向量标准化") {
		vects[num].cmd_type = 4;
		sscanf_s(operate[command + 1].c_str(), "%f,%f,%f %f,%f,%f", \
			&vects[num].vect1.x, &vects[num].vect1.y, &vects[num].vect1.z);
		vects[num].vect3 = vects[num].vect1;
		vects[num].vect3.Normalize();
	}
	else if (operate[command] == "向量求模") {
		vects[num].cmd_type = 5;
		sscanf_s(operate[command + 1].c_str(), "%f,%f,%f %f,%f,%f", \
			&vects[num].vect3.x, &vects[num].vect3.y, &vects[num].vect3.z);
		vects[num].result = vects[num].vect3.len();
	}
	else if (operate[command] == "向量投影") {
		vects[num].cmd_type = 6;
		sscanf_s(operate[command + 1].c_str(), "%f,%f,%f %f,%f,%f", \
			&vects[num].vect1.x, &vects[num].vect1.y, &vects[num].vect1.z, &vects[num].vect2.x, &vects[num].vect2.y, &vects[num].vect2.z);
		vects[num].vect3 = vects[num].vect1.project(vects[num].vect2);
	}

}

//读取test.txt文本
void readTxt() {
	ifstream in("test.txt");
	string line;
	int i = 0;
	in.imbue(locale("chs"));
	if (in) {
		while (getline(in, line)) {
			operate[i] = line;
			i++;
		}
		cmd_num = i;
	}
	else {
		printf("未找到test.txt文件！\n");
		system("Pause");
		exit(1);
	}
}

//写入out.txt文本
void writeTxt() {
	ofstream out("out.txt", ios::trunc);
	for (int i = 0; i < cmd_num; i += 2) {
		out << setiosflags(ios::fixed);
		out << operate[i] << endl;
		if (operate[i] == "向量点乘" || operate[i] == "向量求模") {
			out << operate[i + 1] << "\t" << setprecision(2) << vects[i / 2].result << endl;
		}
		else{
			out << operate[i + 1] << "\t" << vects[i / 2].vect3.toString() << endl;
		}
	}
	out.close();
}

//预处理,包括读写文件,计算所有向量结果
void Pretreatment() {
	readTxt();//读取txt中的命令文本

	for (command = 0; command < cmd_num; command += 2) {
		vectOperation(command / 2);
	}
	result_num = command / 2;
	writeTxt();
}

int main(int argc, char *argv[]){
	Pretreatment();//预先计算并处理文本和所有向量

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1280, 720);
	glutCreateWindow("图形学2_1120142053_徐晨浩");
	glutDisplayFunc(&myDisplay);
	glutTimerFunc(1, myTimerFunc, 0);
	glutReshapeFunc(&myReshape);
	glutKeyboardFunc(&myKeyboardFunc);
	SetRC();
	glutMainLoop();
	return 0;
}