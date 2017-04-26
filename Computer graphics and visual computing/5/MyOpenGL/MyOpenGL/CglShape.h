#pragma once

#include "CglVector3.h"


class AFX_API_EXPORT CglShape {
public:
	CglVector3 *m_vertArray;	//顶点
	CglVector3 *m_normalArray;	//法线
	CglVector3 *m_texArray;	//纹理坐标
	int	*m_indexArray;
	int m_nVert;			//需要绘制的点的个数

	CglShape();
	~CglShape();

	void DrawCoord(double len);
	void DrawBall(float radius, int lon, int lat);
};

