
// SpaceShipView.cpp : CSpaceShipView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CSpaceShipView ����/����

CSpaceShipView::CSpaceShipView()
{
	// TODO: �ڴ˴����ӹ������

}

CSpaceShipView::~CSpaceShipView()
{
}

BOOL CSpaceShipView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CSpaceShipView ����

void CSpaceShipView::OnDraw(CDC* /*pDC*/)
{
	CSpaceShipDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ�����������ӻ��ƴ���
	m_gl.OnPaint();	//OpenGL�Ļ��ơ�
}


// CSpaceShipView ��ӡ

BOOL CSpaceShipView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CSpaceShipView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ���Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CSpaceShipView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ���Ӵ�ӡ����е���������
}


// CSpaceShipView ���

#ifdef _DEBUG
void CSpaceShipView::AssertValid() const
{
	CView::AssertValid();
}

void CSpaceShipView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSpaceShipDoc* CSpaceShipView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSpaceShipDoc)));
	return (CSpaceShipDoc*)m_pDocument;
}
#endif //_DEBUG


// CSpaceShipView ��Ϣ��������


void CSpaceShipView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	m_gl.OnSize(cx, cy);//�����ڴ�С����ʱ�����OpenGL����ͼ
	// TODO: �ڴ˴�������Ϣ�����������
}


void CSpaceShipView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	m_gl.OnPaint();
	CView::OnTimer(nIDEvent);
}


BOOL CSpaceShipView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	return true;	//ֱ�ӷ���true����ֹwindows������Ļ��ɫ�����ƹ�����ȫ����OpenGL
	//return CView::OnEraseBkgnd(pDC);
}


BOOL CSpaceShipView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ�����ר�ô����/����û���
	//ֱ�Ӱ�������������Ϣ�����͸�OpenGL������
	m_gl.PreTranslateMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
	return CView::PreTranslateMessage(pMsg);
}


void CSpaceShipView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: �ڴ�����ר�ô����/����û���
	if(!m_gl.Init(GetSafeHwnd()))
		AfxMessageBox("error!");
	SetTimer(1, 30, NULL);
}