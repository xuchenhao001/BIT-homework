
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
//ȫ�ֱ������岿��(���ɸ���)
//==========================

string operate[20];//���д洢txt�еĲ�������Ͳ�������
int command = 0;//��¼��ǰִ�е�����λ��
int cmd_num = 0;//��¼��������

const float deviation = 0.00001;//����趨

float mx = 15, my = 5, mz = 15, tx = 0, ty = 0, tz = 0;//ƽ�ƺ���ת
float mspeed = 0.5, rspeed = 1;

//==============
//�����ඨ�岿��
//==============

class CVector3 {
public:
	float x, y, z;

	friend CVector3 operator+(const CVector3 &a, const CVector3 &b);		//��������'+'
	friend CVector3 operator-(const CVector3 &a, const CVector3 &b);		//��������'-'
	friend CVector3 operator*(const CVector3 &a, double &b);				//�����������˲���
	friend bool operator == (const CVector3 &a, const CVector3 &b);			//����'=='�Ƚϲ���
	friend bool operator != (const CVector3 &a, const CVector3 &b);			//����'!='�Ƚϲ���					

	//���캯��
	CVector3() {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	//�������Ĺ��캯��
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

	//������˲���
	float dotMul(CVector3 &n) {
		return this->x*n.x + this->y*n.y + this->z*n.z;
	}

	//������˲���
	CVector3 crossMul(CVector3 &n) {
		return CVector3(this->y*n.z - n.y*this->z, this->z*n.x - n.z*this->x, this->x*n.y - n.x*this->y);
	}

	//������׼������
	void Normalize() {
		float length = this->len();
		if (length < deviation) {
			printf("�������޷���׼����");
			return ;
		}
		this->x /= length;
		this->y /= length;
		this->z /= length;
	}

	//������ģ����
	float len() {
		return sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
	}

	//����ͶӰ����
	CVector3 project(CVector3 &n) {
		float length = n.len();
		if (length < deviation) {
			return CVector3();
		}
		float proportion = this->dotMul(n) / length / length;
		return CVector3(n.x*proportion, n.y*proportion, n.z*proportion);
	}

	//���ظ�ֵ����
	CVector3 operator=(const CVector3 &p) {
		x = p.x;
		y = p.y;
		z = p.z;
		return *this;
	}

	//����Java��toString,������xyz��ֵ����string
	string toString(){
		ostringstream buffer;
		buffer << setiosflags(ios::fixed);
		buffer << setprecision(2) << this->x << "," << this->y << "," << this->z;
		return string(buffer.str());
	}

	//����ת��
	operator float*() {
		return &x;
	}
};

//��������'+'
CVector3 operator+(const CVector3 &a, const CVector3 &b) {
	return CVector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

//��������'-'
CVector3 operator-(const CVector3 &a, const CVector3 &b) {
	return CVector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

//�����������˲���'*'
CVector3 operator*(const CVector3 &a, float b) {
	return CVector3(a.x*b, a.y*b, a.z*b);
}

//����'=='�Ƚϲ���
bool operator==(const CVector3 &a, const CVector3 &b) {
	if (a.x == b.x&&a.y == b.y&&a.z == b.z) {
		return true;
	}
	return false;
}

//����'!='�Ƚϲ���
bool operator!=(const CVector3 &a, const CVector3 &b) {
	if (a.x == b.x&&a.y == b.y&&a.z == b.z) {
		return false;
	}
	return true;
}

//============
//ȫ���������
//============

struct Results{
	CVector3 vect1,vect2,vect3;
	float result;
	int cmd_type;
}vects[50];
int result_num;//��¼����������
int result_p = 0;

//============
//������������
//============

void myTimerFunc(int val);//��ʱ������
void myKeyboardFunc(unsigned char key, int x, int y);//���̿��ƺ���
void SetRC();//��������
void SetView();//�ӵ�����
void DrawAxis();//������ϵ
void DrawVect(CVector3 vect, int color);//������
void DrawCompute();//�������������
void selectFont(int size, int charset, const char* face);//ѡ������
void drawCNString(const char* str);//��OpenGL������ƺ���
void myDisplay(void);//�ܻ���ͼ��
void myReshape(int w, int h);//���ڵ���
void vectOperation();//��������
void readTxt();//��ȡtest.txt�ı�
void writeTxt();//д��out.txt�ı�
void Pretreatment();//Ԥ����

//============
//��ͼ��������
//============

//��ʱ������
void myTimerFunc(int val){
	myDisplay();
	glutTimerFunc(1, myTimerFunc, 0);
}

//���̿��ƺ���,֧��wasdqeikjluo�ƶ��ӽǺͿո��л���ʾ���
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

//��������
void SetRC(){
	glShadeModel(GL_FLAT);
	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_BACK, GL_LINE);

	selectFont(37, DEFAULT_CHARSET, "���ķ���");//���������ʽ
}

//�ӵ�����
void SetView(){
	gluLookAt(mx, my, mz, tx, ty, tz, 0, 1, 0);
}

//OpenGL��������ѡ��
void selectFont(int size, int charset, const char* face) {
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}

//��OpenGL������ƺ���
void drawCNString(const char* str)
{
	int len, i;
	wchar_t* wstring;
	HDC hDC = wglGetCurrentDC();
	GLuint list = glGenLists(1);
	// �����ַ��ĸ���
	// �����˫�ֽ��ַ��ģ����������ַ����������ֽڲ���һ���ַ�
	// ����һ���ֽ���һ���ַ�
	len = 0;
	for (i = 0; str[i] != '\0'; ++i)
	{
		if (IsDBCSLeadByte(str[i]))
			++i;
		++len;
	}
	// ������ַ�ת��Ϊ���ַ�
	wstring = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, -1, wstring, len);
	wstring[len] = L'\0';
	// �������ַ�
	for (i = 0; i<len; ++i)
	{
		wglUseFontBitmapsW(hDC, wstring[i], 1, list);
		glCallList(list);
	}
	// ����������ʱ��Դ
	free(wstring);
	glDeleteLists(list, 1);
}

//������ϵ,�����˵�������Բ׶
void DrawAxis(){
	glLineWidth(3);
	
	//����X��
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

	//����Y��
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

	//����Z��
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

//������,�����ֱ�Ϊ:Ҫ���Ƶ�����,������ɫ,�������
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

//������������̵�������˵������
void DrawCompute() {
	string str;
	int type = vects[result_p].cmd_type;
	if (type == 1) {
		DrawVect(vects[result_p].vect1, 0, 2);
		DrawVect(vects[result_p].vect2, 1, 2);
		DrawVect(vects[result_p].vect3, 2, 2);

		str = "������ӣ�(";
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

		str = "������ˣ�(";
		str.append(vects[result_p].vect1.toString());
		str.append(") �� (");
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

		str = "������ˣ�(";
		str.append(vects[result_p].vect1.toString());
		str.append(") �� (");
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

		str = "������׼����(";
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

		str = "������ģ��mod(";
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

		str = "����ͶӰ��(";
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

//�ܻ���ͼ��
void myDisplay(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	SetView();
	DrawAxis();
	DrawCompute();
	glPopMatrix();
	glutSwapBuffers();
}

//���ڵ���
void myReshape(int w, int h){
	GLfloat nRange = 100.0f;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, GLfloat(w) / h, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//��������
void vectOperation(int num) {

	if (operate[command] == "�������") {
		vects[num].cmd_type = 1;
		sscanf_s(operate[command + 1].c_str(), "%f,%f,%f %f,%f,%f",\
			&vects[num].vect1.x, &vects[num].vect1.y, &vects[num].vect1.z, &vects[num].vect2.x, &vects[num].vect2.y, &vects[num].vect2.z);
		vects[num].vect3 = vects[num].vect1 + vects[num].vect2;
	}
	else if (operate[command] == "�������") {
		vects[num].cmd_type = 2;
		sscanf_s(operate[command + 1].c_str(), "%f,%f,%f %f,%f,%f", \
			&vects[num].vect1.x, &vects[num].vect1.y, &vects[num].vect1.z, &vects[num].vect2.x, &vects[num].vect2.y, &vects[num].vect2.z);
		vects[num].result = vects[num].vect1.dotMul(vects[num].vect2);
	}
	else if (operate[command] == "�������") {
		vects[num].cmd_type = 3;
		sscanf_s(operate[command + 1].c_str(), "%f,%f,%f %f,%f,%f", \
			&vects[num].vect1.x, &vects[num].vect1.y, &vects[num].vect1.z, &vects[num].vect2.x, &vects[num].vect2.y, &vects[num].vect2.z);
		vects[num].vect3 = vects[num].vect1.crossMul(vects[num].vect2);
	}
	else if (operate[command] == "������׼��") {
		vects[num].cmd_type = 4;
		sscanf_s(operate[command + 1].c_str(), "%f,%f,%f %f,%f,%f", \
			&vects[num].vect1.x, &vects[num].vect1.y, &vects[num].vect1.z);
		vects[num].vect3 = vects[num].vect1;
		vects[num].vect3.Normalize();
	}
	else if (operate[command] == "������ģ") {
		vects[num].cmd_type = 5;
		sscanf_s(operate[command + 1].c_str(), "%f,%f,%f %f,%f,%f", \
			&vects[num].vect3.x, &vects[num].vect3.y, &vects[num].vect3.z);
		vects[num].result = vects[num].vect3.len();
	}
	else if (operate[command] == "����ͶӰ") {
		vects[num].cmd_type = 6;
		sscanf_s(operate[command + 1].c_str(), "%f,%f,%f %f,%f,%f", \
			&vects[num].vect1.x, &vects[num].vect1.y, &vects[num].vect1.z, &vects[num].vect2.x, &vects[num].vect2.y, &vects[num].vect2.z);
		vects[num].vect3 = vects[num].vect1.project(vects[num].vect2);
	}

}

//��ȡtest.txt�ı�
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
		printf("δ�ҵ�test.txt�ļ���\n");
		system("Pause");
		exit(1);
	}
}

//д��out.txt�ı�
void writeTxt() {
	ofstream out("out.txt", ios::trunc);
	for (int i = 0; i < cmd_num; i += 2) {
		out << setiosflags(ios::fixed);
		out << operate[i] << endl;
		if (operate[i] == "�������" || operate[i] == "������ģ") {
			out << operate[i + 1] << "\t" << setprecision(2) << vects[i / 2].result << endl;
		}
		else{
			out << operate[i + 1] << "\t" << vects[i / 2].vect3.toString() << endl;
		}
	}
	out.close();
}

//Ԥ����,������д�ļ�,���������������
void Pretreatment() {
	readTxt();//��ȡtxt�е������ı�

	for (command = 0; command < cmd_num; command += 2) {
		vectOperation(command / 2);
	}
	result_num = command / 2;
	writeTxt();
}

int main(int argc, char *argv[]){
	Pretreatment();//Ԥ�ȼ��㲢�����ı�����������

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1280, 720);
	glutCreateWindow("ͼ��ѧ2_1120142053_�쳿��");
	glutDisplayFunc(&myDisplay);
	glutTimerFunc(1, myTimerFunc, 0);
	glutReshapeFunc(&myReshape);
	glutKeyboardFunc(&myKeyboardFunc);
	SetRC();
	glutMainLoop();
	return 0;
}