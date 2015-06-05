// �� MFC ���� �ҽ� �ڵ�� MFC Microsoft Office Fluent ����� �������̽�("Fluent UI")�� 
// ����ϴ� ����� ���� �ָ�, MFC C++ ���̺귯�� ����Ʈ��� ���Ե� 
// Microsoft Foundation Classes Reference �� ���� ���� ������ ���� 
// �߰������� �����Ǵ� �����Դϴ�.  
// Fluent UI�� ����, ��� �Ǵ� �����ϴ� �� ���� ��� ����� ������ �����˴ϴ�.  
// Fluent UI ���̼��� ���α׷��� ���� �ڼ��� ������ 
// http://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// Graphic_EditView.cpp : CGraphic_EditView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "Graphic_Edit.h"
#endif

#include "Graphic_EditDoc.h"
#include "Graphic_EditView.h"
#include "GDIPLUS.h"

using namespace Gdiplus;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGraphic_EditView

IMPLEMENT_DYNCREATE(CGraphic_EditView, CView)

BEGIN_MESSAGE_MAP(CGraphic_EditView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_SELECTED, &CGraphic_EditView::OnSelected)
	ON_COMMAND(ID_LINE, &CGraphic_EditView::OnLine)
	ON_COMMAND(ID_POLYLINE, &CGraphic_EditView::OnPolyline)
	ON_COMMAND(ID_ELLIPSE, &CGraphic_EditView::OnEllipse)
	ON_COMMAND(ID_RECTANGLE, &CGraphic_EditView::OnRectangle)
	ON_COMMAND(ID_TEXT, &CGraphic_EditView::OnText)
END_MESSAGE_MAP()

// CGraphic_EditView ����/�Ҹ�

CGraphic_EditView::CGraphic_EditView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	
	m_current_type = SELECTED;

	//�޴� ���� �� �ƴ��� �ٲ��ֱ� ���ؼ�..
	m_Menu_SELECT = TRUE;
	m_Menu_LINE = FALSE;
	m_Menu_POLYLINE = FALSE;
	m_Menu_ELLIPSE = FALSE;
	m_Menu_RECTANGLE = FALSE;
	m_Menu_TEXT = FALSE;

}

CGraphic_EditView::~CGraphic_EditView()
{
}

BOOL CGraphic_EditView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CGraphic_EditView �׸���

void CGraphic_EditView::OnDraw(CDC* pDC)
{
	CGraphic_EditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	CRect m_rect;
	GetClientRect(m_rect);

	CBitmap bmp;
	bmp.CreateBitmap(m_rect.Width(), m_rect.Height(), 1, 0x20, NULL);
	CDC memdc;
	memdc.CreateCompatibleDC(pDC);
	memdc.SelectObject(bmp);

	memdc.SelectStockObject(NULL_PEN);
	memdc.Rectangle(m_rect);
	memdc.SelectStockObject(BLACK_PEN);

	Draw(&memdc);

	pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(), &memdc, 0, 0, SRCCOPY);
	bmp.DeleteObject();
	memdc.DeleteDC();
}

void CGraphic_EditView::Draw(CDC* pDC)
{
	Graphics graphics(*pDC);

	CGraphic_EditDoc* doc = (CGraphic_EditDoc*)GetDocument();

	POSITION pos = doc->gobj_list.GetHeadPosition();
	while (pos)
	{
		GObject* gobj = (GObject*)doc->gobj_list.GetNext(pos);

		CPen pen(PS_SOLID, gobj->m_Bold, gobj->m_LineColor);
		CPen *oldpen = pDC->SelectObject(&pen);

		gobj->draw(pDC);


		pDC->SelectObject(&oldpen);
	}

	//���� �׸��� �ִ� ��ü ȭ�� ���
	if (doc->m_bDrawing == TRUE)
	{
		GObject* curr_obj = (GObject*)doc->current_gobj.GetHead();
		curr_obj->setPoint(doc->m_EPoint.x, doc->m_EPoint.y);
		curr_obj->draw(pDC);
	}

	if (doc->m_GroupIDUsed > 0)
	{
		CRect* rgb;

		for (int i = 0; i < doc->m_GroupIDUsed; i++)
		{
			//rgb = doc->m_groupSet.getRect(doc->m_GroupID[i]);

			CPen pen(PS_DOT, 1, BLACK_PEN);
			pDC->SelectObject(pen);
			pDC->SelectStockObject(NULL_BRUSH);

			pDC->Rectangle(rgb);
		}
	}

	if (doc->m_GroupIDUsed == 1)
	{
		//GGroup* group = doc->m_groupSet.getGroup(doc->m_GroupID[0]);

		//group->DrawPoint(pDC);
	}

	if (doc->current_gobj.GetCount() == 1)
	{
		GObject* gobj = (GObject*)doc->current_gobj.GetHead();

		gobj->DrawPoint(pDC);

	}


}

void CGraphic_EditView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CGraphic_EditView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CGraphic_EditView ����

#ifdef _DEBUG
void CGraphic_EditView::AssertValid() const
{
	CView::AssertValid();
}

void CGraphic_EditView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGraphic_EditDoc* CGraphic_EditView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGraphic_EditDoc)));
	return (CGraphic_EditDoc*)m_pDocument;
}
#endif //_DEBUG


// CGraphic_EditView �޽��� ó����


void CGraphic_EditView::OnSelected()
{
	// TODO: Add your command handler code here
	m_IsSelect = TRUE;
	m_IsLine = FALSE;
	m_IsPolyline = FALSE;
	m_IsEllipse = FALSE;
	m_IsRectangle = FALSE;
	m_IsText = FALSE;

	m_current_type = SELECTED;
}


void CGraphic_EditView::OnLine()
{
	// TODO: Add your command handler code here
	m_IsSelect = FALSE;
	m_IsLine = TRUE;
	m_IsPolyline = FALSE;
	m_IsEllipse = FALSE;
	m_IsRectangle = FALSE;
	m_IsText = FALSE;

	m_current_type = LINE;
}


void CGraphic_EditView::OnPolyline()
{
	m_IsSelect = FALSE;
	m_IsLine = FALSE;
	m_IsPolyline = TRUE;
	m_IsEllipse = FALSE;
	m_IsRectangle = FALSE;
	m_IsText = FALSE;

	m_current_type = POLYLINE;
}


void CGraphic_EditView::OnEllipse()
{
	m_IsSelect = FALSE;
	m_IsLine = FALSE;
	m_IsPolyline = FALSE;
	m_IsEllipse = TRUE;
	m_IsRectangle = FALSE;
	m_IsText = FALSE;

	m_current_type = ELLIPSE;
}


void CGraphic_EditView::OnRectangle()
{
	m_IsSelect = FALSE;
	m_IsLine =FALSE;
	m_IsPolyline = FALSE;
	m_IsEllipse = FALSE;
	m_IsRectangle =TRUE;
	m_IsText = FALSE;

	m_current_type = RECTANGLE;
}


void CGraphic_EditView::OnText()
{
	m_IsSelect = FALSE;
	m_IsLine = FALSE;
	m_IsPolyline = FALSE;
	m_IsEllipse = FALSE;
	m_IsRectangle = FALSE;
	m_IsText = TRUE;

	m_current_type = TEXT;
}
