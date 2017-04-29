#include "stdafx.h"
#include <GL/glew.h>
#include "CmyOpenGL.h"
#pragma comment(lib,"glew32.lib ")

//====================================
//全局变量定义部分(可依据情况适当修改)
//====================================
int width = 1280, height = 720;		//星星绘制窗口默认大小为1280x720
const int STAR_NUM = 10000;			//星星总数量，默认为10000个
const int TAIL_LEN = 121;			//小尾巴长度+1，默认为120米
const int SMOOTH = 100;				//视点过渡平滑程度，数值越大越平滑，默认为100
float mspeed = 0.01, rspeed = 0.05;	//移动速度和旋转速度
CglVector3 startAt(100, 200, 200);	//设定视点起始位置
float size = 3;						//飞机大小
int mode = 0;						//程序运行时的视点状态：0：自由变换 1：跟随并控制飞机

//=========================
//全局变量定义部分(不可更改)
//=========================

CglVector3 stars[STAR_NUM];
//可爱的小尾巴
cycleQueue<CglVector3> tail(TAIL_LEN);

//==============
//绘制函数实现部分
//==============

//计算星星距离函数
float starDis(CglVector3 a, CglVector3 b) {
	return (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) + (a.z - b.z)*(a.z - b.z);;
}

//对星星进行排序
void sortStars() {
	stars[0] = startAt;
	CglVector3 tmp_star;
	float tmp_dis = FLT_MAX;
	int tmp_j;
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

//初始化星星函数
void initStar() {
	for (int i = 0; i < STAR_NUM; i++) {
		stars[i].x = rand() / double(RAND_MAX) * width * 32 - width * 16;
		stars[i].y = rand() / double(RAND_MAX) * height * 32 - height * 16;
		stars[i].z = rand() / double(RAND_MAX) * 32000 - 16000;
	}
	
	//sortStars();
}

//设置环境
void SetRC() {

	//星星是圆的(与控制大小变换冲突所以注视掉了)
	//glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glShadeModel(GL_FLAT);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_BACK, GL_LINE);

	srand((unsigned)time(NULL));

	//控制星星随着视点远近变换大小
	glewInit();
	float Quadratic[] = { 0,0.1,0 };
	glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, Quadratic);
	glPointParameterf(GL_POINT_SIZE_MIN, 1);//设置最小点
	glPointParameterf(GL_POINT_SIZE_MAX, 100);//设置最大点
}

//画星星
void drawStar() {
	glColor3f(1.0, 1.0, 1.0);
	glPointSize(3);
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
		} else {
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

//画一个简易小飞机
void drawPlane(CglCamera* m_pCamere) {
	//飞机前后长度和左右宽度的比例
	double ship_prop = 10;

	glPushMatrix();
	//调整飞机与视点前后距离
	CglVector3 fore_aft_dis;
	fore_aft_dis.Set(&m_pCamere->m_viewMatrixInverse[8]);
	fore_aft_dis = fore_aft_dis * 8;
	//调整飞机与视点上下距离
	CglVector3 up_down_dis;
	up_down_dis.Set(&m_pCamere->m_viewMatrixInverse[4]);
	up_down_dis = up_down_dis * 5;
	//最终调整向量
	CglVector3 plane_pos;
	plane_pos = m_pCamere->m_pos - fore_aft_dis - up_down_dis;
	//调整飞机位置姿势
	glTranslatef(plane_pos.x, plane_pos.y, plane_pos.z);
	glRotatef(m_pCamere->m_hpr.x, 0, 1, 0);
	glRotatef(m_pCamere->m_hpr.y, 1, 0, 0);
	glRotatef(m_pCamere->m_hpr.z, 0, 0, 1);
	
	//简易飞机(箭头)初始方向为y轴正方向，调整与视点方向负z轴一致
	glRotatef(-90, 1, 0, 0);
	//四个侧面
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, size * ship_prop, 0.0f);
	glVertex3f(-size, -size, size);
	glVertex3f(size, -size, size);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, size * ship_prop, 0.0f);
	glVertex3f(size, -size, size);
	glVertex3f(size, -size, -size);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, size * ship_prop, 0.0f);
	glVertex3f(size, -size, -size);
	glVertex3f(-size, -size, -size);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, size * ship_prop, 0.0f);
	glVertex3f(-size, -size, -size);
	glVertex3f(-size, -size, size);
	glEnd();
	//底面
	glBegin(GL_QUADS);
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(-size, -size, size);
	glVertex3f(-size, -size, -size);
	glVertex3f(size, -size, -size);
	glVertex3f(size, -size, size);
	glEnd();

	glPopMatrix();
}

//==================
//COpenGL重载实现部分
//==================

CmyOpenGL::CmyOpenGL() {

}

CmyOpenGL::~CmyOpenGL() {

}

//程序初始化时调用
void CmyOpenGL::PostInit() {
	m_pControl->SetSpeed(mspeed, rspeed);

	glClearColor(0, 0, 0, 1);
	m_pCamere->SetCamera(startAt, CglVector3(0, 0, 0), true);
	m_pCamere->SaveCamera();

	//设置环境
	SetRC();
	//初始化星星
	initStar();
}

//总绘制函数
void CmyOpenGL::InDraw() {
	//调试用坐标轴
	m_pShape->DrawCoord(1000);
	//画模型
	DrawModel();
	
	CString str;
	glColor3f(1, 0, 0);
	str.Format("%.2f,%.2f,%.2f", m_pCamere->m_pos.x, m_pCamere->m_pos.y, m_pCamere->m_pos.z);
	m_pFont->Font2D(str.GetBuffer(0), -0.9, 0.9, 7);
}

bool CmyOpenGL::OnKey(unsigned int nChar, bool bDown) {
	if (nChar == 'w' || nChar == 's' || nChar == 'a' || nChar == 'd' || nChar == 'q' || nChar == 'e'||
		nChar == 'W' || nChar == 'S' || nChar == 'A' || nChar == 'D' || nChar == 'Q' || nChar == 'E') {
		if (tail.isFull()) {
			tail.pop();
		}
		tail.push(m_pCamere->m_pos);
	}
	if (bDown)
		switch (nChar) {
		case VK_F1:
			//调用键盘控制函数以重置状态
			mode = (mode + 1) % 2;
			break;
			
		}
	return false;
}

void CmyOpenGL::DrawModel() {
	//SetView();
	//画星星
	drawStar();
	//画飞机
	drawPlane(m_pCamere);
	//画尾巴
	drawTail();
}
