
// SpaceShip.h : SpaceShip Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CSpaceShipApp:
// �йش����ʵ�֣������ SpaceShip.cpp
//

class CSpaceShipApp : public CWinAppEx {
public:
	CSpaceShipApp();


	// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSpaceShipApp theApp;