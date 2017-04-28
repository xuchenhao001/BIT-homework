#pragma once
#include "opengl/OpenGL.h"
#pragma comment(lib,"opengl/MyOpenGL")

class CmyOpenGL :public COpenGL {
public:
	CmyOpenGL();
	~CmyOpenGL();

	virtual void PostInit();
	virtual void InDraw();
	bool m_bWired;

private:
	virtual bool OnKey(unsigned char nChar, bool bDown);
	virtual void DrawModel();
};

