#pragma once

#include "CglParticle.h"
#include "gl\glut.h"

class AFX_API_EXPORT CglParticleCtrl {
public:
	CglParticleCtrl();
	~CglParticleCtrl();

	//β�沿��
	void DrawTail();
	void UpdateTail();							
	void InitTail(int num, char* materialName);

private:
	void ResetTail(int id, bool bFirst = false);	//����β��ĳ�ʼ��״̬
	CglParticle particleTail[1001];	//Ԥ����β������
	int m_numParticle;//�����õ����Ӹ���
	GLuint tailTextureid;//β������
	GLuint patchTextureid;//��Ƭ����
	int m_lasttime;//��¼�������е��ϴ�ʱ��
};

