
// SpaceShipView.cpp : CSpaceShipView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "SpaceShip.h"
#endif

#include "SpaceShipDoc.h"
#include "SpaceShipView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSpaceShipView

IMPLEMENT_DYNCREATE(CSpaceShipView, CView)

BEGIN_MESSAGE_MAP(CSpaceShipView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CSpaceShipView 构造/析构

CSpaceShipView::CSpaceShipView() {
	// TODO: 在此处添加构造代码

}

CSpaceShipView::~CSpaceShipView() {
}

BOOL CSpaceShipView::PreCreateWindow(CREATESTRUCT& cs) {
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CSpaceShipView 绘制

void CSpaceShipView::OnDraw(CDC* /*pDC*/) {
	CSpaceShipDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	m_gl.OnPaint();	//OpenGL的绘制。
}


// CSpaceShipView 打印

BOOL CSpaceShipView::OnPreparePrinting(CPrintInfo* pInfo) {
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CSpaceShipView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/) {
	// TODO: 添加额外的打印前进行的初始化过程
}

void CSpaceShipView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/) {
	// TODO: 添加打印后进行的清理过程
}


// CSpaceShipView 诊断

#ifdef _DEBUG
void CSpaceShipView::AssertValid() const {
	CView::AssertValid();
}

void CSpaceShipView::Dump(CDumpContext& dc) const {
	CView::Dump(dc);
}

// 非调试版本是内联的
CSpaceShipDoc* CSpaceShipView::GetDocument() const {
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSpaceShipDoc)));
	return (CSpaceShipDoc*)m_pDocument;
}
#endif //_DEBUG


// CSpaceShipView 消息处理程序


void CSpaceShipView::OnSize(UINT nType, int cx, int cy) {
	CView::OnSize(nType, cx, cy);
	m_gl.OnSize(cx, cy);//当窗口大小调整时候调整OpenGL的视图
						// TODO: 在此处添加消息处理程序代码
}


void CSpaceShipView::OnTimer(UINT_PTR nIDEvent) {
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_gl.OnPaint();
	CView::OnTimer(nIDEvent);
}


BOOL CSpaceShipView::OnEraseBkgnd(CDC* pDC) {
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return true;	//直接返回true，禁止windows绘制屏幕底色。绘制工作完全交给OpenGL
					//return CView::OnEraseBkgnd(pDC);
}


BOOL CSpaceShipView::PreTranslateMessage(MSG* pMsg) {
	// TODO: 在此添加专用代码和/或调用基类
	//直接把所有鼠标键盘消息都发送给OpenGL处理。
	m_gl.PreTranslateMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
	return CView::PreTranslateMessage(pMsg);
}


void CSpaceShipView::OnInitialUpdate() {
	CView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	if (!m_gl.Init(GetSafeHwnd()))
		AfxMessageBox("error!");
	SetTimer(1, 30, NULL);
}
