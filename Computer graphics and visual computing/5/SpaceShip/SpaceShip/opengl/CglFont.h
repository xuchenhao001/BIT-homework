#pragma once

#include "gl\glut.h"
#include <afxtempl.h>

class AFX_API_EXPORT CglFont
{
public:
	CglFont();
	~CglFont();

	void Font2D(char * str, double x, double y, int type);
	bool Font3D(CString str);
	bool Font3D(CString str, int &id);

private:
	LOGFONT m_logFontDefault;
	CFont* m_pFontSelected;
	CArray<int, int>m_fontlist;//保存所有的已经生成的三维字体列表

	bool Create3DFont();
	int m_iFormat;
	double m_fDeviation;
	double m_fExtrusion;
	int m_iFormatDefault;
	double m_fDeviationDefault;
	double m_fExtrusionDefault;
	int m_iFirstChar;
	int m_iLastChar;
	int m_iNumberChars;
	int m_iDisplayListStart;
	int m_iDisplayListBase;
	bool m_bHaveCreate3DFont;
};

