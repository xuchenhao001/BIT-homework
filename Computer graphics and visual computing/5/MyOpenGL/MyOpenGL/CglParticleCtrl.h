#pragma once

#include "CglParticle.h"
#include "gl\glut.h"

class AFX_API_EXPORT CglParticleCtrl {
public:
	CglParticleCtrl();
	~CglParticleCtrl();

	void Draw();
	void UpdatePos();	//�������ӵ�λ��
	void Init(int num);

private:
	void ResetParticle(int id, bool bFirst = false);	//�������ӵĳ�ʼ��״̬
	CglParticle m_particle[10000];	//Ԥ����һ��������
	int m_numParticle;//�����õ����Ӹ���
	GLuint m_textureid;//��������
	int m_lasttime;//��¼�������е��ϴ�ʱ��
};

