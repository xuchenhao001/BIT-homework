#pragma once

#include "gl\glut.h"
#include "CglCamera.h"
#include "CglControl.h"
#include "CglFont.h"
#include "CglMaterial.h"
#include "CglPicLoad.h"
#include "CglShape.h"
#include "CglMath.h"
#include "CglParticle.h"
#include "CglParticleCtrl.h"

class AFX_API_EXPORT COpenGL
{
public:
	COpenGL();
	~COpenGL();

	virtual void InDraw();
	void OnView();
	bool PreTranslateMessage(unsigned int message, unsigned int wParam, unsigned int lParam);
	void OnSize(int w, int h);
	void OnPaint();
	virtual void PostInit();
	bool Init(HWND hWnd);
	virtual bool OnKey(unsigned int nChar, bool bDown);
	virtual bool OnMouse(int message, CPoint pt);
	friend class CglControl;

protected:
	HDC m_hDC;
	HGLRC m_hglrc;
	float m_PerspectiveParam[4];
	CglControl *m_pControl;
	HWND m_hWnd;
	CglCamera *m_pCamere;
	CglFont *m_pFont;
	CglShape *m_pShape;
	CglPicLoad *m_pPicLoad;
	float m_fps;
};

