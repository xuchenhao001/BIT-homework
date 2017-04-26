#pragma once

#define PAI (3.14159)
//弧度转角度
#define RadToAng(x) ((x)/PAI*180)
//角度转弧度
#define AngToRad(x) ((x)/180.0*PAI)

class CglVector3;
class CglEuler;
class CglQuaternion;

class AFX_API_EXPORT CglMatrix
{
public:
	double m00, m10, m20, m30;
	double m01, m11, m21, m31;
	double m02, m12, m22, m32;
	double m03, m13, m23, m33;

	CglMatrix();//构造函数(默认构造单位矩阵)
	~CglMatrix();

	void Zero();
	void Identity();
	void Set(double *p);
	void SetRow(int i, CglVector3 p);
	void SetCol(int i, CglVector3 p);
	CglMatrix & operator=(CglMatrix &p);
	CglMatrix operator*(double d);
	CglMatrix operator*(CglMatrix &p);
	CglVector3 vecMul(const CglVector3 &p);
	CglVector3 posMul(const CglVector3 &p);
	void Transpose();
	CglMatrix &SetRotate(double seta, CglVector3 axis);
	CglMatrix &SetTrans(CglVector3 trans);
	CglMatrix &SetScale(CglVector3 p);
	double Inverse();
	CglMatrix GetInverse();
	CglEuler ToEuler();
	CglQuaternion ToQuaternion();
	operator double*();
};

