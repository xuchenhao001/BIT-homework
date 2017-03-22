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
//全局变量定义部分(可依据情况适当修改)
//====================================
const unsigned int TIME = 200;//动画每一帧时长，单位为毫秒，默认为200ms.
const int STAR_NUM = 10000;//星星总数量，默认为10000个.
int width = 1280, height = 720;//星星绘制窗口默认大小为1280x720，运行后可任意更改.
int mode = 0;//程序运行时的默认状态：0：闪烁星星 1：星星连线 2：三角带.

//=====================
//函数声明部分
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
//*以下参数请勿更改
//=================
bool draw_star = 1;//画星星控制变量   

struct star {
	float x, y, z;
	float color, size;
};

struct star stars[STAR_NUM];
struct star * now_star = &stars[0];//该指针指向第一个星星

//=====================
//函数实现部分
//=====================

//计算星星距离函数
float starDis(struct star a, struct star b) {
	return (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) + (a.z - b.z)*(a.z - b.z);;
}

//初始化星星函数
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
	//对星星进行选择排序
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

//环境设置函数
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

	printf("按空格以切换三种状态：画闪烁的星星，将最近的星星连线，取最近的星星画三角带\n");
}

//一闪一闪画星星
void drawStar() {
	glPushMatrix();
	glTranslatef(0, 0, -1000);
	glBegin(GL_POINTS);
	for (int i = 0; i < STAR_NUM; i++) {
		//千分之5的几率星星眨眼
		if (rand() % 1000 > 4) {
			glColor3f(stars[i].color, stars[i].color, stars[i].color);
			glPointSize(stars[i].size);
			glVertex3f(stars[i].x, stars[i].y, stars[i].z);
		}
	}
	glEnd();
	glPopMatrix();
}

//画线段
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

//画三角带
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

//总绘制控制函数
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
		draw_star = 0;//星星只画一次
		drawLine();
	}
	else if (mode == 2 && now_star < &stars[STAR_NUM - 1]) {
		if (draw_star == 1) {
			drawStar(); 
			drawTriangle();
		}
		draw_star = 0;//星星只画一次
		now_star--;
		drawTriangle();
	}
	glFlush();
}

//更改窗口大小时，调用键盘控制函数以重画图像
void myReshape(int w, int h) {
	width = w;
	height = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, GLfloat(w) / h, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//调用键盘控制函数以重置状态
	mode = (mode + 2) % 3;
	myKeyboardFunc(' ', 0, 0);
}

//控制帧动画函数
void myTimerFunc(int val){
	myDisplay();
	glutTimerFunc(TIME, myTimerFunc, 0);
}

//键盘操作函数
void myKeyboardFunc(unsigned char key, int x, int y) {
	if(key == 32){
		mode = (mode + 1) % 3;
		if (mode == 0) {
			printf("当前处于状态：画星星\n");
		}
		else if (mode == 1) {
			printf("当前处于状态：将最短距离星星连线\n");
		}
		else if (mode == 2) {
			printf("当前处于状态：取最近的星星画三角带\n");
		}
		draw_star = 1;
		now_star = &stars[0];
	}
}

//=======================
//主函数部分
//=======================
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("图形学1_1120142053_徐晨浩");
	initStar();
	glutDisplayFunc(&myDisplay);
	glutTimerFunc(TIME, myTimerFunc, 0);
	glutReshapeFunc(&myReshape);
	glutKeyboardFunc(&myKeyboardFunc);
	SetRC();
	glutMainLoop();
	return 0;
}