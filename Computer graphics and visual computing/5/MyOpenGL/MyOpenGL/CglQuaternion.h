#pragma once

#define PAI (3.14159)
//弧度转角度
#define RadToAng(x) ((x)/PAI*180)
//角度转弧度
#define AngToRad(x) ((x)/180.0*PAI)

class CglVector3;
class CglEuler;
class CglMatrix;

class CglQuaternion {
public:
	double x, y, z, w;

	CglQuaternion();
	CglQuaternion(double x, double y, double z, double w);
	~CglQuaternion();

	void Set(double x, double y, double z, double w);
	void SetAngle(double angle, CglVector3 axis);
	operator double*();
	CglQuaternion & operator=(const CglQuaternion &p);
	CglQuaternion operator+(const CglQuaternion &p);
	CglQuaternion operator*(double data);
	CglQuaternion operator*(const CglQuaternion &p);
	double dotMul(const CglQuaternion&p);
	double len();
	CglQuaternion conjugate();
	bool Normalize();
	CglQuaternion & Inverse();
	CglQuaternion GetInverse();
	CglQuaternion Div(const CglQuaternion &b);
	CglQuaternion exp(const double t);
	void GetAngle(double &angle, CglVector3 &axis);
	CglQuaternion Slerp(const CglQuaternion &Vend, double t);
	void Slerp(const CglQuaternion &Vend, int n, double *t, CglQuaternion *Result);
	CglEuler ToEuler();
	CglMatrix ToMatrix();
};

