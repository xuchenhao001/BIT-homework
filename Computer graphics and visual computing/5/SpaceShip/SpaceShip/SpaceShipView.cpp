
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
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSpaceShipView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CSpaceShipView ����/����

CSpaceShipView::CSpaceShipView()
{
	// TODO: �ڴ˴���ӹ������

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

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CSpaceShipView ��ӡ


void CSpaceShipView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CSpaceShipView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CSpaceShipView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CSpaceShipView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
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


// CSpaceShipView ��Ϣ�������
