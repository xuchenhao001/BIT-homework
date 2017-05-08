#include "stdafx.h"
#include "CglVector3.h"
#include "CglEuler.h"
#include <math.h>

CglVector3::CglVector3() {
	x = 0; y = 0; z = 0;
}

CglVector3::CglVector3(double fx, double fy, double fz) {
	x = fx; y = fy; z = fz;
}

CglVector3::CglVector3(double *pos) {
	x = pos[0];
	y = pos[1];
	z = pos[2];
}

CglVector3::~CglVector3() {

}

void CglVector3::Set(double fx, double fy, double fz) {
	x = fx; y = fy; z = fz;
}

void CglVector3::Set(double *pos) {
	x = pos[0];
	y = pos[1];
	z = pos[2];
}

void CglVector3::Get(double *pos) {
	pos[0] = x;
	pos[1] = y;
	pos[2] = z;
}

CglVector3::operator double*() {
	return &x;
}

CglVector3::operator const double*() const {
	return &x;
}

CglVector3& CglVector3::operator=(const CglVector3& p) {
	x = p.x; y = p.y; z = p.z;
	return *this;
}

CglVector3& CglVector3::operator=(const CglEuler& p) {
	x = p.h; y = p.p; z = p.b;
	return *this;
}

CglVector3 CglVector3::operator+(const CglVector3& p) const {
	CglVector3 vec;
	vec.x = x + p.x;
	vec.y = y + p.y;
	vec.z = z + p.z;
	return vec;
}

bool CglVector3::operator==(const CglVector3& p) {
	if (fabs(x - p.x)<1e-5)
		if (fabs(y - p.x)<1e-5)
			if (fabs(x - p.x)<1e-5)
				return true;
	return false;
}

CglVector3& CglVector3::operator+=(const CglVector3& p) {
	x += p.x;
	y += p.y;
	z += p.z;
	return *this;
}

CglVector3& CglVector3::operator-=(const CglVector3& p) {
	x -= p.x;
	y -= p.y;
	z -= p.z;
	return *this;
}

CglVector3 CglVector3::operator-(const CglVector3& p) const {
	CglVector3 vec;
	vec.x = x - p.x;
	vec.y = y - p.y;
	vec.z = z - p.z;
	return vec;
}

CglVector3 CglVector3::operator-() const {
	CglVector3 vec;
	vec.x = -x;
	vec.y = -y;
	vec.z = -z;
	return vec;
}

CglVector3 CglVector3::operator*(double data) const {
	CglVector3 vec;
	vec.x = x*data;
	vec.y = y*data;
	vec.z = z*data;
	return vec;
}

CglVector3 operator*(double data, const CglVector3& p) {
	CglVector3 vec;
	vec.x = p.x*data;
	vec.y = p.y*data;
	vec.z = p.z*data;
	return vec;
}

CglVector3 CglVector3::operator*(double *mat) const {
	double v[4];
	int i;
	for (i = 0; i<4; i++)
	{
		v[i] = mat[i * 4] * x + mat[1 + i * 4] * y + mat[2 + i * 4] * z + mat[3 + i * 4];
	}
	CglVector3 vec;
	for (i = 0; i<3; i++)
		vec[i] = v[0] / v[3];
	return vec;
}

CglVector3 operator*(double *mat, const CglVector3& p) {
	double v[4];
	for (int i = 0; i<4; i++)
	{
		v[i] = mat[i] * p.x + mat[4 + i] * p.y + mat[8 + i] * p.z + mat[12 + i];
	}
	CglVector3 vec;
	vec.x = v[0] / v[3];
	vec.y = v[1] / v[3];
	vec.z = v[2] / v[3];
	return vec;
}

CglVector3 CglVector3::operator/(double data) const {
	CglVector3 vec;
	vec.x = x / data;
	vec.y = y / data;
	vec.z = z / data;
	return vec;
}

double CglVector3::dotMul(const CglVector3&p) {
	return x*p.x + y*p.y + z*p.z;
}

CglVector3 CglVector3::crossMul(const CglVector3&p) const {
	CglVector3 vec;
	vec.x = y*p.z - z*p.y;
	vec.y = z*p.x - x*p.z;
	vec.z = x*p.y - y*p.x;
	return vec;
}

double CglVector3::len() {
	return sqrt(x*x + y*y + z*z);
}

void CglVector3::Normalize() {
	double l = len();
	x /= l;
	y /= l;
	z /= l;
}

//向量投影操作
CglVector3 CglVector3::project(CglVector3 &p) {
	double length = p.len();
	double proportion = this->dotMul(p) / length / length;
	return CglVector3(p.x*proportion, p.y*proportion, p.z*proportion);
}

void CglVector3::Add(int addr, double val) {
	if (addr == 0)
		x += val;
	else if (addr == 1)
		y += val;
	else if (addr == 2)
		z += val;
}

void CglVector3::Rotate(double seta, int axis) {//绕某个轴旋转seta角度,弧度角
	switch (axis)
	{
	case 0://X 轴
	{
		double y1 = y*cos(seta) - z*sin(seta);
		z = y*sin(seta) + z*cos(seta);
		y = y1;
	}
	break;
	case 1://Y 轴
	{
		double x1 = x*cos(seta) + z*sin(seta);
		z = -x*sin(seta) + z*cos(seta);
		x = x1;
	}
	break;
	case 2://Y 轴
	{
		double x1 = x*cos(seta) - y*sin(seta);
		y = x*sin(seta) + y*cos(seta);
		x = x1;
	}
	break;
	}
}

void CglVector3::Rotate(double seta, double x, double y, double z) {//绕某个方向转seta角度,弧度角
	double dis1 = x*x + z*z;
	double dis2 = dis1 + y*y;
	dis1 = sqrt(dis1);
	dis2 = sqrt(dis2);
	//先绕x轴转，使轴转动到XOZ平面上,角度为xyz与XOZ夹角
	double setax = acos(dis1 / dis2);
	if (y<0) setax = -setax;
	Rotate(setax, 0);
	//再绕y轴转，使轴转动到Z轴重合位置,角度为xyz投影与z轴夹角
	double setay = acos(z / dis1);
	if (x>0) setay = -setay;
	Rotate(setay, 1);
	//开始绕z轴转
	Rotate(seta, 2);
	//////////////////////////////////////////////////////////////////////////恢复
	//绕y转回来。
	Rotate(-setay, 1);
	//绕x转回来。
	Rotate(-setax, 0);
}

//向量转换为欧拉角
CglEuler CglVector3::ToEuler() {
	CglVector3 a(x, y, z), b(x, 0, z), z(0, 0, -1);
	double h = acos(b.dotMul(z) / (b.len()*z.len()));
	if (x > 0) {
		h = RadToAng(-fabs(h));
	} else {
		h = RadToAng(fabs(h));
	}
	double p = acos(a.dotMul(b) / (a.len()*b.len()));
	if (y > 0) {
		p = RadToAng(fabs(p));
	} else {
		p = RadToAng(-fabs(p));
	}
	return CglEuler(h, p, 0);
}

