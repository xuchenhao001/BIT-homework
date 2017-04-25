// glVector3.h: interface for the CglVector3 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLVECTOR3_H__926CE975_74C8_4369_B898_7D3823497800__INCLUDED_)
#define AFX_GLVECTOR3_H__926CE975_74C8_4369_B898_7D3823497800__INCLUDED_

#include "math.h"


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PAI (3.14159)
//弧度转角度
#define RadToAng(x) ((x)/PAI*180)
//角度转弧度
#define AngToRad(x) ((x)/180.0*PAI)

class AFX_API_EXPORT CglVector3
{
public:
	double x, y, z;

	CglVector3(void);
	CglVector3(double fx, double fy, double fz);
	CglVector3(float *pos);
	CglVector3(double *pos);
	void Set(double fx, double fy, double fz);
	void Set(float *pos);
	void Set(double *pos);
	void Get(double *pos);
	void Get(float *pos);
	~CglVector3(void);
	operator double*();
	operator const double*() const;
	CglVector3& operator=(const CglVector3& p);
	CglVector3 operator+(const CglVector3& p) const;
	bool operator==(const CglVector3& p);
	CglVector3& operator+=(const CglVector3& p);
	CglVector3& operator-=(const CglVector3& p);
	CglVector3 operator-(const CglVector3& p) const;
	CglVector3 operator-() const;
	CglVector3 operator*(double data) const;
	friend CglVector3 operator*(double data, const CglVector3& p);
	CglVector3 operator*(float *mat) const;
	friend CglVector3 operator*(float *mat, const CglVector3& p);
	CglVector3 operator*(double *mat) const;
	friend CglVector3 operator*(double *mat, const CglVector3& p);
	CglVector3 operator/(double data) const;
	double dotMul(const CglVector3&p);
	CglVector3 crossMul(const CglVector3&p) const;
	double len();
	void Normalize();
	void Add(int addr, double val);
	void Rotate(double seta, int axis);
	void Rotate(double seta, double x, double y, double z);
};

#endif // !defined(AFX_GLVECTOR3_H__926CE975_74C8_4369_B898_7D3823497800__INCLUDED_)
