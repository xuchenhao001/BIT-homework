#pragma once

#include "CglVector3.h"

class AFX_API_EXPORT CglCamera
{
public:
	CglCamera();
	~CglCamera();

	CglVector3 m_pos;	//�ӵ㵱ǰλ��
	CglVector3 m_hpr;	//�ӵ㷽��ǣ�xΪh,yΪp,zΪr
	CglVector3 m_eyedir;//���߷���
	CglVector3 m_updir;	//�Ϸ�����
	double m_viewMatrix[16];	//��ͼ����
	double m_viewMatrixInverse[16];	//��ͼ����������
	double m_viewMatrixSave[16];	//�������ͼ����
	int m_type;//	������͡�0��ʾ��������(hpr�ۻ�)��1��ʾ�������Σ�2��ʾ��������

	void SetType(int type);
	void LoadCamera();
	void SaveCamera();
	void Init();
	void SetCamera(const CglVector3& pos, const CglVector3& obj_dir, bool bObj = true, int type = 0);
	void ShowView();
	void Rotate(int dir, double len);
	void Move(int dir, double len);
};

