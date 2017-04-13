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
//��������������
//==============
class CVector3 {
public:
	float x, y, z;

	//���캯��
	CVector3();

	//�������Ĺ��캯��
	CVector3(float x, float y, float z);

	void Set(float x, float y, float z);

	//������˲���
	float dotMul(CVector3 &n);

	//������˲���
	CVector3 crossMul(CVector3 &n);

	//������׼������
	void Normalize();

	//������ģ����
	float len();

	//����ͶӰ����
	CVector3 project(CVector3 &n);

	//��������'+'
	CVector3 operator+(const CVector3 &b);

	//��������'-'
	CVector3 operator-(const CVector3 &b);

	//�����������˲���
	CVector3 operator*(float b);

	//����'=='�Ƚϲ���
	bool operator == (const CVector3 &b);

	//����'!='�Ƚϲ���
	bool operator != (const CVector3 &b);

	//���ظ�ֵ����
	CVector3 operator=(const CVector3 &p);

	//����ת��Ϊŷ����
	CEuler ToEuler();

	//����Java��toString,������xyz��ֵ����string
	string toString();

	//����ת��
	operator float*();

};

//==============
//��������������
//==============
class CMatrix {
public:
	float m00, m10, m20, m30;
	float m01, m11, m21, m31;
	float m02, m12, m22, m32;
	float m03, m13, m23, m33;

	//���캯��
	CMatrix();

	//��������
	void Zero();

	//����Ϊ��λ����
	void Identity();

	//������16��Ԫ�ظ�ֵ
	void Set(float *p);

	//���õ�i��Ԫ��
	void SetRow(int i, CVector3 p);

	//���õ�i��Ԫ��
	void SetCol(int i, CVector3 p);

	//����ֵ
	CMatrix & operator=(CMatrix &p);

	//��������
	CMatrix operator*(float d);

	//�������
	CMatrix operator*(CMatrix &p);

	//�������������
	CVector3 vecMul(const CVector3 &p);

	//����������
	CVector3 posMul(const CVector3 &p);

	//����ת��
	void Transpose();

	//����Ϊ��ת����
	CMatrix &SetRotate(float seta, CVector3 axis);

	//����Ϊƽ�ƾ���
	CMatrix &SetTrans(CVector3 trans);

	//����Ϊ���ž���
	CMatrix &SetScale(CVector3 p);

	//�������棬�ɹ���������ʽ��ֵ�����򷵻�0
	float Inverse();

	//���������
	CMatrix GetInverse();

	//����ת��Ϊŷ����
	CEuler ToEuler();

	//����ת��Ϊ��Ԫ��
	CQuaternion ToQuaternion();

	//ִ�е���ǰ������
	void run();

	//����Java��toString
	string toString();

	//����ת��
	operator float *();

};

//================
//ŷ��������������
//================
class CEuler {
public:
	float h, p, b;
	
	//��ֵ����
	void Set(float h, float p, float b);

	//���캯��
	CEuler();

	//�������Ĺ��캯��
	CEuler(float h, float p, float b);

	//����ת��
	operator float*();

	//ŷ����ת��Ϊ����
	CVector3 ToVector3(CVector3 *updir=0);

	//ŷ����ת��Ϊ����
	CMatrix ToMatrix();

	//ŷ����ת��Ϊ��Ԫ��
	CQuaternion ToQuaternion();

	//ŷ���ǹ淶��
	void Normal();
};
//================
//��Ԫ������������
//================
class CQuaternion {
public:
	float x, y, z, w;

	//Ĭ�Ϲ��캯��
	CQuaternion();

	//��ֵ���캯��
	CQuaternion(float x, float y, float z, float w);

	//��ֵ����
	void Set(float x, float y, float z, float w);

	//����ת��
	operator float*();

	//��Ԫ������
	void SetAngle(float angle, CVector3 axis);

	//���ظ�ֵ
	CQuaternion & operator=(const CQuaternion &p);

	//���ء�+��
	CQuaternion operator+(const CQuaternion &p);

	//��������
	CQuaternion operator*(float data);

	//��Ԫ���˷�(�¶���˷�)
	CQuaternion operator*(const CQuaternion &p);

	//���
	float dotMul(const CQuaternion&p);

	//��ģ
	float len();

	//����
	CQuaternion conjugate();

	//���׼��
	bool Normalize();

	//������Ԫ��,��ı�����
	CQuaternion & Inverse();

	//������Ԫ��,���ı����������µ���Ԫ��
	CQuaternion GetInverse();

	//��� ��ǰΪa,��c=a-b
	CQuaternion Div(const CQuaternion &b);

	//����
	CQuaternion exp(const float t);

	//����ת��ͽǶ�
	void GetAngle(float &angle, CVector3 &axis);

	//��ֵ���ӵ�ǰ��Ԫ����ֵ��Vend��Ԫ��,t�ǲ���[0,1]
	CQuaternion Slerp(const CQuaternion &Vend, float t);

	//��ֵ��һ�β�ֵ��n�����ݡ���ֵ��������������t�У�������ص�����Result��
	void Slerp(const CQuaternion &Vend, int n, float *t, CQuaternion *Result);

	//��Ԫ��ת��Ϊŷ����
	CEuler ToEuler();

	//��Ԫ��ת��Ϊ����
	CMatrix ToMatrix();
};

//============
//ѭ�����ж���
//============
template <class T>
class cycleQueue
{
private:
	unsigned int m_size;
	int m_front;
	int m_now;//�����������
	int m_rear;
	int now_size;
	T*  m_data;
public:
	//���캯��
	cycleQueue(unsigned size) :m_size(size), m_front(0), m_rear(0) {
		m_data = new T[size];
	}

	//��������
	~cycleQueue() {
		delete[] m_data;
	}

	//��ʼ��������
	void reset() {
		m_now = m_front;
	}

	//��ǰ������Ԫ��
	T now() throw(bad_exception) {
		if (this->isEmpty()) {
			throw bad_exception();
		}
		return m_data[m_now];
	}

	//����������һ��Ԫ��
	T next() throw(bad_exception) {
		if (m_now == m_rear) {
			throw bad_exception();
		}
		m_now = (m_now + 1) % m_size;
		return m_data[m_now];
	}

	//��ǰ������Ԫ������
	int size() {
		return now_size;
	}

	//ѭ�������Ƿ��
	bool isEmpty() {
		return m_front == m_rear;
	}

	//ѭ�������Ƿ���
	bool isFull() {
		return m_front == (m_rear + 1) % m_size;
	}

	//��β����Ԫ��
	void push(T ele)throw(bad_exception) {
		if (isFull()) {
			throw bad_exception();
		}
		now_size++;
		m_data[m_rear] = ele;
		m_rear = (m_rear + 1) % m_size;
	}

	//��ͷȡ����ɾ��Ԫ��
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

//================
//ѭ��ջ���Դ�������
//��д��http://www.cnblogs.com/diegodu/p/4619104.html
//================

template <class T>
class cycleStack
{
private:
	unsigned int m_size;
	int m_front;
	int m_end;
	int m_now;//���嵱ǰָ��
	int now_size;
	T*  m_data;
public:
	//���캯��
	cycleStack(unsigned size) :m_size(size), m_front(0), m_end(0) {
		m_data = new T[size];
	}

	//��������
	~cycleStack() {
		delete[] m_data;
	}

	//ѭ��ջ�Ƿ��
	bool isEmpty() {
		return m_front == m_end;
	}

	//ѭ��ջ�Ƿ���
	bool isFull() {
		return m_front == (m_end + 1) % m_size;
	}

	//ջβ����Ԫ��
	void push(T ele) {
		//���ջ�������ų�����һ��Ԫ�أ������洢��ǰԪ��
		if (isFull()) {
			m_front = (m_front + 1) % m_size;
		}
		else {
			now_size++;
		}
		m_end = (m_end + 1) % m_size;
		m_data[m_end] = ele;
	}

	//ջβȡ����ɾ��Ԫ��
	T pop() {
		if (isEmpty()) {
			printf("No Previous record!\n");
			return m_data[m_end];
		}
		now_size--;
		T tmp = m_data[m_end];
		m_end = (m_size + m_end - 1) % m_size;
		return tmp;
	}

	//��ʼ��ָ��λ��
	void reset() {
		m_now = m_end;
	}

	//��ȡ��һ����Ԫ��
	T getPre() {
		if ((m_size + m_now - 1) % m_size == m_front) {
			printf("No privious element!\n");
			return m_data[m_now];
		}
		T tmp = m_data[m_now];
		m_now = (m_size + m_now - 1) % m_size;
		return tmp;
	}

	//��ȡ��һ��Ԫ��
	T getNext() {
		if (m_now == m_end) {
			printf("No next element!\n");
			return m_data[m_now];
		}
		m_now = (m_now + 1) % m_size;
		return m_data[m_now];
	}

	//�ӵ�ǰλ�ÿ�ʼ����ջ
	void update() {
		m_end = m_now;
	}

	//��ǰ������Ԫ������
	int size() {
		return now_size;
	}
};

#endif