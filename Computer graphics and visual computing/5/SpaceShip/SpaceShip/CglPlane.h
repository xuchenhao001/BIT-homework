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
	void Draw(double size, double prop);
	float Move(int dir, double plus_or_minus, float usetime);
	void Rotate(int dir, double plus_or_minus, float usetime);
	void SetSpeed(float mspeed, float rspeed);
	void SetDir(CglVector3 dir);
	float GetMSpeed();
	float GetRSpeed();
	int GetUseTime();
	void Trick(float usetime);//特技动作
private:
	int lasttime = -1;
	double scale_step[2];		//移动和旋转步长缩放倍率
	float key_step;		//键盘按下一个运动命令代表移动步长
};

