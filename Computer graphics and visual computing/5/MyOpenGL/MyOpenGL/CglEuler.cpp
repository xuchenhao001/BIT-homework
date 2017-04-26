#include "stdafx.h"
#include "CglEuler.h"
#include "CglVector3.h"
#include "CglMatrix.h"
#include "CglQuaternion.h"
#include <math.h>

CglEuler::CglEuler() {
	this->h = this->p = this->b = 0;
}

CglEuler::CglEuler(double h, double p, double b) {
	this->h = h;
	this->p = p;
	this->b = b;
}

CglEuler::~CglEuler() {

}

void CglEuler::Set(double h, double p, double b) {
	this->h = h;
	this->p = p;
	this->b = b;
}

CglVector3 CglEuler::ToVector3(CglVector3 *updir) {
	CglMatrix m = this->ToMatrix();
	CglVector3 dir = m.vecMul(CglVector3(0, 0, -1));
	if (updir != 0) {
		*updir = m.vecMul(CglVector3(0, 1, 0));
	}
	return dir;
}

//欧拉角转换为矩阵
CglMatrix CglEuler::ToMatrix() {
	double hRad = AngToRad(h);
	double pRad = AngToRad(p);
	double bRad = AngToRad(b);
	CglMatrix m;
	m.m00 = cosf(hRad)*cosf(bRad) + sinf(hRad)*sinf(pRad)*sinf(bRad);
	m.m01 = -cosf(hRad)*sinf(bRad) + sinf(hRad)*sinf(pRad)*cosf(bRad);
	m.m02 = sinf(hRad)*cosf(pRad);
	m.m10 = sinf(bRad)*cosf(pRad);
	m.m11 = cosf(bRad)*cosf(pRad);
	m.m12 = -sinf(pRad);
	m.m20 = -sinf(hRad)*cosf(bRad) + cosf(hRad)*sinf(pRad)*sinf(bRad);
	m.m21 = sinf(bRad)*sinf(hRad) + cosf(hRad)*sinf(pRad)*cosf(bRad);
	m.m22 = cosf(hRad)*cosf(pRad);
	m.m33 = 1;
	return m;
}

CglQuaternion CglEuler::ToQuaternion() {
	CglQuaternion quater;
	double hRad = AngToRad(h);
	double pRad = AngToRad(p);
	double bRad = AngToRad(b);
	quater.w = cos(hRad / 2)*cos(pRad / 2)*cos(bRad / 2) + sin(hRad / 2)*sin(pRad / 2)*sin(bRad / 2);
	quater.x = cos(hRad / 2)*sin(pRad / 2)*cos(bRad / 2) + sin(hRad / 2)*cos(pRad / 2)*sin(bRad / 2);
	quater.y = sin(hRad / 2)*cos(pRad / 2)*cos(bRad / 2) - cos(hRad / 2)*sin(pRad / 2)*sin(bRad / 2);
	quater.z = cos(hRad / 2)*cos(pRad / 2)*sin(bRad / 2) - sin(hRad / 2)*sin(pRad / 2)*cos(bRad / 2);
	return quater;
}

//欧拉角规范化
void CglEuler::Normal() {
	(*this) = (*this).ToMatrix().ToEuler();
}

CglEuler::operator double*() {
	return &h;
}
