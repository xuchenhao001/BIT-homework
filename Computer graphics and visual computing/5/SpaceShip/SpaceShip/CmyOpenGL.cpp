#include "stdafx.h"
#include <GL/glew.h>
#include "CmyOpenGL.h"
#pragma comment(lib,"glew32.lib ")

//====================================
//全局变量定义部分(可依据情况适当修改)
//====================================
int width = 1280, height = 720;					//星星绘制窗口默认大小为1280x720
const int STAR_NUM = 10000;						//星星总数量，默认为10000个
const int TAIL_LEN = 121;						//小尾巴长度+1，默认为120米
const int SMOOTH = 100;							//视点过渡平滑程度，数值越大越平滑，默认为100
const double AIR_PLANE_SIZE = 3;				//飞机大小
float view_mspeed = 0.1, view_rspeed = 0.05;	//视点初始移动速度和旋转速度
float ship_mspeed = 0.1, ship_rspeed = 0.5;		//飞机初始移动速度和旋转速度
CglVector3 startAt(10, 20, 20);					//设定视点起始位置

int view_mode = 0;								//程序运行时的视点状态：0欧拉视角 1自由视点控制
int view_pos = 0;								//程序运行时视点位置: 0自由变换 1跟随飞机
int fly_mode = 0;								//程序运行时的飞行状态：0手动控制 1跟随星星航行

//=========================
//全局变量定义部分(不可更改)
//=========================

CglVector3 stars[STAR_NUM];				//星空
cycleQueue<CglVector3> tail(TAIL_LEN);	//可爱的小尾巴
CglVector3 *now_star = stars;			//当前星星指针

int step = SMOOTH;						//控制平滑插值参数
double t[SMOOTH];						//平滑参量数组
CglQuaternion slerpResult[SMOOTH];		//平滑插值结果四元数数组
CglVector3 posResult[SMOOTH];			//平滑插值位置结果数组

CglVector3 follow_pos;
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

//==================
//COpenGL重载实现部分
//==================

CmyOpenGL::CmyOpenGL() {

}

CmyOpenGL::~CmyOpenGL() {

}

//程序初始化时调用
void CmyOpenGL::PostInit() {
	m_pControl->SetSpeed(view_mspeed, view_rspeed);//设置视点初始速度
	//初始化平滑参量
	for (int i = 0; i < SMOOTH; i++) {
		t[i] = i / SMOOTH;
	}

	glClearColor(0, 0, 0, 1);
	m_pCamere->SetCamera(startAt, CglVector3(0, 0, 0), true);

	//设置环境
	SetRC();
	//初始化星星
	initStar();
	//初始化飞机
	airPlane.Init();
	airPlane.SetSpeed(ship_mspeed, ship_rspeed);//设置飞船初始速度
}

//总绘制函数
void CmyOpenGL::InDraw() {
	//调试用坐标轴
	m_pShape->DrawCoord(1000);
	//画模型
	DrawModel();
	
	CString str;
	glColor3f(1, 0, 0);
	str.Format("%.2f, %.2f, %.2f", follow_pos.x, follow_pos.y, follow_pos.z);
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

		//变换飞行模式, 0为手动控制, 1为跟随星星航行
		case VK_F1:
			//fly_mode = 1 - fly_mode;

			break;

		//变换视点模式, 0为欧拉角视图, 1为自由变换视角
		case VK_F2:
			view_mode = 1 - view_mode;
			m_pCamere->SaveCamera();
			m_pCamere->m_type = view_mode;
			m_pCamere->LoadCamera();
			break;

		//飞船自动飞行, 加速
		case VK_OEM_COMMA:
			ship_mspeed += 0.1;
			airPlane.SetSpeed(ship_mspeed, ship_rspeed);
			break;
		//飞船自动飞行, 减速
		case VK_OEM_PERIOD:
			ship_mspeed -= 0.1;
			airPlane.SetSpeed(ship_mspeed, ship_rspeed);
			break;

		//跟踪视点变换
		case VK_SPACE:
			view_pos = 1 - view_pos;
			break;

		//飞船飞行方向控制
		case VK_LEFT:
			airPlane.Rotate(0, 1);//向左，正方向
			break;
		case VK_RIGHT:
			airPlane.Rotate(0, -1);//向左，负方向（向右）
			break;
		case VK_UP:
			airPlane.Rotate(1, 1);//向上，正方向
			break;
		case VK_DOWN:
			airPlane.Rotate(1, -1);//向上，负方向（向下）
			break;
		}
	return false;
}

void CmyOpenGL::DrawModel() {
	drawStar();//画星星
	drawTail();//画尾巴

	//画前进轨迹
	/*if (view_mode == 1 && step == 0) {
		drawLine();
	}*/

	//画飞机
	airPlane.Draw(AIR_PLANE_SIZE);
	//airPlane.Move(2, 1);//向前，正方向

	//视点是否跟随飞机
	if (view_pos == 1) {
		follow_pos = airPlane.m_pos - airPlane.m_dir * 30 + airPlane.m_updir * 15;
		m_pCamere->followCamera(follow_pos, airPlane.m_pos, airPlane.m_matrix);
	}
	
}
