#include "stdafx.h"
#include "CglPlane.h"

CglPlane::CglPlane() {

}

CglPlane::~CglPlane() {

}

void APIENTRY glu_vertex(GLdouble *xyz) {
	glTexCoord2dv(xyz + 3);
	glVertex3dv(xyz);
}

void CglPlane::Init(void)
{
	m_dir = CglVector3(0, 0, -1);
	m_pos = CglVector3(0, 0, 0);
	m_updir = CglVector3(0, 1, 0);
	m_matrix.Identity();
	scale_step[0] = 0.01;
	scale_step[1] = 0.01;
	key_step = 3;
	speed_Max = 1;
	score = 0;
	range = 10;

	//初始化飞机尾焰
	m_particle.InitTail(1000, "flame2.bmp");
	//初始化飞机名字
	str.Format("%s", "FUTURE");
	m_pFont = new CglFont();

	int                i;
	GLUquadric         *quadric;
	GLUtesselator      *tess;

	//主翼位置
	static GLdouble    wing[][5] =
	{
		/* { x, y, z, s, t } */
		{ 0.25, 0.0, -1.0,  0.125, 0.0 },
		{ 0.45, 0.0,  0.0,  0.25,  0.4 },
		{ 1.65, 0.0,  0.8,  1.0,   0.8 },
		{ 1.65, 0.0,  1.2,  1.0,   1.0 },
		{ 0.35, 0.0,  1.2,  0.15,  1.0 },
		{ 0.35, 0.0,  2.4,  0.15,  2.0 },
		{ 0.25, 0.0,  2.4,  0.125, 2.0 },
		{ 0.25, 0.0,  2.0,  0.125, 1.5 },
		{ -0.25, 0.0,  2.0, -0.125, 1.5 },
		{ -0.25, 0.0,  2.4, -0.125, 2.0 },
		{ -0.35, 0.0,  2.4, -0.15,  2.0 },
		{ -0.35, 0.0,  1.2, -0.15,  1.0 },
		{ -1.65, 0.0,  1.2, -1.0,   1.0 },
		{ -1.65, 0.0,  0.8, -1.0,   0.8 },
		{ -0.45, 0.0,  0.0, -0.25,  0.4 },
		{ -0.25, 0.0, -1.0, -0.125, 0.0 }
	};
	//尾翼位置
	static GLdouble    tail[][5] = 
	{
		/* { x, y, z, s, t } */
		{ 0.0, 0.24, 0.5, 1.5, 0.0 },
		{ 0.0, 0.4,  1.1, 1.2, 0.1 },
		{ 0.0, 1.0,  2.0, 0.4, 1.0 },
		{ 0.0, 1.0,  2.4, 0.05, 1.0 },
		{ 0.0, 0.4,  2.1, 0.2, 0.1 },
		{ 0.0, 0.24, 2.1, 0.2, 0.0 }
	};
	//左副翼位置
	static GLdouble    left_fin[][5] =
	{
		/* { x, y, z, s, t } */
		{ -0.1,  -0.1, 1.1, 0.0, 0.0 },
		{ -0.25, -0.3, 1.2, 1.0, 0.0 },
		{ -0.25, -0.3, 1.5, 1.0, 1.0 },
		{ -0.1,  -0.1, 1.5, 1.0, 0.0 }
	};
	//右副翼位置
	static GLdouble    right_fin[][5] =
	{
		/* { x, y, z, s, t } */
		{ 0.1,  -0.1, 1.1, 0.0, 0.0 },
		{ 0.25, -0.3, 1.2, 1.0, 0.0 },
		{ 0.25, -0.3, 1.5, 1.0, 1.0 },
		{ 0.1,  -0.1, 1.5, 1.0, 0.0 }
	};
	//左后翼位置
	static GLdouble    left_rolleron[][5] = 
	{
		/* { x, y, z, s, t } */
		{ -0.35, 0.0, 1.6, 0.0, 0.0 },
		{ -0.85, 0.0, 2.1, 1.0, 0.5 },
		{ -0.85, 0.0, 2.4, 1.0, 1.0 },
		{ -0.35, 0.0, 2.4, 0.0, 1.0 }
	};
	//右后翼位置
	static GLdouble    right_rolleron[][5] = 
	{
		/* { x, y, z, s, t } */
		{ 0.35, 0.0, 1.6, 0.0, 0.0 },
		{ 0.85, 0.0, 2.1, 1.0, 0.5 },
		{ 0.85, 0.0, 2.4, 1.0, 1.0 },
		{ 0.35, 0.0, 2.4, 0.0, 1.0 }
	};

	F16Body = glGenLists(1);
	glNewList(F16Body, GL_COMPILE);

	tess = gluNewTess();
	gluTessCallback(tess, GLU_TESS_BEGIN, (void(__stdcall *)())glBegin);
	gluTessCallback(tess, GLU_TESS_END, glEnd);
	gluTessCallback(tess, GLU_TESS_VERTEX, (void(CALLBACK *)())glu_vertex);

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);

	//主机身
	//glColor3f(0.3, 0.0, 0.5); //机身颜色
	glColor3f(1.0, 1.0, 1.0);
	glColor3f(1.0,1.0,1.0);
	glPushMatrix();
	glTranslatef(0.0, 0.0, -1.5);
	gluCylinder(quadric, 0.25, 0.25, 3.5, 20, 2);
	glPopMatrix();

	//机头
	//glColor3f(0.3, 0.2, 0.5);//机头颜色
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslatef(0.0, 0.0, -2.5);
	gluCylinder(quadric, 0.0, 0.25, 1.0, 20, 2);
	glPopMatrix();

	//主翼
	//glColor3f(0.3, 0.0, 0.5);
	glColor3f(1.0, 1.0, 1.0);
	gluTessBeginPolygon(tess, NULL);
	for (i = 0; i < 16; i++)
		gluTessVertex(tess, wing[i], wing[i]);

	gluTessEndPolygon(tess);

	//副翼
	//glColor3f(0.3, 0.0, 0.5);
	glColor3f(1.0, 1.0, 1.0);
	glNormal3f(-1.0, 0.0, 0.0);
	gluTessBeginPolygon(tess, NULL);
	for (i = 0; i < 4; i++)
		gluTessVertex(tess, left_fin[i], left_fin[i]);
	gluTessEndPolygon(tess);

	//glColor3f(0.3, 0.0, 0.5);
	glColor3f(1.0, 1.0, 1.0);
	glNormal3f(1.0, 0.0, 0.0);
	gluTessBeginPolygon(tess, NULL);
	for (i = 0; i < 4; i++)
		gluTessVertex(tess, right_fin[i], right_fin[i]);
	gluTessEndPolygon(tess);

	//尾翼
	glColor3f(1.0, 1.0, 1.0);
	gluTessBeginPolygon(tess, NULL);
	for (i = 0; i < 6; i++)
		gluTessVertex(tess, tail[i], tail[i]);
	gluTessEndPolygon(tess);

	//罩
	glPushMatrix();
	glColor4f(0.5, 0.5, 1.0, 0.75);
	glTranslatef(0.0, 0.2, -1.0);
	glScalef(1.0, 1.0, 0.65 / 0.15);
	gluSphere(quadric, 0.15, 6, 12);
	glPopMatrix();

	//引擎
	glPushMatrix();
	//整流罩
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(0.0, 0.0, 2.0);
	gluCylinder(quadric, 0.25, 0.15, 0.5, 20, 2);
	gluDisk(quadric, 0.0, 0.25, 20, 2);

	//排气
	glPushAttrib(GL_LIGHTING_BIT);
	glColorMaterial(GL_FRONT_AND_BACK, GL_EMISSION);
	glColor3f(1.0, 1.0, 1.0);
	gluCylinder(quadric, 0.2, 0.0, 0.3, 10, 2);
	glColor4f(0.25, 0.25, 1.0, 0.75);
	gluCylinder(quadric, 0.2, 0.1, 0.4, 10, 2);
	glPopAttrib();
	glPopMatrix();
	glEndList();

	//左后翼
	F16Rolleron[0] = glGenLists(1);
	glNewList(F16Rolleron[0], GL_COMPILE);
	glColor3f(1.0, 1.0, 1.0);  
	glNormal3f(0.0, 1.0, 0.0);
	gluTessBeginPolygon(tess, NULL);
	for (i = 0; i < 4; i++)
		gluTessVertex(tess, left_rolleron[i], left_rolleron[i]);
	gluTessEndPolygon(tess);
	glEndList();

	//右后翼
	F16Rolleron[1] = glGenLists(1);
	glNewList(F16Rolleron[1], GL_COMPILE);
	glColor3f(1.0, 1.0, 1.0);
	glNormal3f(0.0, 1.0, 0.0);
	gluTessBeginPolygon(tess, NULL);
	for (i = 0; i < 4; i++)
		gluTessVertex(tess, right_rolleron[i], right_rolleron[i]);
	gluTessEndPolygon(tess);
	glEndList();

	gluDeleteQuadric(quadric);
	gluDeleteTess(tess);
}

void CglPlane::Draw(void){
	glPushMatrix();
	glMultMatrixd(m_matrix);
	
	//画3D文字
	glPushMatrix();
	glTranslatef(-1.7, 1, 0);
	m_pFont->Font3D(str.GetBuffer(0));
	glPopMatrix();

    //画主机身
	glCallList(F16Body);

	//左后翼
	glPushMatrix();
	glTranslatef(0.0, 0.0, 2.0);
	glTranslatef(0.0, 0.0, -2.0);
	glCallList(F16Rolleron[0]);
	glPopMatrix();

	//右后翼
	glPushMatrix();
	glTranslatef(0.0, 0.0, 2.0);
	glTranslatef(0.0, 0.0, -2.0);
	glCallList(F16Rolleron[1]);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, 4.2);
	glRotatef(90, 1, 0, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//尾焰
	m_particle.UpdateTail();
	m_particle.DrawTail();
	
	glPopMatrix();
	glPopMatrix();
}

//dir=0表示左右移动，dir=1表示上下移动，dir=2表示前后移动
float CglPlane::Move(int dir, double plus_or_minus, float usetime) {
	float movedis = usetime * key_step / 30;
	CglMatrix m_tr;
	if (dir == 0)
		m_tr.SetTrans(CglVector3(-movedis * scale_step[0] * plus_or_minus, 0, 0));
	else if (dir == 1)
		m_tr.SetTrans(CglVector3(0, -movedis * scale_step[0] * plus_or_minus, 0));
	else
		m_tr.SetTrans(CglVector3(0, 0, -movedis * scale_step[0] * plus_or_minus));
	m_matrix = m_matrix * m_tr;
	m_pos = &m_matrix[12];
	return movedis * scale_step[0];
}

//dir=0表示左右转，dir=1表示上下转，dir=2表示滚动转
void CglPlane::Rotate(int dir, double plus_or_minus, float usetime) {
	float rotdis = usetime * key_step / 30;
	CglMatrix m_ro;
	if (dir == 0)
		m_ro.SetRotate(rotdis*scale_step[1] * plus_or_minus, CglVector3(0, 1, 0));//h
	else if (dir == 1)
		m_ro.SetRotate(rotdis*scale_step[1] * plus_or_minus, CglVector3(1, 0, 0));//p
	else
		m_ro.SetRotate(rotdis*scale_step[1] * plus_or_minus, CglVector3(0, 0, 1));//r

	m_matrix[12] = 0;
	m_matrix[13] = 0;
	m_matrix[14] = 0;
	m_matrix = m_matrix*m_ro;
	m_matrix[12] += m_pos.x;
	m_matrix[13] += m_pos.y;
	m_matrix[14] += m_pos.z;

	m_dir = m_matrix.vecMul(CglVector3(0, 0, -1));
	m_updir = m_matrix.vecMul(CglVector3(0, 1, 0));
}

void CglPlane::SetSpeed(float mspeed, float rspeed) {
	scale_step[0] = mspeed;
	scale_step[1] = rspeed;
}

void CglPlane::SetDir(CglVector3 dir) {
	dir.Normalize();
	CglVector3 xdir = dir.crossMul(m_updir);	//确定子坐标系x方向
	xdir.Normalize();
	CglVector3 ydir = xdir.crossMul(dir);		//确定子坐标系y方向
	ydir.Normalize();
	m_matrix.SetCol(0, xdir);
	m_matrix.SetCol(1, ydir);
	m_matrix.SetCol(2, -dir);
	m_updir = ydir;
	m_dir = dir;
}

float CglPlane::GetMSpeed() {
	return scale_step[0] * key_step / 30;
}

float CglPlane::GetRSpeed() {
	return scale_step[1] * key_step / 30;
}

int CglPlane::GetUseTime() {
	int curtime = GetTickCount();
	int usetime = 0;
	if (lasttime > 0) {
		usetime = curtime - lasttime;
	}
	lasttime = curtime;
	return usetime;
}
