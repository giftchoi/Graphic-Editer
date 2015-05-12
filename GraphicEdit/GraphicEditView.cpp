
// GraphicEditView.cpp : CGraphicEditView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "GraphicEdit.h"
#endif

#include "GraphicEditDoc.h"
#include "GraphicEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGraphicEditView

IMPLEMENT_DYNCREATE(CGraphicEditView, CView)

BEGIN_MESSAGE_MAP(CGraphicEditView, CView)
END_MESSAGE_MAP()

// CGraphicEditView ����/�Ҹ�

CGraphicEditView::CGraphicEditView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CGraphicEditView::~CGraphicEditView()
{
}

BOOL CGraphicEditView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CGraphicEditView �׸���

void CGraphicEditView::OnDraw(CDC* /*pDC*/)
{
	CGraphicEditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CGraphicEditView ����

#ifdef _DEBUG
void CGraphicEditView::AssertValid() const
{
	CView::AssertValid();
}

void CGraphicEditView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGraphicEditDoc* CGraphicEditView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGraphicEditDoc)));
	return (CGraphicEditDoc*)m_pDocument;
}
#endif //_DEBUG


// CGraphicEditView �޽��� ó����
