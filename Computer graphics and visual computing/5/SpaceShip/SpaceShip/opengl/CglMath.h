#pragma once

#include "gl\glut.h"
#include "CglVector3.h"

class AFX_API_EXPORT CglMath
{
public:
	CglMath();
	~CglMath();

	static double InverseMatrix(double Matrix[], double inverseMatrix[]);
	static void CalculateNormal(CglVector3 *vert, CglVector3 *norm, int numRow, int numCol);
	static void Dir2HPR(CglVector3 dir, CglVector3 up, CglVector3 &hpr);
};

