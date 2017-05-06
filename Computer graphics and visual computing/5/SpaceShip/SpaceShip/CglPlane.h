#pragma once

#include "opengl/OpenGL.h"
#pragma comment(lib,"opengl/MyOpenGL")

class CglPlane {
public:
	CglVector3 m_pos;
	CglVector3 m_dir;
	CglVector3 m_updir;
	CglMatrix m_matrix;

	CglPlane();
	~CglPlane();

	void Init();
	void Draw(double size);
	void Move(int dir, double plus_or_minus);
	void Rotate(int dir, double plus_or_minus);
	void SetSpeed(float mspeed, float rspeed);
	float getMSpeed();
	float getRSpeed();
private:
	double m_fStep[2];		//移动和旋转步长缩放倍率
	float m_fKeyStep;		//键盘按下一个运动命令代表移动步长
};

