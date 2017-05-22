#include "stdafx.h"
#include <GL/glew.h>
#include "CmyOpenGL.h"
#pragma comment(lib,"glew32.lib ")

//====================================
//全局变量定义部分(可依据情况适当修改)
//====================================
int width = 1280, height = 720;						//星星绘制窗口默认大小为1280x720
const int STAR_NUM = 10000;							//星星总数量，默认为10000个
const int SMOOTH = 100;								//视点过渡平滑程度，数值越大越平滑，默认为100
const double AIR_PLANE_SIZE = 3;					//飞机大小
const double AIR_PLANE_PROP = 10;					//飞机长宽比例
int TAIL_LEN = AIR_PLANE_SIZE*AIR_PLANE_PROP * 100;	//小尾巴长度+1，默认为飞机长度100倍
float view_mspeed = 0.1, view_rspeed = 0.05;		//视点初始移动速度和旋转速度
float ship_mspeed = 0.1, ship_rspeed = 0.5;			//飞机初始移动速度和旋转速度
CglVector3 startAt(10, 20, 20);						//设定视点起始位置

int view_mode = 0;									//程序运行时的视点状态：0欧拉视角 1自由视点控制
int view_pos = 0;									//程序运行时视点位置: 0自由变换 1跟随飞机
int flyMode = 0;									//程序运行时的飞行状态：0手动控制 1跟随星星航行

//=========================
//全局变量定义部分(不可更改)
//=========================

CglVector3 stars[STAR_NUM];				//星空
cycleQueue<CglVector3> tail(TAIL_LEN);	//可爱的小尾巴
CglVector3 *nowStar = stars;			//当前星星指针

int usetime;
int step = SMOOTH;						//控制平滑插值参数
double t[SMOOTH];						//平滑参量数组
CglQuaternion slerpResult[SMOOTH];		//平滑插值结果四元数数组
CglVector3 posResult[SMOOTH];			//平滑插值位置结果数组
bool trick = false;						//特效动作状态控制
bool toNextStar = false;				//转向下一个星星控制

//==============
//绘制函数实现部分
//==============

//计算两点距离函数
float posDis(CglVector3 a, CglVector3 b) {
	return (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) + (a.z - b.z)*(a.z - b.z);;
}

//对星星进行排序
void sortStars() {
	stars[0] = CglVector3(0, 0, 0);
	CglVector3 tmp_star;
	float tmp_dis = FLT_MAX;
	int tmp_j;
	for (int i = 0; i < STAR_NUM - 1; i++) {
		tmp_dis = FLT_MAX;
		tmp_j = i + 1;
		for (int j = i + 1; j < STAR_NUM; j++) {
			float dis = posDis(stars[i], stars[j]);
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
	
	sortStars();
}

//设置环境
void SetRC() {

	//星星是圆的(由于我的显卡兼容性问题，与控制星星大小变换冲突所以注视掉了)
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
	for (int i = 0; i < tail.nowSize() - 1; i++) {
		float color;
		glLineWidth(1);
		//当尾巴长度达到最长的时候，控制最后20%的尾巴渐渐消失
		if (tail.nowSize() == tail.size() - 1 && i < tail.size() / 5.0) {
			color = i / (tail.size() / 5.0);
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

//画星迹线
void drawStarLine() {
	CglVector3 *pStar = stars;

	glPushMatrix();
	glLineWidth(1);
	glColor3f(1.0, 1.0, 0);
	glBegin(GL_LINE_STRIP);
	do{
		glVertex3f(pStar->x, pStar->y, pStar->z);
		glVertex3f((pStar + 1)->x, (pStar + 1)->y, (pStar + 1)->z);
		pStar++;
	} while (pStar != nowStar);
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
	//设置飞船初始速度
	airPlane.SetSpeed(ship_mspeed, ship_rspeed);
}

//总绘制函数
void CmyOpenGL::InDraw() {
	//调试用坐标轴
	m_pShape->DrawCoord(1000);
	//画模型
	DrawModel();
	
	//参数显示
	/*CString str;
	glColor3f(1, 0, 0);
	str.Format("%lf, %lf, %lf", (nowStar + 1)->x, (nowStar + 1)->y, (nowStar + 1)->z);
	m_pFont->Font2D(str.GetBuffer(0), -0.9, 0.9, 7);*/
}

//更新每一帧绘图的数据
void CmyOpenGL::Update() {
	usetime = airPlane.GetUseTime();

	//向前, 正方向, 画尾巴
	float movdis = airPlane.Move(2, 1, usetime);

	//沿着星迹线飞行
	if (flyMode == 1) {
		if (toNextStar == true) {
			toNextStar = false;
			airPlane.SetDir(*nowStar - airPlane.m_pos);
		}
		//飞机距离目标星星0.1时，认为飞机已经达到目标星星
		if (posDis(*nowStar, airPlane.m_pos) <= movdis*2) {
			toNextStar = true;
			nowStar++;
		}
	}
	
	//视点如果跟随飞机
	if (view_pos == 1) {
		CglVector3 follow_pos = airPlane.m_pos - airPlane.m_dir * 30 + airPlane.m_updir * 15;
		m_pCamere->followCamera(follow_pos, airPlane.m_pos, airPlane.m_updir);
	}

	//尾巴跟随
	for (int i = 0; i < movdis; i++) {
		if (tail.isFull()) {
			tail.pop();
		}
		tail.push(airPlane.m_pos);
	}

	//控制特技
	if (trick == true) {
		airPlane.Trick(usetime);
	}
}

bool CmyOpenGL::OnKey(unsigned int nChar, bool bDown) {
	if (bDown)
		switch (nChar) {

		//变换飞行模式, 0为手动控制, 1为跟随星星航行
		case VK_F1:
			flyMode = 1 - flyMode;
			toNextStar = true;
			nowStar++;
			break;

		//变换视点模式, 0为欧拉角视图, 1为自由变换视角
		case VK_F2:
			view_mode = 1 - view_mode;
			m_pCamere->m_type = view_mode;
			break;

		//飞船自动飞行, 加速, [,<]键
		case VK_OEM_COMMA:
			ship_mspeed += 0.1;
			airPlane.SetSpeed(ship_mspeed, ship_rspeed);
			break;
		//飞船自动飞行, 减速, [.>]键
		case VK_OEM_PERIOD:
			ship_mspeed -= 0.1;
			airPlane.SetSpeed(ship_mspeed, ship_rspeed);
			break;

		//飞船特技[/?]键
		case VK_OEM_2:
			trick = (trick == true) ? false : true;
			break;

		//跟踪视点变换, [space]键
		case VK_SPACE:
			view_pos = 1 - view_pos;
			break;

		//飞船飞行方向控制
		case VK_LEFT:
			airPlane.Rotate(0, 1, usetime);//向左，正方向
			break;
		case VK_RIGHT:
			airPlane.Rotate(0, -1, usetime);//向左，负方向（向右）
			break;
		case VK_UP:
			airPlane.Rotate(1, 1, usetime);//向上，正方向
			break;
		case VK_DOWN:
			airPlane.Rotate(1, -1, usetime);//向上，负方向（向下）
			break;
		}
	return false;
}

void CmyOpenGL::DrawModel() {
	drawStar();//画星星
	drawTail();//画尾巴
	//画飞机
	airPlane.Draw(AIR_PLANE_SIZE, AIR_PLANE_PROP);
	//画星迹线
	if (flyMode == 1) {
		drawStarLine();
	}
}
