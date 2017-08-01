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
//float ship_max_mspeed = 1;							//飞机初始最大移动速度
CglVector3 startAt(10, 20, 20);						//设定视点起始位置

int view_mode = 0;									//程序运行时的视点状态：0欧拉视角 1自由视点控制
int view_pos = 1;									//程序运行时视点位置: 0自由变换 1跟随飞机

//=========================
//全局变量定义部分(不可更改)
//=========================

CglVector3 stars[STAR_NUM];				//星空
cycleQueue<CglVector3> tail(TAIL_LEN);	//可爱的小尾巴

int usetime;
int step = SMOOTH;						//控制平滑插值参数
double t[SMOOTH];						//平滑参量数组
CglQuaternion slerpResult[SMOOTH];		//平滑插值结果四元数数组
CglVector3 posResult[SMOOTH];			//平滑插值位置结果数组
GLuint texturePlane;					//飞机贴图
CglMaterial materialPlane;				//飞机材料
GLuint textureSky;						//天空贴图
CglMaterial materialSky;				//天空材料
GLuint textureGarbage;					//天空贴图
CglMaterial materialGarbage;			//天空材料
GLuint textureTail;						//尾焰贴图
CglMaterial materialTail;				//尾焰材料
GLuint texturePatch;					//碎片贴图
CglMaterial materialPatch;	    		//碎片材料


SkyBox mybox;							//天空盒
CglPlane airPlane;
list<CglGarbage> garbageList;			//垃圾控制类
CglPatch *patch;						//垃圾碎片类
vector<CglPatch> patchList;				//垃圾碎片数组

//==============
//绘制函数实现部分
//==============

//设置环境
void SetRC() {

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glShadeModel(GL_FLAT);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);

	srand((unsigned)time(NULL));

	glEnable(GL_LIGHTING);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, sunambient);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, sunpos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, suncolor);
	glLightfv(GL_LIGHT0, GL_AMBIENT, sunambient);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

//==================
//Garbage链表部分
//==================

//列表中增加垃圾
void addGarbage(int num) {
	while (num--) {
		CglGarbage* garbage = new CglGarbage();
		garbageList.push_back(*garbage);
	}
}

//画垃圾列表
void drawGarbage() {
	if (!garbageList.size()) return;
	list<CglGarbage>::iterator it;
	for (it = garbageList.begin(); it != garbageList.end(); it++) {
		it->draw(2, *patch, &patchList);
	}
}

//判断垃圾与飞船距离, 远离返回1, 靠近返回0
int garbageJudge(CglVector3 garbagePos, CglVector3 airPlanePos) {
	float disX = garbagePos[0] - airPlanePos[0];
	float disY = garbagePos[1] - airPlanePos[1];
	float disZ = garbagePos[2] - airPlanePos[2];
	float dis = disX * disX + disY * disY + disZ * disZ;
	if (dis > airPlane.range) {
		return 1;
	}
	return 0;
}

//更新垃圾列表, 更新每一个垃圾的位置, 以及是否存在
void updateGarbage(CglVector3 airPlanePos) {
	list<CglGarbage>::iterator it;
	for (it = garbageList.begin(); it != garbageList.end(); it++) {
		int usetime = it->GetUseTime();
		it->Move(2, 1, usetime);
		it->Rotate(it->r_dir, it->r_v, 10);
		if (!garbageJudge(it->m_pos, airPlanePos)) {
			it->status = 0;
			patch->Reset(&patchList, it->m_pos, airPlane.m_dir, airPlane.GetMSpeed());
			airPlane.speed_Max += 1;
			//计算飞机分数
			airPlane.score += it->point;
			if ((airPlane.score) % 10 == 0) {
				airPlane.range += 1;
			}
		}
	}
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
	//设置视点初始速度
	m_pControl->SetSpeed(view_mspeed, view_rspeed);
	//初始化尾迹平滑参量
	for (int i = 0; i < SMOOTH; i++) {
		t[i] = i / SMOOTH;
	}
	glClearColor(0, 0, 0, 1);
	m_pCamere->SetCamera(startAt, CglVector3(0, 0, 0), true);
	//设置环境
	SetRC();

	//加载天空盒纹理
	textureSky = materialSky.LoadGLTextures("sky.bmp");

	//初始化飞机纹理
	texturePlane = materialPlane.LoadGLTextures("plane.bmp"); 

	//初始化垃圾纹理
	textureGarbage = materialGarbage.LoadGLTextures("garbage.bmp");
	//初始化垃圾
	addGarbage(1000);

	//初始化尾焰
	//textureTail = materialTail.LoadGLTextures("flame.bmp");

	//初始化碎片
	patch = new CglPatch();
	patch->Init(&patchList, 100);

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
	CString str;
	glColor3f(1, 0, 0);

	str.Format("Score : %d", airPlane.score);
	m_pFont->Font2D(str.GetBuffer(0), -0.9, 0.9, 7);
	
	str.Format("AirPlane Speed : %0.2f", airPlane.GetMSpeed()*10);
	m_pFont->Font2D(str.GetBuffer(0), -0.9, 0.82, 7);

	str.Format("AirPlane Max Speed : %0.2f", airPlane.speed_Max);
	m_pFont->Font2D(str.GetBuffer(0), -0.9, 0.74, 7);

	str.Format("AirPlane Pos : (%0.2f,%0.2f,%0.2f)",airPlane.m_pos.x, airPlane.m_pos.y, airPlane.m_pos.z);
	m_pFont->Font2D(str.GetBuffer(0), -0.9, 0.66, 7);

	str.Format("AirPlane Range : %0.2f", airPlane.range);
	m_pFont->Font2D(str.GetBuffer(0), -0.9, 0.58, 7);
}

//更新每一帧绘图的数据
void CmyOpenGL::Update() {

	//更新飞机状态
	usetime = airPlane.GetUseTime();
	float movdis = airPlane.Move(2, 1, usetime);

	//更新垃圾状态
	updateGarbage(airPlane.m_pos);
	patch->Update(&patchList);

	//视点如果跟随飞机
	if (view_pos == 1) {
		CglVector3 follow_pos = airPlane.m_pos - airPlane.m_dir * 7 + airPlane.m_updir * 3;
		m_pCamere->followCamera(follow_pos, airPlane.m_pos, airPlane.m_updir); 
	}

	//尾巴跟随
	for (int i = 0; i < movdis; i++) {
		if (tail.isFull()) {
			tail.pop();
		}
		tail.push(airPlane.m_pos);
	}
}

bool CmyOpenGL::OnKey(unsigned int nChar, bool bDown) {
	if (bDown)
		switch (nChar) {
		//变换视点模式, 0为欧拉角视图, 1为自由变换视角
		case VK_F2:
			view_mode = 1 - view_mode;
			m_pCamere->m_type = view_mode;
			break;

		//跟踪视点变换, [space]键
		case VK_SPACE:
			view_pos = 1 - view_pos;
			break;

		//飞船自动飞行, 加速, [,<]键
		case VK_OEM_COMMA:
			if (ship_mspeed < airPlane.speed_Max) {
				ship_mspeed += 0.1;
			}
			airPlane.SetSpeed(ship_mspeed, ship_rspeed);
			break;
		//飞船自动飞行, 减速, [.>]键
		case VK_OEM_PERIOD:
			ship_mspeed -= 0.1;
			if (ship_mspeed <= 0.1) {
				ship_mspeed = 0.1;
			}
			airPlane.SetSpeed(ship_mspeed, ship_rspeed);
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
	drawTail();//画尾巴

	//画天空盒，（半径，经度，纬度）
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureSky);
	mybox.DrawBall(3000, 30, 30);
	glDisable(GL_TEXTURE_2D);

	//画飞机
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturePlane);
	//如果飞机到中心点距离大于2000，飞机速度取0
	if ((airPlane.m_pos.len() - CglVector3(0, 0, 0).len()) > 2000) {
		airPlane.SetSpeed(0,0.01);
		//airPlane.m_dir = -airPlane.m_dir;
		//airPlane.m_pos = CglVector3(0, 0, 0);
	}
	airPlane.Draw();
	glDisable(GL_TEXTURE_2D);

	//画垃圾
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureGarbage);
	drawGarbage();
	glDisable(GL_TEXTURE_2D);
}
