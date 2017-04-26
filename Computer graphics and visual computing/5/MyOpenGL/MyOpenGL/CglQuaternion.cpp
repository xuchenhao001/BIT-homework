#include "stdafx.h"
#include "CglQuaternion.h"
#include "CglMatrix.h"
#include "CglEuler.h"
#include "CglVector3.h"
#include <math.h>

CglQuaternion::CglQuaternion() {
	this->w = this->x = this->y = this->z = 0;
}

CglQuaternion::CglQuaternion(double x, double y, double z, double w) {
	this->w = w;
	this->x = x;
	this->y = y;
	this->z = z;
}

CglQuaternion::~CglQuaternion() {

}

//��ֵ����
void CglQuaternion::Set(double x, double y, double z, double w) {
	this->w = w;
	this->x = x;
	this->y = y;
	this->z = z;
}

//����ת��
CglQuaternion::operator double*() {
	return &x;
}

//������ת�����ת�Ƕ�������Ԫ��
void CglQuaternion::SetAngle(double angle, CglVector3 axis) {
	angle *= 0.5f;
	axis.Normalize();
	this->x = (axis.x * sinf(angle));
	this->y = (axis.y * sinf(angle));
	this->z = (axis.z * sinf(angle));
	this->w = cos(angle);
}

//���ظ�ֵ
CglQuaternion& CglQuaternion::operator=(const CglQuaternion &p) {
	this->x = p.x;
	this->y = p.y;
	this->z = p.z;
	this->w = p.w;
	return (*this);
}

//���ء�+��
CglQuaternion CglQuaternion::operator+(const CglQuaternion &p) {
	if (w*p.w >= 0) {
		return CglQuaternion(x + p.x, y + p.y, z + p.z, w + p.w);
	} else {
		return CglQuaternion(x - p.x, y - p.y, z - p.z, w - p.w);
	}
}

//��������
CglQuaternion CglQuaternion::operator*(double data) {
	return CglQuaternion(x*data, y*data, z*data, w*data);
}

//��Ԫ���˷�(�¶���˷�)
CglQuaternion CglQuaternion::operator*(const CglQuaternion &p) {
	CglQuaternion quater;
	quater.w = w*p.w - x*p.x - y*p.y - z*p.z;
	quater.x = w*p.x + x*p.w + z*p.y - y*p.z;
	quater.y = w*p.y + y*p.w + x*p.z - z*p.x;
	quater.z = w*p.z + z*p.w + y*p.x - x*p.y;
	return quater;
}

//���
double CglQuaternion::dotMul(const CglQuaternion&p) {
	return w*p.w + x*p.x + y*p.y + z*p.z;
}

//��ģ
double CglQuaternion::len() {
	return sqrt(w*w + x*x + y*y + z*z);
}

//����
CglQuaternion CglQuaternion::conjugate() {
	return CglQuaternion(-x, -y, -z, w);
}

//��Ԫ����λ��
bool CglQuaternion::Normalize() {
	double n = this->len();
	*this = (*this) * (1 / n);
	return TRUE;
}

//������Ԫ��,��ı�����
CglQuaternion& CglQuaternion::Inverse() {
	(*this) = this->GetInverse();
	return (*this);
}

//������Ԫ��,���ı����������µ���Ԫ��
CglQuaternion CglQuaternion::GetInverse() {
	double n = this->len();
	if (n == 0) {
		printf("��Ԫ��ģΪ0�޷����棡");
		return (*this);
	}
	return this->conjugate()*(1 / (n*n));
}

//��� ��ǰΪa,��c=a-b
CglQuaternion CglQuaternion::Div(const CglQuaternion &b) {
	return this->GetInverse()*b;
}

//����
CglQuaternion CglQuaternion::exp(const double t) {
	double angle;
	CglVector3 vect;
	this->GetAngle(angle, vect);
	angle = AngToRad(angle);
	vect = vect*(sinf(t*angle / 2));
	return CglQuaternion(vect.x, vect.y, vect.z, cosf(t*angle / 2));
}

//����ת��ͽǶ�
void CglQuaternion::GetAngle(double &angle, CglVector3 &axis) {
	this->Normalize();
	angle = acosf(w);
	axis = CglVector3(this->x / sinf(angle), this->y / sinf(angle), this->z / sinf(angle));
	angle = RadToAng(angle*2.0);
}

//��ֵ���ӵ�ǰ��Ԫ����ֵ��Vend��Ԫ��,t�ǲ���[0,1]
CglQuaternion CglQuaternion::Slerp(const CglQuaternion &Vend, double t) {
	this->Normalize();
	CglQuaternion cqstart = (*this), cqEnd = Vend;
	cqEnd.Normalize();
	double omiga;
	CglVector3 axis;
	(*this).Div(cqEnd).GetAngle(omiga, axis);
	omiga = AngToRad(omiga);
	if (omiga == 0) {
		return Vend;
	}
	double k0 = sinf((1 - t)*omiga) / sinf(omiga);
	double k1 = sinf(t*omiga) / sinf(omiga);
	CglQuaternion result = (*this)*k0 + cqEnd*k1;
	result.Normalize();
	return result;
}

//��ֵ��һ�β�ֵ��n�����ݡ���ֵ��������������t�У�������ص�����Result��
void CglQuaternion::Slerp(const CglQuaternion &Vend, int n, double *t, CglQuaternion *Result) {
	for (int i = 0; i < n; i++) {
		*(Result + i) = Slerp(Vend, *(t + i));
	}
}

//��Ԫ��ת��Ϊŷ����
CglEuler CglQuaternion::ToEuler() {
	CglEuler euler;
	euler.p = RadToAng(asin(2 * w*x - 2 * y*z));
	if (cosf(euler.p) < 0) {
		euler.h = RadToAng(atan2(2 * w*y - 2 * z*x, 1 - 2 * y*y - 2 * z*z));
		euler.b = 0;
	} else {
		euler.h = RadToAng(atan2(2 * z*x + 2 * w*y, 1 - 2 * x*x - 2 * y*y));
		euler.b = RadToAng(atan2(2 * w*z + 2 * y*x, 1 - 2 * x*x - 2 * z*z));
	}
	return euler;
}

//��Ԫ��ת��Ϊ����
CglMatrix CglQuaternion::ToMatrix() {
	CglMatrix matrix;
	matrix.m00 = 1 - 2 * y*y - 2 * z*z;
	matrix.m01 = 2 * x*y - 2 * w*z;
	matrix.m02 = 2 * x*z + 2 * w*y;
	matrix.m10 = 2 * x*y + 2 * w*z;
	matrix.m11 = 1 - 2 * x*x - 2 * z*z;
	matrix.m12 = 2 * y*z - 2 * w*x;
	matrix.m20 = 2 * x*z - 2 * w*y;
	matrix.m21 = 2 * y*z + 2 * w*x;
	matrix.m22 = 1 - 2 * x*x - 2 * y*y;
	matrix.m33 = 1;
	return matrix;
}
