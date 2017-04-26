#include "stdafx.h"
#include "CglMatrix.h"
#include "CglVector3.h"
#include "CglEuler.h"
#include "CglQuaternion.h"
#include <math.h>

//构造函数(默认构造单位矩阵)
CglMatrix::CglMatrix() {
	(*this).Identity();
}

CglMatrix::~CglMatrix() {

}

void CglMatrix::Zero() {
	for (int i = 0; i < 16; i++) {
		(*this)[i] = 0;
	}
}

void CglMatrix::Identity() {
	this->Zero();
	this->m00 = this->m11 = this->m22 = this->m33 = 1;
}

void CglMatrix::Set(double *p) {
	for (int i = 0; i < 16; i++) {
		(*this)[i] = p[i];
	}
}

void CglMatrix::SetRow(int i, CglVector3 p) {
	switch (i) {
	case 0: m00 = p.x; m01 = p.y; m02 = p.z; m03 = 0; break;
	case 1: m10 = p.x; m11 = p.y; m12 = p.z; m13 = 0; break;
	case 2: m20 = p.x; m21 = p.y; m22 = p.z; m23 = 0; break;
	case 3: m30 = p.x; m31 = p.y; m32 = p.z; m33 = 1; break;
	}
}

void CglMatrix::SetCol(int i, CglVector3 p) {
	switch (i) {
	case 0: m00 = p.x; m10 = p.y; m20 = p.z; m30 = 0; break;
	case 1: m01 = p.x; m11 = p.y; m21 = p.z; m31 = 0; break;
	case 2: m02 = p.x; m12 = p.y; m22 = p.z; m32 = 0; break;
	case 3: m03 = p.x; m13 = p.y; m23 = p.z; m33 = 1; break;
	}
}

CglMatrix& CglMatrix:: operator=(CglMatrix &p) {
	for (int i = 0; i < 16; i++) {
		(*this)[i] = p[i];
	}
	return (*this);
}

CglMatrix CglMatrix::operator*(double d) {
	for (int i = 0; i < 16; i++) {
		(*this)[i] *= d;
	}
	return (*this);
}

CglMatrix CglMatrix::operator*(CglMatrix &p) {
	CglMatrix result;
	result.m00 = result.m11 = result.m22 = result.m33 = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				result[i * 4 + j] += p[i * 4 + k] * (*this)[k * 4 + j];
			}
		}
	}
	return result;
}

CglVector3 CglMatrix::vecMul(const CglVector3 &p) {
	CglVector3 cv;
	for (int i = 0; i < 3; i++) {
		cv[i] = (*this)[i] * p.x + (*this)[4 + i] * p.y + (*this)[8 + i] * p.z;
	}
	return cv;
}

CglVector3 CglMatrix::posMul(const CglVector3 &p) {
	CglVector3 cv;
	for (int i = 0; i < 3; i++) {
		cv[i] = (*this)[i] * p.x + (*this)[4 + i] * p.y + (*this)[8 + i] * p.z + (*this)[12 + i];
	}
	return cv;
}

void CglMatrix::Transpose() {
	double t;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (j < i) {
				t = (*this)[i * 4 + j];
				(*this)[i * 4 + j] = (*this)[j * 4 + i];
				(*this)[j * 4 + i] = t;
			}
		}
	}
}

CglMatrix& CglMatrix::SetRotate(double seta, CglVector3 axis) {
	seta = AngToRad(seta);
	axis.Normalize();

	double x = axis.x;
	double y = axis.y;
	double z = axis.z;

	this->m00 = x*x*(1 - cosf(seta)) + cosf(seta);
	this->m01 = x*y*(1 - cosf(seta)) - z*sinf(seta);
	this->m02 = x*z*(1 - cosf(seta)) + y*sinf(seta);
	this->m03 = 0;

	this->m10 = x*y*(1 - cosf(seta)) + z*sinf(seta);
	this->m11 = y*y*(1 - cosf(seta)) + cosf(seta);
	this->m12 = y*z*(1 - cosf(seta)) - x*sinf(seta);
	this->m13 = 0;

	this->m20 = x*z*(1 - cosf(seta)) - y*sinf(seta);
	this->m21 = y*z*(1 - cosf(seta)) + x*sinf(seta);
	this->m22 = z*z*(1 - cosf(seta)) + cosf(seta);
	this->m23 = 0;

	this->m30 = 0;
	this->m31 = 0;
	this->m32 = 0;
	this->m33 = 1;

	return (*this);
}

CglMatrix& CglMatrix::SetTrans(CglVector3 trans) {
	(*this)[0] = (*this)[5] = (*this)[10] = (*this)[15] = 1;
	(*this)[12] = trans.x;
	(*this)[13] = trans.y;
	(*this)[14] = trans.z;
	return (*this);
}

CglMatrix& CglMatrix::SetScale(CglVector3 p) {
	(*this)[0] = p.x;
	(*this)[5] = p.y;
	(*this)[10] = p.z;
	(*this)[15] = 1;
	return (*this);
}

double CglMatrix::Inverse() {
	double det, det00, det01, det02, det03;
	det00 = m11*(m22*m33 - m32*m23) - m12*(m21*m33 - m31*m23) + m13*(m21*m32 - m31*m22);
	det01 = m10*(m22*m33 - m32*m23) - m12*(m20*m33 - m30*m23) + m13*(m20*m32 - m30*m22);
	det02 = m10*(m21*m33 - m31*m23) - m11*(m20*m33 - m30*m23) + m13*(m20*m31 - m30*m21);
	det03 = m10*(m21*m32 - m31*m22) - m11*(m20*m32 - m30*m22) + m12*(m20*m31 - m30*m21);

	det = m00*det00 - m01*det01 + m02*det02 - m03*det03;
	if (det == 0) {
		return 0;
	}
	double invdet = 1.f / det;
	det01 *= -1; det03 *= -1;
	double det10, det11, det12, det13;
	det10 = m01*(m22*m33 - m32*m23) - m02*(m21*m33 - m31*m23) + m03*(m21*m32 - m31*m22);
	det11 = m00*(m22*m33 - m32*m23) - m02*(m20*m33 - m30*m23) + m03*(m20*m32 - m30*m22);
	det12 = m00*(m21*m33 - m31*m23) - m01*(m20*m33 - m30*m23) + m03*(m20*m31 - m30*m21);
	det13 = m00*(m21*m32 - m31*m22) - m01*(m20*m32 - m30*m22) + m02*(m20*m31 - m30*m21);
	det10 *= -1; det12 *= -1;

	double det20, det21, det22, det23;
	det20 = m01*(m12*m33 - m32*m13) - m02*(m11*m33 - m31*m13) + m03*(m11*m32 - m31*m12);
	det21 = m00*(m12*m33 - m32*m13) - m02*(m10*m33 - m30*m13) + m03*(m10*m32 - m30*m12);
	det22 = m00*(m11*m33 - m31*m13) - m01*(m10*m33 - m30*m13) + m03*(m10*m31 - m30*m11);
	det23 = m00*(m11*m32 - m31*m12) - m01*(m10*m32 - m30*m12) + m02*(m10*m31 - m30*m11);
	det21 *= -1; det23 *= -1;

	double det30, det31, det32, det33;
	det30 = m01*(m12*m23 - m22*m13) - m02*(m11*m23 - m21*m13) + m03*(m11*m22 - m21*m12);
	det31 = m00*(m12*m23 - m22*m13) - m02*(m10*m23 - m20*m13) + m03*(m10*m22 - m20*m12);
	det32 = m00*(m11*m23 - m21*m13) - m01*(m10*m23 - m20*m13) + m03*(m10*m21 - m20*m11);
	det33 = m00*(m11*m22 - m21*m12) - m01*(m10*m22 - m20*m12) + m02*(m10*m21 - m20*m11); 
	det30 *= -1; det32 *= -1;

	m00 = det00*invdet; m10 = det01*invdet; m20 = det02*invdet; m30 = det03*invdet;
	m01 = det10*invdet; m11 = det11*invdet; m21 = det12*invdet; m31 = det13*invdet;
	m02 = det20*invdet; m12 = det21*invdet; m22 = det22*invdet; m32 = det23*invdet;
	m03 = det30*invdet; m13 = det31*invdet; m23 = det32*invdet; m33 = det33*invdet;
	return det;
}

CglMatrix CglMatrix::GetInverse() {
	CglMatrix cm = (*this);
	cm.Inverse();
	return cm;
}

CglEuler CglMatrix::ToEuler() {
	if (this->m12 == 1 || this->m12 == -1) {
		return CglEuler(RadToAng(atan2f(this->m20, this->m00)), RadToAng(this->m12*1.57), 0);
	} else {
		return CglEuler(RadToAng(atan2f(this->m02, this->m22)), RadToAng(asinf(-this->m12)), RadToAng(atan2f(this->m10, this->m11)));
	}
}

CglQuaternion CglMatrix::ToQuaternion() {
	double s;
	if (this->m00 > this->m11 && this->m00 > this->m22) {
		s = 2 * sqrt(1.0 + this->m00 - this->m11 - this->m22);
		return CglQuaternion(0.25*s, (this->m10 + this->m01) / s, (this->m02 + this->m20) / s, (this->m21 - this->m12) / s);
	} else if (this->m11 > this->m22) {
		s = 2 * sqrt(1.0 + this->m11 - this->m00 - this->m22);
		return CglQuaternion((this->m10 + this->m01) / s, 0.25*s, (this->m21 + this->m12) / s, (this->m02 - this->m20) / s);
	} else {
		s = 2 * sqrt(1.0 + this->m22 - this->m00 - this->m11);
		return CglQuaternion((this->m02 + this->m20) / s, (this->m21 + this->m12) / s, 0.25*s, (this->m10 - this->m01) / s);
	}
}

CglMatrix::operator double *() {
	return &m00;
}
