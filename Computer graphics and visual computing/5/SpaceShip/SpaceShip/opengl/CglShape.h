#pragma once

#include "CglVector3.h"


class AFX_API_EXPORT CglShape {
public:
	CglVector3 *m_vertArray;	//����
	CglVector3 *m_normalArray;	//����
	CglVector3 *m_texArray;	//��������
	int	*m_indexArray;
	int m_nVert;			//��Ҫ���Ƶĵ�ĸ���

	CglShape();
	~CglShape();

	void DrawCoord(double len);
	void DrawBall(float radius, int lon, int lat);
};

