
// SpaceShipDoc.cpp : CSpaceShipDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "SpaceShip.h"
#endif

#include "SpaceShipDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSpaceShipDoc

IMPLEMENT_DYNCREATE(CSpaceShipDoc, CDocument)

BEGIN_MESSAGE_MAP(CSpaceShipDoc, CDocument)
END_MESSAGE_MAP()


// CSpaceShipDoc ����/����

CSpaceShipDoc::CSpaceShipDoc() {
	// TODO: �ڴ����һ���Թ������
}

CSpaceShipDoc::~CSpaceShipDoc() {
}

BOOL CSpaceShipDoc::OnNewDocument() {
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CSpaceShipDoc ���л�

void CSpaceShipDoc::Serialize(CArchive& ar) {
	if (ar.IsStoring()) {
		// TODO: �ڴ���Ӵ洢����
	} else {
		// TODO: �ڴ���Ӽ��ش���
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CSpaceShipDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds) {
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void CSpaceShipDoc::InitializeSearchContent() {
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:     strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CSpaceShipDoc::SetSearchContent(const CString& value) {
	if (value.IsEmpty()) {
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	} else {
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL) {
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CSpaceShipDoc ���

#ifdef _DEBUG
void CSpaceShipDoc::AssertValid() const {
	CDocument::AssertValid();
}

void CSpaceShipDoc::Dump(CDumpContext& dc) const {
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSpaceShipDoc ����
