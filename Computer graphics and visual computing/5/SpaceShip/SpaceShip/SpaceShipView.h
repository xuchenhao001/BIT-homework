
// SpaceShipView.h : CSpaceShipView ��Ľӿ�
//

#pragma once

#include"CmyOpenGL.h"

class CSpaceShipView : public CView
{
protected: // �������л�����
	CSpaceShipView();
	DECLARE_DYNCREATE(CSpaceShipView)

// ����
public:
	CSpaceShipDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CSpaceShipView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	CmyOpenGL m_gl;

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // SpaceShipView.cpp �еĵ��԰汾
inline CSpaceShipDoc* CSpaceShipView::GetDocument() const
   { return reinterpret_cast<CSpaceShipDoc*>(m_pDocument); }
#endif

