#pragma once

#include "CglVector3.h"

class CglQuaternion;
class CglMatrix;

class AFX_API_EXPORT CglCamera {
public:
	CglCamera();
	~CglCamera();

	CglVector3 m_pos;				//视点当前位置
	CglVector3 m_hpr;				//视点方向角，x为h,y为p,z为r
	CglVector3 m_eyedir;			//视线方向
	CglVector3 m_updir;				//上方向量z
	double m_viewMatrix[16];		//视图矩阵
	double m_viewMatrixInverse[16];	//视图矩阵的逆矩阵
	double m_viewMatrixSave[16];	//保存的视图矩阵
	int m_type;						//相机类型。0表示地面漫游(hpr累积)，1表示任意漫游，2表示环绕漫游

	void SetType(int type);
	void LoadCamera();
	void SaveCamera();
	void Init();
	void SetCamera(const CglVector3& pos, const CglVector3& obj_dir, bool bObj = true, int type = 0);
	//相机跟随物体
	void followCamera(const CglVector3& pos, const CglVector3& obj_pos, CglVector3& updir);
	void ShowView();
	void Rotate(int dir, double len);
	void Move(int dir, double len);
	//从当前视图矩阵插值到目标矩阵,返回Result列表和pos列表
	double* Slerp(double m_toMatrix[16], int n, double *t, CglQuaternion *Result, CglVector3 *pos);
};

