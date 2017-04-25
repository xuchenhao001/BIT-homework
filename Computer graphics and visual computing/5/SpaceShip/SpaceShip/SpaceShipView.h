
// SpaceShipView.h : CSpaceShipView 类的接口
//

#pragma once

#include "myOpenGL.h"
class CSpaceShipView : public CView
{
	CmyOpenGL m_gl;
protected: // 仅从序列化创建
	CSpaceShipView();
	DECLARE_DYNCREATE(CSpaceShipView)

// 特性
public:
	CSpaceShipDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnInitialUpdate();
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CSpaceShipView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // SpaceShipView.cpp 中的调试版本
inline CSpaceShipDoc* CSpaceShipView::GetDocument() const
   { return reinterpret_cast<CSpaceShipDoc*>(m_pDocument); }
#endif

