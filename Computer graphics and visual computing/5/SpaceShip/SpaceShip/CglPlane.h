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
	void Trick(float usetime);//�ؼ�����
private:
	int lasttime = -1;
	double scale_step[2];		//�ƶ�����ת�������ű���
	float key_step;		//���̰���һ���˶���������ƶ�����
};

