#pragma once

#include "gl\glut.h"

class AFX_API_EXPORT CglMaterial {
public:
	CglMaterial();
	~CglMaterial();

	static GLuint LoadGLTextures(char *filename);
	static GLuint LoadGLTransparentTextures(char *filename);	//颜色越深透明度越高
};

