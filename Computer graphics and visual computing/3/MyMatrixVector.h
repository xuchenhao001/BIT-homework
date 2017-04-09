#ifndef _MMV_H_
#define _MMV_H_

#pragma once

#include "stdafx.h"
#include <iostream>
#include <string>

using namespace std;

//==============
//��������������
//==============
class CVector3 {
public:
	float x, y, z;

	friend CVector3 operator+(const CVector3 &a, const CVector3 &b);		//��������'+'
	friend CVector3 operator-(const CVector3 &a, const CVector3 &b);		//��������'-'
	friend CVector3 operator*(const CVector3 &a, float &b);					//�����������˲���
	friend bool operator == (const CVector3 &a, const CVector3 &b);			//����'=='�Ƚϲ���
	friend bool operator != (const CVector3 &a, const CVector3 &b);			//����'!='�Ƚϲ���

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

	//���ظ�ֵ����
	CVector3 operator=(const CVector3 &p);

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

	//ִ�е���ǰ������
	void run();

	//����Java��toString
	string toString();

	operator float *();

};


#endif