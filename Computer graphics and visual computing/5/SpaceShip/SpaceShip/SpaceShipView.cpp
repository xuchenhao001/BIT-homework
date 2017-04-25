
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
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSpaceShipView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CSpaceShipView 构造/析构

CSpaceShipView::CSpaceShipView()
{
	// TODO: 在此处添加构造代码

}

CSpaceShipView::~CSpaceShipView()
{
}

BOOL CSpaceShipView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CSpaceShipView 绘制

void CSpaceShipView::OnDraw(CDC* /*pDC*/)
{
	CSpaceShipDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	m_gl.OnPaint();	//OpenGL的绘制。
	// TODO: 在此处为本机数据添加绘制代码
}


// CSpaceShipView 打印


void CSpaceShipView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CSpaceShipView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CSpaceShipView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CSpaceShipView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CSpaceShipView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSpaceShipView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CSpaceShipView 诊断

#ifdef _DEBUG
void CSpaceShipView::AssertValid() const
{
	CView::AssertValid();
}

void CSpaceShipView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSpaceShipDoc* CSpaceShipView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSpaceShipDoc)));
	return (CSpaceShipDoc*)m_pDocument;
}
#endif //_DEBUG


// CSpaceShipView 消息处理程序
BOOL CSpaceShipView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	//直接把所有鼠标键盘消息都发送给OpenGL处理。
	m_gl.PreTranslateMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
	return CView::PreTranslateMessage(pMsg);
}

void CSpaceShipView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	if (!m_gl.Init(this->GetSafeHwnd()))	//windows窗口生成完毕后，此时初始化OpenGL,把窗口绘制权利交给OpenGL
		AfxMessageBox("error!");
	SetTimer(1, 30, NULL);
	// TODO: Add your specialized code here and/or call the base class

}

BOOL CSpaceShipView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return true;	//直接返回true，禁止windows绘制屏幕底色。绘制工作完全交给OpenGL
	return CView::OnEraseBkgnd(pDC);
}

void CSpaceShipView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	m_gl.OnSize(cx, cy);	//当窗口大小调整时候调整OpenGL的视图
							// TODO: Add your message handler code here
}

void CSpaceShipView::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	m_gl.OnPaint();
	CView::OnTimer(nIDEvent);
}
