// 001.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <GL/glut.h>
#include "math.h"
#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>

//====================================
//ȫ�ֱ������岿��(����������ʵ��޸�)
//====================================
const unsigned int TIME = 200;//����ÿһ֡ʱ������λΪ���룬Ĭ��Ϊ200ms.
const int STAR_NUM = 10000;//������������Ĭ��Ϊ10000��.
int width = 1280, height = 720;//���ǻ��ƴ���Ĭ�ϴ�СΪ1280x720�����к���������.
int mode = 0;//��������ʱ��Ĭ��״̬��0����˸���� 1���������� 2�����Ǵ�.

//=====================
//������������
//=====================

float starDis(struct star a, struct star b);
void initStar();
void SetRC();
void drawStar();
void drawLine();
void drawTriangle();
void myDisplay(void);
void myReshape(int w, int h);
void myTimerFunc(int val);
void myKeyboardFunc(unsigned char key, int x, int y);

//=================
//*���²����������
//=================
bool draw_star = 1;//�����ǿ��Ʊ���   

struct star {
	float x, y, z;
	float color, size;
};

struct star stars[STAR_NUM];
struct star * now_star = &stars[0];//��ָ��ָ���һ������

//=====================
//����ʵ�ֲ���
//=====================

//�������Ǿ��뺯��
float starDis(struct star a, struct star b) {
	return (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) + (a.z - b.z)*(a.z - b.z);;
}

//��ʼ�����Ǻ���
void initStar() {
	int tmp_j;
	float tmp_dis = FLT_MAX;
	struct star tmp_star;

	for (int i = 0; i < STAR_NUM; i++) {
		stars[i].x = rand() / double(RAND_MAX) * width - width / 2;
		stars[i].y = rand() / double(RAND_MAX) * height - height / 2;
		stars[i].z = rand() / double(RAND_MAX) * 1000;
		stars[i].color = stars[i].z / 1000;
		stars[i].size = stars[i].z / 3000;
	}
	stars[0].x = stars[0].y = stars[0].z = 0;
	//�����ǽ���ѡ������
	for (int i = 0; i < STAR_NUM - 1; i++) {
		tmp_dis = FLT_MAX;
		for (int j = i + 1; j < STAR_NUM; j++) {
			float dis = starDis(stars[i], stars[j]);
			if (dis < tmp_dis) {
				tmp_dis = dis;
				tmp_j = j;
			}
		}
		tmp_star = stars[tmp_j];
		stars[tmp_j] = stars[i + 1];
		stars[i + 1] = tmp_star;
	}
}

//�������ú���
void SetRC(){
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT);
	glFrontFace(GL_CW);
	glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_LINE);

	srand((unsigned)time(NULL));

	printf("���ո����л�����״̬������˸�����ǣ���������������ߣ�ȡ��������ǻ����Ǵ�\n");
}

//һ��һ��������
void drawStar() {
	glPushMatrix();
	glTranslatef(0, 0, -1000);
	glBegin(GL_POINTS);
	for (int i = 0; i < STAR_NUM; i++) {
		//ǧ��֮5�ļ�������գ��
		if (rand() % 1000 > 4) {
			glColor3f(stars[i].color, stars[i].color, stars[i].color);
			glPointSize(stars[i].size);
			glVertex3f(stars[i].x, stars[i].y, stars[i].z);
		}
	}
	glEnd();
	glPopMatrix();
}

//���߶�
void drawLine() {
	glPushMatrix();
	glTranslatef(0, 0, -1000);
	glBegin(GL_LINE_STRIP);
	glVertex3f(now_star->x, now_star->y, now_star->z);
	now_star++;
	glVertex3f(now_star->x, now_star->y, now_star->z);
	glEnd();
	glPopMatrix();
}

//�����Ǵ�
void drawTriangle() {
	glPushMatrix();
	glTranslatef(0, 0, -1000);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(now_star->x, now_star->y, now_star->z);
	now_star++;
	glVertex3f(now_star->x, now_star->y, now_star->z);
	now_star++;
	glVertex3f(now_star->x, now_star->y, now_star->z);
	glEnd();
	glPopMatrix();
}

//�ܻ��ƿ��ƺ���
void myDisplay(void){
	if (draw_star == 1) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	if (mode == 0) {
		drawStar();
	}
	else if (mode == 1 && now_star < &stars[STAR_NUM - 1]) {
		if (draw_star == 1) {
			drawStar();
		}
		draw_star = 0;//����ֻ��һ��
		drawLine();
	}
	else if (mode == 2 && now_star < &stars[STAR_NUM - 1]) {
		if (draw_star == 1) {
			drawStar(); 
			drawTriangle();
		}
		draw_star = 0;//����ֻ��һ��
		now_star--;
		drawTriangle();
	}
	glFlush();
}

//���Ĵ��ڴ�Сʱ�����ü��̿��ƺ������ػ�ͼ��
void myReshape(int w, int h) {
	width = w;
	height = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, GLfloat(w) / h, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//���ü��̿��ƺ���������״̬
	mode = (mode + 2) % 3;
	myKeyboardFunc(' ', 0, 0);
}

//����֡��������
void myTimerFunc(int val){
	myDisplay();
	glutTimerFunc(TIME, myTimerFunc, 0);
}

//���̲�������
void myKeyboardFunc(unsigned char key, int x, int y) {
	if(key == 32){
		mode = (mode + 1) % 3;
		if (mode == 0) {
			printf("��ǰ����״̬��������\n");
		}
		else if (mode == 1) {
			printf("��ǰ����״̬������̾�����������\n");
		}
		else if (mode == 2) {
			printf("��ǰ����״̬��ȡ��������ǻ����Ǵ�\n");
		}
		draw_star = 1;
		now_star = &stars[0];
	}
}

//=======================
//����������
//=======================
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("ͼ��ѧ1_1120142053_�쳿��");
	initStar();
	glutDisplayFunc(&myDisplay);
	glutTimerFunc(TIME, myTimerFunc, 0);
	glutReshapeFunc(&myReshape);
	glutKeyboardFunc(&myKeyboardFunc);
	SetRC();
	glutMainLoop();
	return 0;
}