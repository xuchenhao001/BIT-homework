#pragma once

#define PAI (3.14159)
//����ת�Ƕ�
#define RadToAng(x) ((x)/PAI*180)
//�Ƕ�ת����
#define AngToRad(x) ((x)/180.0*PAI)

class CglVector3;
class CglMatrix;
class CglQuaternion;

class AFX_API_EXPORT CglEuler {
public:
	double h, p, b;

	CglEuler();
	CglEuler(double h, double p, double b);
	~CglEuler();

	void Set(double h, double p, double b);
	CglVector3 ToVector3(CglVector3 *updir = 0);
	CglMatrix ToMatrix();
	CglQuaternion ToQuaternion();
	void Normal();
	operator double*();
};

