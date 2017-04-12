#include "stdafx.h"
#include <iostream>
#include <GL/glut.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <math.h>
#include "MyOpenGlClass.h"

using namespace std;

float toRad(float x) {
	return x / 180 * 3.1415926;
}

float toAngle(float x) {
	return x / 3.1415926 * 180;
}

//==============
//向量类定义部分
//==============
const float deviation = 0.00001;//误差设定

//构造函数
CVector3::CVector3() {
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

//带参数的构造函数
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

//向量点乘操作
float CVector3::dotMul(CVector3 &n) {
	return this->x*n.x + this->y*n.y + this->z*n.z;
}

//向量叉乘操作
CVector3 CVector3::crossMul(CVector3 &n) {
	return CVector3(this->y*n.z - n.y*this->z, this->z*n.x - n.z*this->x, this->x*n.y - n.x*this->y);
}

//向量标准化操作
void CVector3::Normalize() {
	float length = this->len();
	if (length < deviation) {
		printf("零向量无法标准化！");
		return;
	}
	this->x /= length;
	this->y /= length;
	this->z /= length;
}

//向量求模操作
float CVector3::len() {
	return sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
}

//向量投影操作
CVector3 CVector3::project(CVector3 &n) {
	float length = n.len();
	if (length < deviation) {
		return CVector3();
	}
	float proportion = this->dotMul(n) / length / length;
	return CVector3(n.x*proportion, n.y*proportion, n.z*proportion);
}

//重载向量'+'
CVector3 CVector3::operator+(const CVector3 &b) {
	return CVector3(this->x + b.x, this->y + b.y, this->z + b.z);
}

//重载向量'-'
CVector3 CVector3::operator-(const CVector3 &b) {
	return CVector3(this->x - b.x, this->y - b.y, this->z - b.z);
}

//重载向量数乘操作'*'
CVector3 CVector3::operator*(float b) {
	return CVector3(this->x*b, this->y*b, this->z*b);
}

//重载'=='比较操作
bool CVector3::operator==(const CVector3 &b) {
	if (this->x == b.x&&this->y == b.y&&this->z == b.z) {
		return true;
	}
	return false;
}

//重载'!='比较操作
bool CVector3::operator!=(const CVector3 &b) {
	if (this->x == b.x&&this->y == b.y&&this->z == b.z) {
		return false;
	}
	return true;
}

//重载赋值操作
CVector3 CVector3::operator=(const CVector3 &p) {
	x = p.x;
	y = p.y;
	z = p.z;
	return *this;
}

//仿照Java中toString,将向量xyz的值存入string
string CVector3::toString() {
	ostringstream buffer;
	buffer << setiosflags(ios::fixed);
	buffer << setprecision(2) << this->x << "," << this->y << "," << this->z;
	return string(buffer.str());
}

//类型转换
CVector3::operator float*() {
	return &x;
}

//向量转换为欧拉角
CEuler CVector3::ToEuler()
{
	CVector3 a(x, y, z), b(x, 0, z), z(0, 0, -1);
	float h = acosf(b.dotMul(z) / (b.len()*z.len()));
	if (x > 0) {
		h = toAngle(-fabs(h));
	}
	else {
		h = toAngle(fabs(h));
	}
	float p = acosf(a.dotMul(b) / (a.len()*b.len()));
	if (y > 0) {
		p = toAngle(fabs(p));
	}
	else {
		p = toAngle(-fabs(p));
	}
	return CEuler(h, p, 0);
}

//==============
//矩阵类定义部分
//==============

//构造函数(默认构造单位矩阵)
CMatrix::CMatrix() {
	(*this).Identity();
}

//矩阵清零
void CMatrix::Zero() {
	for (int i = 0; i < 16; i++) {
		(*this)[i] = 0;
	}
}

//设置为单位矩阵
void CMatrix::Identity() {
	this->Zero();
	this->m00 = this->m11 = this->m22 = this->m33 = 1;
}

//给矩阵16个元素赋值
void CMatrix::Set(float *p) {
	for (int i = 0; i < 16; i++) {
		(*this)[i] = p[i];
	}
}

//设置第i行元素
void CMatrix::SetRow(int i, CVector3 p) {
	switch (i) {
	case 0: m00 = p.x; m01 = p.y; m02 = p.z; m03 = 0; break;
	case 1: m10 = p.x; m11 = p.y; m12 = p.z; m13 = 0; break;
	case 2: m20 = p.x; m21 = p.y; m22 = p.z; m23 = 0; break;
	case 3: m30 = p.x; m31 = p.y; m32 = p.z; m33 = 1; break;
	}
}

//设置第i列元素
void CMatrix::SetCol(int i, CVector3 p) {
	switch (i) {
	case 0: m00 = p.x; m10 = p.y; m20 = p.z; m30 = 0; break;
	case 1: m01 = p.x; m11 = p.y; m21 = p.z; m31 = 0; break;
	case 2: m02 = p.x; m12 = p.y; m22 = p.z; m32 = 0; break;
	case 3: m03 = p.x; m13 = p.y; m23 = p.z; m33 = 1; break;
	}
}

//矩阵赋值
CMatrix& CMatrix:: operator=(CMatrix &p) {
	for (int i = 0; i < 16; i++) {
		(*this)[i] = p[i];
	}
	return (*this);
}

//矩阵数乘
CMatrix CMatrix::operator*(float d) {
	for (int i = 0; i < 16; i++) {
		(*this)[i] *= d;
	}
	return (*this);
}

//矩阵相乘
CMatrix CMatrix::operator*(CMatrix &p) {
	CMatrix result;
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

//矩阵与向量相乘
CVector3 CMatrix::vecMul(const CVector3 &p) {
	CVector3 cv;
	for (int i = 0; i < 3; i++) {
		cv[i] = (*this)[i] * p.x + (*this)[4 + i] * p.y + (*this)[8 + i] * p.z;
	}
	return cv;
}

//矩阵与点相乘
CVector3 CMatrix::posMul(const CVector3 &p) {
	CVector3 cv;
	for (int i = 0; i < 3; i++) {
		cv[i] = (*this)[i] * p.x + (*this)[4 + i] * p.y + (*this)[8 + i] * p.z + (*this)[12 + i];
	}
	return cv;
}

//矩阵转置
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

//设置为旋转矩阵
CMatrix& CMatrix::SetRotate(float seta, CVector3 axis) {
	seta = toRad(seta);

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

//设置为平移矩阵
CMatrix& CMatrix::SetTrans(CVector3 trans) {
	(*this)[0] = (*this)[5] = (*this)[10] = (*this)[15] = 1;
	(*this)[12] = trans.x;
	(*this)[13] = trans.y;
	(*this)[14] = trans.z;
	return (*this);
}

//设置为缩放矩阵
CMatrix& CMatrix::SetScale(CVector3 p) {
	(*this)[0] = p.x;
	(*this)[5] = p.y;
	(*this)[10] = p.z;
	(*this)[15] = 1;
	return (*this);
}

//矩阵求逆，成功返回行列式的值，否则返回0
float CMatrix::Inverse() {
	//计算|M|,结果保存在det中，det00->det03计算的每个元素对应的代数余子式
	//计算过程就是把四阶的行列式展开成三阶的行列式
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
	//把余子式变成代数余子式  
	det01 *= -1; det03 *= -1;
	//计算det10->det33 对应元素的代数余子式  
	float det10, det11, det12, det13;
	det10 = m01*(m22*m33 - m32*m23) - m02*(m21*m33 - m31*m23) + m03*(m21*m32 - m31*m22);
	det11 = m00*(m22*m33 - m32*m23) - m02*(m20*m33 - m30*m23) + m03*(m20*m32 - m30*m22);
	det12 = m00*(m21*m33 - m31*m23) - m01*(m20*m33 - m30*m23) + m03*(m20*m31 - m30*m21);
	det13 = m00*(m21*m32 - m31*m22) - m01*(m20*m32 - m30*m22) + m02*(m20*m31 - m30*m21);
	//把余子式变成代数余子式  
	det10 *= -1; det12 *= -1;

	float det20, det21, det22, det23;
	det20 = m01*(m12*m33 - m32*m13) - m02*(m11*m33 - m31*m13) + m03*(m11*m32 - m31*m12);
	det21 = m00*(m12*m33 - m32*m13) - m02*(m10*m33 - m30*m13) + m03*(m10*m32 - m30*m12);
	det22 = m00*(m11*m33 - m31*m13) - m01*(m10*m33 - m30*m13) + m03*(m10*m31 - m30*m11);
	det23 = m00*(m11*m32 - m31*m12) - m01*(m10*m32 - m30*m12) + m02*(m10*m31 - m30*m11);
	//把余子式变成代数余子式  
	det21 *= -1; det23 *= -1;

	float det30, det31, det32, det33;
	det30 = m01*(m12*m23 - m22*m13) - m02*(m11*m23 - m21*m13) + m03*(m11*m22 - m21*m12);
	det31 = m00*(m12*m23 - m22*m13) - m02*(m10*m23 - m20*m13) + m03*(m10*m22 - m20*m12);
	det32 = m00*(m11*m23 - m21*m13) - m01*(m10*m23 - m20*m13) + m03*(m10*m21 - m20*m11);
	det33 = m00*(m11*m22 - m21*m12) - m01*(m10*m22 - m20*m12) + m02*(m10*m21 - m20*m11);
	//把余子式变成代数余子式  
	det30 *= -1; det32 *= -1;

	//伴随矩阵M*/|M| ,1/|M|保存在Invdet  
	//注意这里伴随矩阵是把行元素对应的代数余子式写在列上  
	m00 = det00*invdet; m10 = det01*invdet; m20 = det02*invdet; m30 = det03*invdet;
	m01 = det10*invdet; m11 = det11*invdet; m21 = det12*invdet; m31 = det13*invdet;
	m02 = det20*invdet; m12 = det21*invdet; m22 = det22*invdet; m32 = det23*invdet;
	m03 = det30*invdet; m13 = det31*invdet; m23 = det32*invdet; m33 = det33*invdet;

	return det;
}

//返回逆矩阵
CMatrix CMatrix::GetInverse() {
	CMatrix cm = (*this);
	cm.Inverse();
	return cm;
}

//矩阵转换为欧拉角
CEuler CMatrix::ToEuler() {
	if (this->m12 == 1 || this->m12 == -1) {
		return CEuler(atan2f(this->m20, this->m00), this->m12*1.57, 0);
	}
	else {
		return CEuler(atan2f(this->m00, this->m22), asinf(-this->m12), atan2f(this->m10, this->m11));
	}
}

//矩阵转换为四元数
CQuaternion CMatrix::ToQuaternion() {
	float s;
	if (this->m00 > this->m11 && this->m00 > this->m22) {
		s = 2 * sqrt(1.0 + this->m00 - this->m11 - this->m22);
		return CQuaternion(0.25*s, (this->m10 + this->m01) / s, (this->m02 + this->m20) / s, (this->m21 - this->m12) / s);
	}
	else if (this->m11 > this->m22) {
		s = 2 * sqrt(1.0 + this->m11 - this->m00 - this->m22);
		return CQuaternion((this->m10 + this->m01) / s, 0.25*s, (this->m21 + this->m12) / s, (this->m02 - this->m20) / s);
	}
	else {
		s = 2 * sqrt(1.0 + this->m22 - this->m00 - this->m11);
		return CQuaternion((this->m02 + this->m20) / s, (this->m21 + this->m12) / s, 0.25*s, (this->m10 - this->m01) / s);
	}
}

//执行到当前矩阵中
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

//类型转换
CMatrix::operator float *() {
	return &m00;
}


//================
//欧拉角类定义部分
//================

//构造函数
CEuler::CEuler() {
	this->h = this->p = this->b = 0;
}

//含参量的构造函数
CEuler::CEuler(float h, float p, float b) {
	this->h = h;
	this->p = p;
	this->b = b;
}

//赋值函数
void CEuler::Set(float h, float p, float b) {
	this->h = h;
	this->p = p;
	this->b = b;
}

//类型转换
CEuler::operator float*() {
	return &h;
}

//欧拉角转换为向量
CVector3 CEuler::ToVector3(CVector3 *updir) {
	CMatrix m = this->ToMatrix();
	CVector3 dir = m.vecMul(CVector3(0, 0, -1));
	if (updir != 0) {
		*updir = m.vecMul(CVector3(0, 1, 0));
	}
	return dir;
}

//欧拉角转换为矩阵
CMatrix CEuler::ToMatrix() {
	float hRad = toRad(h);
	float pRad = toRad(p);
	float bRad = toRad(b);
	CMatrix m;
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

//欧拉角转换为四元数
CQuaternion CEuler::ToQuaternion() {
	float hRad = toRad(h);
	float pRad = toRad(p);
	float bRad = toRad(b);
	CQuaternion quater;
	quater.w = cos(hRad/2)*cos(pRad/2)*cos(bRad/2) + sin(hRad/2)*sin(pRad/2)*sin(bRad/2);
	quater.x = cos(hRad/2)*sin(pRad/2)*cos(bRad/2) + sin(hRad/2)*cos(pRad/2)*sin(bRad/2);
	quater.y = sin(hRad/2)*cos(pRad/2)*cos(bRad/2) - cos(hRad/2)*sin(pRad/2)*sin(bRad/2);
	quater.z = cos(hRad/2)*cos(pRad/2)*sin(bRad/2) - sin(hRad/2)*sin(pRad/2)*cos(bRad/2);
	return quater;
}

//欧拉角规范化
void CEuler::Normal() {
	if (fabs(p - 90) < deviation) {
		h -= b;
		b = 0;
	}
	else if (fabs(p + 90) < deviation) {
		h += b;
		b = 0;
	}
	//角度范围的限制
	if (h > 180) {
		(*this).h -= 360;
	}
	if (h < -180) {
		(*this).h += 360;
	}
	if (b > 180) {
		(*this).b -= 360;
	}
	if (b < -180) {
		(*this).b += 360;
	}
	if (p > 90 || p <-90) {
		(*this) = (*this).ToMatrix().ToEuler();
	}
}

//================
//四元数类定义部分
//================

//默认构造函数
CQuaternion::CQuaternion() {
	this->w = this->x = this->y = this->z = 0;
}

//赋值构造函数
CQuaternion::CQuaternion(float x, float y, float z, float w) {
	this->w = w;
	this->x = x;
	this->y = y;
	this->z = z;
}

//赋值函数
void CQuaternion::Set(float x, float y, float z, float w) {
	this->w = w;
	this->x = x;
	this->y = y;
	this->z = z;
}

//类型转换
CQuaternion::operator float*() {
	return &x;
}

//四元数设置
void CQuaternion::SetAngle(float angle, CVector3 axis) {
	angle *= 0.5f;
	axis.Normalize();
	this->x = (axis.x * sinf(angle));
	this->y = (axis.y * sinf(angle));
	this->z = (axis.z * sinf(angle));
	this->w = cos(angle);
}

//重载赋值
CQuaternion& CQuaternion::operator=(const CQuaternion &p) {
	return CQuaternion(p.x, p.y, p.z, p.w);
}

//重载‘+’
CQuaternion CQuaternion::operator+(const CQuaternion &p) {
	return CQuaternion(x + p.x, y + p.y, z + p.z, w + p.w);
}

//重载数乘
CQuaternion CQuaternion::operator*(float data) {
	return CQuaternion(x*data, y*data, z*data, w*data);
}

//四元数乘法(新定义乘法)
CQuaternion CQuaternion::operator*(const CQuaternion &p) {
	CQuaternion quater;
	quater.w = w*p.w - x*p.x - y*p.y - z*p.z;
	quater.x = w*p.x + x*p.w + z*p.y - y*p.z;
	quater.y = w*p.y + y*p.w + x*p.z - z*p.x;
	quater.z = w*p.z + z*p.w + y*p.x - x*p.y;
	return quater;
}

//点乘
float CQuaternion::dotMul(const CQuaternion&p) {
	return w*p.w + x*p.x + y*p.y + z*p.z;
}

//求模
float CQuaternion::len() {
	return sqrt(w*w + x*x + y*y + z*z);
}

//共轭
CQuaternion CQuaternion::conjugate() {
	return CQuaternion(-x, -y, -z, w);
}

//求标准化
bool CQuaternion::Normalize() {
	float n = this->len();
	if (fabs(n) < deviation || fabs(n - 1.0) > deviation) {
		printf("四元数模为0无法单位化！");
		return 0;
	}
	*this = (*this) * (1/n);
}

//求逆四元数,会改变自身
CQuaternion& CQuaternion::Inverse() {
	(*this) = this->GetInverse();
	return (*this);
}

//求逆四元数,不改变自身，生成新的四元数
CQuaternion CQuaternion::GetInverse() {
	float n = this->len();
	if (n == 0) {
		printf("四元数模为0无法求逆！");
		return (*this);
	}
	return this->conjugate()*(1 / (n*n));
}

//求差 当前为a,求c=a-b
CQuaternion CQuaternion::Div(const CQuaternion &b) {
	return this->Inverse()*b;
}

//求幂
CQuaternion CQuaternion::exp(const float t) {
	float angle;
	CVector3 vect;
	this->GetAngle(angle, vect);
	vect = vect*(sinf(t*angle / 2));
	return CQuaternion(vect.x, vect.y, vect.z, cosf(t*angle / 2));
}

//求旋转轴和角度
void CQuaternion::GetAngle(float &angle, CVector3 &axis) {
	this->Normalize();
	float n = this->len();
	axis = CVector3(this->x / n, this->y / n, this->z / n);
	angle = acosf(this->w)*2.0;
}

//插值。从当前四元数插值到Vend四元数,t是参数[0,1]
CQuaternion CQuaternion::Slerp(const CQuaternion &Vend, float t) {
	return (*this)*((this->Inverse()*Vend).exp(t));
}

//插值。一次插值出n个数据。插值参数保存在数组t中，结果返回到数组Result中
void CQuaternion::Slerp(const CQuaternion &Vend, int n, float *t, CQuaternion *Result) {
	for (int i = 0; i < n; i++) {
		*(Result + i) = Slerp(Vend, *(t + i));
	}
}

//四元数转换为欧拉角
CEuler CQuaternion::ToEuler() {
	CEuler euler;
	euler.p = asin(2 * w*x - 2 * y*z);
	if (fabs(cos(euler.p)) < deviation) {
		euler.h = atan2(2 * w*y - 2 * z*x, 1 - 2 * y*y - 2 * z*z);
		euler.b = 0;
	}
	else
	{
		euler.h = atan2(2 * z*x + 2 * w*y, 1 - 2 * x*x - 2 * y*y);
		euler.b = atan2(2 * w*z + 2 * y*x, 1 - 2 * x*x - 2 * z*z);
	}
	return euler;
}

//四元数转换为矩阵
CMatrix CQuaternion::ToMatrix() {
	CMatrix matrix;
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
