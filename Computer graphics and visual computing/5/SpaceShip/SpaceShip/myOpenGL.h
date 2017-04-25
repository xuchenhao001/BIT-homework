// myOpenGL.h: interface for the CmyOpenGL class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYOPENGL_H__9210654D_319B_4970_950D_A11A11AE5ECB__INCLUDED_)
#define AFX_MYOPENGL_H__9210654D_319B_4970_950D_A11A11AE5ECB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "opengl/OpenGL.h"
#pragma comment(lib,"opengl/myOpenGL.lib")
#define MAX_ARRAY_SIZE 10000
class CmyOpenGL : public COpenGL
{
public:
	//计算顶点的法向量
	virtual void InDraw();
	virtual void PostInit();
	CmyOpenGL();
	virtual ~CmyOpenGL();
	bool m_bWired;
private:
	CglParticleCtrl m_particle;
	void DrawModel();
	virtual bool OnKey(unsigned int nChar, bool bDown);
};

#endif // !defined(AFX_MYOPENGL_H__9210654D_319B_4970_950D_A11A11AE5ECB__INCLUDED_)
