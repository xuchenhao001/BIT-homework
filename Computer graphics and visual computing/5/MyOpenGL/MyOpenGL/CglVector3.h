#pragma once

#define PAI (3.14159)
//弧度转角度
#define RadToAng(x) ((x)/PAI*180)
//角度转弧度
#define AngToRad(x) ((x)/180.0*PAI)

class CglEuler;

class AFX_API_EXPORT CglVector3
{
public:
	double x, y, z;

	CglVector3();
	~CglVector3();

	CglVector3(double fx, double fy, double fz);
	CglVector3(double *pos);
	void Set(double fx, double fy, double fz);
	void Set(double *pos);
	void Get(double *pos);
	operator double*();
	operator const double*() const;
	CglVector3& operator=(const CglVector3& p);
	CglVector3& operator=(const CglEuler& p);
	CglVector3 operator+(const CglVector3& p) const;
	bool operator==(const CglVector3& p);
	CglVector3& operator+=(const CglVector3& p);
	CglVector3& operator-=(const CglVector3& p);
	CglVector3 operator-(const CglVector3& p) const;
	CglVector3 operator-() const;
	CglVector3 operator*(double data) const;
	friend CglVector3 operator*(double data, const CglVector3& p);
	CglVector3 operator*(double *mat) const;
	friend CglVector3 operator*(double *mat, const CglVector3& p);
	CglVector3 operator/(double data) const;
	double dotMul(const CglVector3&p);
	CglVector3 crossMul(const CglVector3&p) const;
	double len();
	void Normalize();
	CglVector3 project(CglVector3 &p);//向量投影操作
	void Add(int addr, double val);
	void Rotate(double seta, int axis);
	void Rotate(double seta, double x, double y, double z);
	CglEuler ToEuler();
};

