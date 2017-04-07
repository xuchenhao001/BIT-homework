#include "stdafx.h"
#include <iostream>
#include <GL/glut.h>
#include <string>
#include <sstream>
#include <iomanip>
#include "MyMatrixVector.h"

using namespace std;

//==============
//�����ඨ�岿��
//==============
const float deviation = 0.00001;//����趨

//���캯��
CVector3::CVector3() {
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

//�������Ĺ��캯��
CVector3::CVector3(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

void CVector3::Set(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

//������˲���
float CVector3::dotMul(CVector3 &n) {
	return this->x*n.x + this->y*n.y + this->z*n.z;
}

//������˲���
CVector3 CVector3::crossMul(CVector3 &n) {
	return CVector3(this->y*n.z - n.y*this->z, this->z*n.x - n.z*this->x, this->x*n.y - n.x*this->y);
}

//������׼������
void CVector3::Normalize() {
	float length = this->len();
	if (length < deviation) {
		printf("�������޷���׼����");
		return;
	}
	this->x /= length;
	this->y /= length;
	this->z /= length;
}

//������ģ����
float CVector3::len() {
	return sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
}

	//����ͶӰ����
CVector3 CVector3::project(CVector3 &n) {
	float length = n.len();
	if (length < deviation) {
		return CVector3();
	}
	float proportion = this->dotMul(n) / length / length;
	return CVector3(n.x*proportion, n.y*proportion, n.z*proportion);
}

//���ظ�ֵ����
CVector3 CVector3::operator=(const CVector3 &p) {
	x = p.x;
	y = p.y;
	z = p.z;
	return *this;
}

//����Java��toString,������xyz��ֵ����string
string CVector3::toString() {
	ostringstream buffer;
	buffer << setiosflags(ios::fixed);
	buffer << setprecision(2) << this->x << "," << this->y << "," << this->z;
	return string(buffer.str());
}

//����ת��
CVector3::operator float*() {
	return &x;
}

//��������'+'
CVector3 operator+(const CVector3 &a, const CVector3 &b) {
	return CVector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

//��������'-'
CVector3 operator-(const CVector3 &a, const CVector3 &b) {
	return CVector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

//�����������˲���'*'
CVector3 operator*(const CVector3 &a, float b) {
	return CVector3(a.x*b, a.y*b, a.z*b);
}

//����'=='�Ƚϲ���
bool operator==(const CVector3 &a, const CVector3 &b) {
	if (a.x == b.x&&a.y == b.y&&a.z == b.z) {
		return true;
	}
	return false;
}

//����'!='�Ƚϲ���
bool operator!=(const CVector3 &a, const CVector3 &b) {
	if (a.x == b.x&&a.y == b.y&&a.z == b.z) {
		return false;
	}
	return true;
}

//==============
//�����ඨ�岿��
//==============

//���캯��
CMatrix::CMatrix() {
	for (int i = 0; i < 16; i++) {
		(*this)[i] = 0;
	}
}

//��������
void CMatrix::Zero() {
	for (int i = 0; i < 16; i++) {
		(*this)[i] = 0;
	}
}

//����Ϊ��λ����
void CMatrix::Identity() {
	this->Zero();
	this->m00 = this->m11 = this->m22 = this->m33 = 1;
}

//������16��Ԫ�ظ�ֵ
void CMatrix::Set(float *p) {
	for (int i = 0; i < 16; i++) {
		(*this)[i] = p[i];
	}
}

//���õ�i��Ԫ��
void CMatrix::SetRow(int i, CVector3 p) {
	switch (i){
	case 0: m00 = p.x; m01 = p.y; m02 = p.z; m03 = 0; break;
	case 1: m10 = p.x; m11 = p.y; m12 = p.z; m13 = 0; break;
	case 2: m20 = p.x; m21 = p.y; m22 = p.z; m23 = 0; break;
	case 3: m30 = p.x; m31 = p.y; m32 = p.z; m33 = 1; break;
	}
}

//���õ�i��Ԫ��
void CMatrix::SetCol(int i, CVector3 p) {
	switch (i) {
	case 0: m00 = p.x; m10 = p.y; m20 = p.z; m30 = 0; break;
	case 1: m01 = p.x; m11 = p.y; m21 = p.z; m31 = 0; break;
	case 2: m02 = p.x; m12 = p.y; m22 = p.z; m32 = 0; break;
	case 3: m03 = p.x; m13 = p.y; m23 = p.z; m33 = 1; break;
	}
}

//����ֵ
CMatrix& CMatrix:: operator=(CMatrix &p) {
	for (int i = 0; i < 16; i++) {
		(*this)[i] = p[i];
	}
	return (*this);
}

//��������
CMatrix CMatrix::operator*(float d) {
	for (int i = 0; i < 16; i++) {
		(*this)[i] *= d;
	}
	return (*this);
}

//�������
CMatrix CMatrix::operator*(CMatrix &p) {
	CMatrix result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				result[i * 4 + j] += p[i * 4 + k] * (*this)[k * 4 + j];
			}
		}
	}
	return result;
}

//�������������
CVector3 CMatrix::vecMul(const CVector3 &p) {
	CVector3 cv;
	for (int i = 0; i < 3; i++) {
		cv[i] = (*this)[i] * p.x + (*this)[4 + i] * p.y + (*this)[8 + i] * p.z;
	}
	return cv;
}

//����������
CVector3 CMatrix::posMul(const CVector3 &p) {
	CVector3 cv;
	for (int i = 0; i < 3; i++) {
		cv[i] = (*this)[i] * p.x + (*this)[4 + i] * p.y + (*this)[8 + i] * p.z + (*this)[12+i];
	}
	return cv;
}

//����ת��
void CMatrix::Transpose() {
	float t;
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

//����Ϊ��ת����
CMatrix& CMatrix::SetRotate(float seta, CVector3 axis) {
	float x = axis.x;
	float y = axis.y;
	float z = axis.z;

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

//����Ϊƽ�ƾ���
CMatrix& CMatrix::SetTrans(CVector3 trans) {
	(*this)[12] = trans.x;
	(*this)[13] = trans.y;
	(*this)[14] = trans.z;
	return (*this);
}

//����Ϊ���ž���
CMatrix& CMatrix::SetScale(CVector3 p) {
	(*this)[0] = p.x;
	(*this)[5] = p.y;
	(*this)[10] = p.z;
	(*this)[15] = 1;
	return (*this);
}

//�������棬�ɹ���������ʽ��ֵ�����򷵻�0
float CMatrix::Inverse() {
	//����|M|,���������det�У�det00->det03�����ÿ��Ԫ�ض�Ӧ�Ĵ�������ʽ
	//������̾��ǰ��Ľ׵�����ʽչ�������׵�����ʽ
	float det, det00, det01, det02, det03;
	det00 = m11*(m22*m33 - m32*m23) - m12*(m21*m33 - m31*m23) + m13*(m21*m32 - m31*m22);
	det01 = m10*(m22*m33 - m32*m23) - m12*(m20*m33 - m30*m23) + m13*(m20*m32 - m30*m22);
	det02 = m10*(m21*m33 - m31*m23) - m11*(m20*m33 - m30*m23) + m13*(m20*m31 - m30*m21);
	det03 = m10*(m21*m32 - m31*m22) - m11*(m20*m32 - m30*m22) + m12*(m20*m31 - m30*m21);

	det = m00*det00 - m01*det01 + m02*det02 - m03*det03;
	if (det == 0) {
		return 0;
	}
	float invdet = 1.f / det;
	//������ʽ��ɴ�������ʽ  
	det01 *= -1; det03 *= -1;
	//����det10->det33 ��ӦԪ�صĴ�������ʽ  
	float det10, det11, det12, det13;
	det10 = m01*(m22*m33 - m32*m23) - m02*(m21*m33 - m31*m23) + m03*(m21*m32 - m31*m22);
	det11 = m00*(m22*m33 - m32*m23) - m02*(m20*m33 - m30*m23) + m03*(m20*m32 - m30*m22);
	det12 = m00*(m21*m33 - m31*m23) - m01*(m20*m33 - m30*m23) + m03*(m20*m31 - m30*m21);
	det13 = m00*(m21*m32 - m31*m22) - m01*(m20*m32 - m30*m22) + m02*(m20*m31 - m30*m21);
	//������ʽ��ɴ�������ʽ  
	det10 *= -1; det12 *= -1;

	float det20, det21, det22, det23;
	det20 = m01*(m12*m33 - m32*m13) - m02*(m11*m33 - m31*m13) + m03*(m11*m32 - m31*m12);
	det21 = m00*(m12*m33 - m32*m13) - m02*(m10*m33 - m30*m13) + m03*(m10*m32 - m30*m12);
	det22 = m00*(m11*m33 - m31*m13) - m01*(m10*m33 - m30*m13) + m03*(m10*m31 - m30*m11);
	det23 = m00*(m11*m32 - m31*m12) - m01*(m10*m32 - m30*m12) + m02*(m10*m31 - m30*m11);
	//������ʽ��ɴ�������ʽ  
	det21 *= -1; det23 *= -1;

	float det30, det31, det32, det33;
	det30 = m01*(m12*m23 - m22*m13) - m02*(m11*m23 - m21*m13) + m03*(m11*m22 - m21*m12);
	det31 = m00*(m12*m23 - m22*m13) - m02*(m10*m23 - m20*m13) + m03*(m10*m22 - m20*m12);
	det32 = m00*(m11*m23 - m21*m13) - m01*(m10*m23 - m20*m13) + m03*(m10*m21 - m20*m11);
	det33 = m00*(m11*m22 - m21*m12) - m01*(m10*m22 - m20*m12) + m02*(m10*m21 - m20*m11);
	//������ʽ��ɴ�������ʽ  
	det30 *= -1; det32 *= -1;

	//�������M*/|M| ,1/|M|������Invdet  
	//ע�������������ǰ���Ԫ�ض�Ӧ�Ĵ�������ʽд������  
	m00 = det00*invdet; m10 = det01*invdet; m20 = det02*invdet; m30 = det03*invdet;
	m01 = det10*invdet; m11 = det11*invdet; m21 = det12*invdet; m31 = det13*invdet;
	m02 = det20*invdet; m12 = det21*invdet; m22 = det22*invdet; m32 = det23*invdet;
	m03 = det30*invdet; m13 = det31*invdet; m23 = det32*invdet; m33 = det33*invdet;
	
	return det;
}

//���������
CMatrix CMatrix::GetInverse() {
	CMatrix cm = (*this);
	cm.Inverse();
	return cm;
}

//ִ�е���ǰ������
void CMatrix::run() {
	glMultMatrixf(*this);
}

string CMatrix::toString() {
	ostringstream buffer;
	buffer << setiosflags(ios::fixed);
	buffer << setprecision(2);
	for (int i = 0; i < 15; i++) {
		buffer << (*this)[i] << ",";
	}
	buffer << (*this)[15];
	return string(buffer.str());
}

//����ת��
CMatrix::operator float *() {
	return &m00;
}
