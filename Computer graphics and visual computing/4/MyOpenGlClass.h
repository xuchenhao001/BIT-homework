#ifndef _MMV_H_
#define _MMV_H_

#pragma once

#include "stdafx.h"
#include <iostream>
#include <string>

using namespace std;

class CVector3;
class CMatrix;
class CEuler;
class CQuaternion;

//==============
//向量类声明部分
//==============
class CVector3 {
public:
	float x, y, z;

	friend CVector3 operator+(const CVector3 &a, const CVector3 &b);		//重载向量'+'
	friend CVector3 operator-(const CVector3 &a, const CVector3 &b);		//重载向量'-'
	friend CVector3 operator*(const CVector3 &a, float &b);					//重载向量数乘操作
	friend bool operator == (const CVector3 &a, const CVector3 &b);			//重载'=='比较操作
	friend bool operator != (const CVector3 &a, const CVector3 &b);			//重载'!='比较操作

	//构造函数
	CVector3();

	//带参数的构造函数
	CVector3(float x, float y, float z);

	void Set(float x, float y, float z);

	//向量点乘操作
	float dotMul(CVector3 &n);

	//向量叉乘操作
	CVector3 crossMul(CVector3 &n);

	//向量标准化操作
	void Normalize();

	//向量求模操作
	float len();

	//向量投影操作
	CVector3 project(CVector3 &n);

	//重载赋值操作
	CVector3 operator=(const CVector3 &p);

	//向量转换为欧拉角
	CEuler ToEuler();

	//仿照Java中toString,将向量xyz的值存入string
	string toString();

	//类型转换
	operator float*();

};

//==============
//矩阵类声明部分
//==============
class CMatrix {
public:
	float m00, m10, m20, m30;
	float m01, m11, m21, m31;
	float m02, m12, m22, m32;
	float m03, m13, m23, m33;

	//构造函数
	CMatrix();

	//矩阵清零
	void Zero();

	//设置为单位矩阵
	void Identity();

	//给矩阵16个元素赋值
	void Set(float *p);

	//设置第i行元素
	void SetRow(int i, CVector3 p);

	//设置第i列元素
	void SetCol(int i, CVector3 p);

	//矩阵赋值
	CMatrix & operator=(CMatrix &p);

	//矩阵数乘
	CMatrix operator*(float d);

	//矩阵相乘
	CMatrix operator*(CMatrix &p);

	//矩阵与向量相乘
	CVector3 vecMul(const CVector3 &p);

	//矩阵与点相乘
	CVector3 posMul(const CVector3 &p);

	//矩阵转置
	void Transpose();

	//设置为旋转矩阵
	CMatrix &SetRotate(float seta, CVector3 axis);

	//设置为平移矩阵
	CMatrix &SetTrans(CVector3 trans);

	//设置为缩放矩阵
	CMatrix &SetScale(CVector3 p);

	//矩阵求逆，成功返回行列式的值，否则返回0
	float Inverse();

	//返回逆矩阵
	CMatrix GetInverse();

	//矩阵转换为欧拉角
	CEuler ToEuler();

	//矩阵转换为四元数
	CQuaternion ToQuaternion();

	//执行到当前矩阵中
	void run();

	//仿照Java中toString
	string toString();

	//类型转换
	operator float *();

};

//================
//欧拉角类声明部分
//================
class CEuler {
public:
	float h, p, b;
	
	//赋值函数
	void Set(float h, float p, float b);

	//构造函数
	CEuler();

	//含参量的构造函数
	CEuler(float h, float p, float b);

	//类型转换
	operator float*();

	//欧拉角转换为向量
	CVector3 ToVector3(CVector3 *updir=0);

	//欧拉角转换为矩阵
	CMatrix ToMatrix();

	//欧拉角转换为四元数
	CQuaternion ToQuaternion();

	//欧拉角规范化
	void Normal();
};
//================
//四元数类声明部分
//================
class CQuaternion {
public:
	float x, y, z, w;

	//默认构造函数
	CQuaternion();

	//赋值构造函数
	CQuaternion(float x, float y, float z, float w);

	//赋值函数
	void Set(float x, float y, float z, float w);

	//类型转换
	operator float*();

	//四元数设置
	void SetAngle(float angle, CVector3 axis);

	//重载赋值
	CQuaternion & operator=(const CQuaternion &p);

	//重载‘+’
	CQuaternion operator+(const CQuaternion &p);

	//重载数乘
	CQuaternion operator*(float data);

	//四元数乘法
	CQuaternion operator*(const CQuaternion &p);

	//点乘
	float dotMul(const CQuaternion&p);

	//求模
	float len();

	//求标准化
	bool Normalize();

	//求逆四元数,会改变自身
	CQuaternion & Inverse();

	//求逆四元数,不改变自身，生成新的四元数
	CQuaternion GetInverse();

	//求差 当前为a,求c=a-b
	CQuaternion Div(const CQuaternion &b);

	//求旋转轴和角度
	void GetAngle(float &angle, CVector3 &axis);

	//插值。从当前四元数插值到Vend四元数,t是参数[0,1]
	CQuaternion Slerp(const CQuaternion &Vend, float t);

	//插值。一次插值出n个数据。插值参数保存在数组t中，结果返回到数组Result中
	void Slerp(const CQuaternion &Vend, int n, float *t, CQuaternion *Result);

	//四元数转换为欧拉角
	CEuler ToEuler();

	//四元数转换为矩阵
	CMatrix ToMatrix();
};

//============
//循环队列定义
//============
template <class T>
class cycleQueue
{
private:
	unsigned int m_size;
	int m_front;
	int m_now;//定义遍历变量
	int m_rear;
	int now_size;
	T*  m_data;
public:
	//构造函数
	cycleQueue(unsigned size) :m_size(size), m_front(0), m_rear(0) {
		m_data = new T[size];
	}

	//析构函数
	~cycleQueue() {
		delete[] m_data;
	}

	//初始化迭代器
	void reset() {
		m_now = m_front;
	}

	//当前迭代的元素
	T now() throw(bad_exception) {
		if (this->isEmpty()) {
			throw bad_exception();
		}
		return m_data[m_now];
	}

	//继续迭代下一个元素
	T next() throw(bad_exception) {
		if (m_now == m_rear) {
			throw bad_exception();
		}
		m_now = (m_now + 1) % m_size;
		return m_data[m_now];
	}

	//当前队列中元素数量
	int size() {
		return now_size;
	}

	//循环队列是否空
	bool isEmpty() {
		return m_front == m_rear;
	}

	//循环队列是否满
	bool isFull() {
		return m_front == (m_rear + 1) % m_size;
	}

	//队尾加入元素
	void push(T ele)throw(bad_exception) {
		if (isFull()) {
			throw bad_exception();
		}
		now_size++;
		m_data[m_rear] = ele;
		m_rear = (m_rear + 1) % m_size;
	}

	//队头取出并删除元素
	T pop() throw(bad_exception) {
		if (isEmpty()) {
			throw bad_exception();
		}
		now_size--;
		T tmp = m_data[m_front];
		m_front = (m_front + 1) % m_size;
		return tmp;
	}
};

#endif