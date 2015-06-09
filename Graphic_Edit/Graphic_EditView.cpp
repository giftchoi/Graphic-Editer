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
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_LINECOLOR, &CGraphic_EditView::OnLinecolor)
	ON_COMMAND(ID_RGNCOLOR, &CGraphic_EditView::OnRgncolor)
//	ON_COMMAND(ID_REGIONPATTERN, &CGraphic_EditView::OnRegionpattern)
//	ON_COMMAND(ID_LINEPATTERN, &CGraphic_EditView::OnLinepattern)
	ON_UPDATE_COMMAND_UI(ID_SELECTED, &CGraphic_EditView::OnUpdateSelected)
	ON_UPDATE_COMMAND_UI(ID_LINE, &CGraphic_EditView::OnUpdateLine)
	ON_UPDATE_COMMAND_UI(ID_POLYLINE, &CGraphic_EditView::OnUpdatePolyline)
	ON_UPDATE_COMMAND_UI(ID_ELLIPSE, &CGraphic_EditView::OnUpdateEllipse)
	ON_UPDATE_COMMAND_UI(ID_RECTANGLE, &CGraphic_EditView::OnUpdateRectangle)
	ON_UPDATE_COMMAND_UI(ID_TEXT, &CGraphic_EditView::OnUpdateText)
	ON_COMMAND(ID_POINTDELETE, &CGraphic_EditView::OnPointdelete)
	ON_UPDATE_COMMAND_UI(ID_POINTDELETE, &CGraphic_EditView::OnUpdatePointdelete)
	ON_COMMAND(ID_DELETE, &CGraphic_EditView::OnDelete)
	ON_UPDATE_COMMAND_UI(ID_DELETE, &CGraphic_EditView::OnUpdateDelete)
	ON_COMMAND(ID_EDIT_CUT, &CGraphic_EditView::OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, &CGraphic_EditView::OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CGraphic_EditView::OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, &CGraphic_EditView::OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CGraphic_EditView::OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CGraphic_EditView::OnUpdateEditPaste)
END_MESSAGE_MAP()

// CGraphic_EditView ����/�Ҹ�

CGraphic_EditView::CGraphic_EditView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	
	m_current_type = SELECTED;

	//�޴� ���� �� �ƴ��� �ٲ��ֱ� ���ؼ�..
	m_IsSelect = TRUE;
	m_IsLine = FALSE;
	m_IsPolyline = FALSE;
	m_IsEllipse = FALSE;
	m_IsRectangle = FALSE;
	m_IsText = FALSE;

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
		curr_obj->setPoint(doc->m_EndPoint.x, doc->m_EndPoint.y);
		curr_obj->draw(pDC);
	}

	if (doc->m_GroupIDUsed > 0)
	{
		CRect* rgb;

		for (int i = 0; i < doc->m_GroupIDUsed; i++)
		{
			rgb = doc->m_groupSet.getRect(doc->m_GroupID[i]);

			CPen pen(PS_DOT, 1, BLACK_PEN);
			pDC->SelectObject(pen);
			pDC->SelectStockObject(NULL_BRUSH);

			pDC->Rectangle(rgb);
		}
	}

	if (doc->m_GroupIDUsed == 1)
	{
		Group* group = doc->m_groupSet.getGroup(doc->m_GroupID[0]);

		group->DrawPoint(pDC);
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

	CGraphic_EditDoc* doc = (CGraphic_EditDoc*)GetDocument();

	CPoint point;
	point.x = -9999;
	point.y = -9999;
	doc->clearSelect(point);

	Invalidate(FALSE);
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

	CGraphic_EditDoc* doc = (CGraphic_EditDoc*)GetDocument();

	CPoint point;
	point.x = -9999;
	point.y = -9999;
	doc->clearSelect(point);

	Invalidate(FALSE);
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

	CGraphic_EditDoc* doc = (CGraphic_EditDoc*)GetDocument();

	CPoint point;
	point.x = -9999;
	point.y = -9999;
	doc->clearSelect(point);

	Invalidate(FALSE);
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

	CGraphic_EditDoc* doc = (CGraphic_EditDoc*)GetDocument();

	CPoint point;
	point.x = -9999;
	point.y = -9999;
	doc->clearSelect(point);

	Invalidate(FALSE);
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

	CGraphic_EditDoc* doc = (CGraphic_EditDoc*)GetDocument();

	CPoint point;
	point.x = -9999;
	point.y = -9999;
	doc->clearSelect(point);

	Invalidate(FALSE);
}


void CGraphic_EditView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CGraphic_EditDoc* doc = (CGraphic_EditDoc*)GetDocument();

	//�ؽ�Ʈ ���� ó��
	if (doc->current_gobj.GetCount() == 1)
	{
		TextG* text = (TextG*)doc->current_gobj.GetHead();
		if (text->getObjectType() == TEXT)
		{
			if (doc->m_bDrawing)
			{
				m_IsSelect = TRUE;
				m_IsLine = FALSE;
				m_IsPolyline = FALSE;
				m_IsEllipse = FALSE;
				m_IsRectangle = FALSE;
				m_IsText = FALSE;

				m_current_type = SELECTED;
				doc->m_bDrawing = FALSE;

				doc->gobj_list.AddTail(text);
				return;
			}
		}
	}

	doc->m_StartPoint = point;
	doc->m_EndPoint = point;
	doc->m_current_type = m_current_type;
	doc->m_bSelect_Object = FALSE;
	doc->m_bDrawing = FALSE;

	//���� ������ �ƴϰ� ���� ����Ʈ�� ��ü�� ������ ������ ��ü����Ʈ�� �����ϰ� ���
	if ((!(nFlags & MK_SHIFT) || doc->m_current_type != SELECTED) && !doc->m_bPolyDraw)
	{
		doc->clearSelect(point);
	}

	switch (doc->m_current_type)
	{
	case SELECTED:
	{
				   //����Ʈ ��ü�� ���鼭 ���� ��ü�� ���õǾ����� ���� ���ø���Ʈ�� �־� ��
				   POSITION pos = doc->gobj_list.GetTailPosition();

				   while (pos)
				   {
					   GObject* gobj = (GObject*)doc->gobj_list.GetPrev(pos);
					   BOOL isDuplication = FALSE;

					   if (gobj->pointInRgn(point))			//���� ����Ʈ�� ������
					   {
						   POSITION currListPos = doc->current_gobj.GetHeadPosition();

						   while (currListPos)			//������ ����Ʈ�� �ִ��� �˻��� �� ������ isDuplication TRUE;
						   {
							   GObject* currlistObj = (GObject*)doc->current_gobj.GetNext(currListPos);
							   if (currlistObj == gobj)
							   {
								   isDuplication = TRUE;
								   break;
							   }
						   }

						   if (!isDuplication)
						   {
							   doc->current_gobj.AddHead(gobj);
							   if (gobj->getID() == -1)
							   {
								   gobj->setSelected(TRUE);
							   }

							   doc->m_bSelect_Object = TRUE;
							   break;
						   }
						   else
						   {
							   break;
						   }
					   }
				   }

				   doc->m_GroupIDUsed = 0;
				   doc->m_groupSet.getGroupsID(&doc->current_gobj, doc->m_GroupID, &doc->m_GroupIDUsed);

				   for (int i = 0; i < doc->m_GroupIDUsed; i++)
				   {
					   CPtrList* group_gobj = doc->m_groupSet.getGroupItemList(doc->m_GroupID[i]);
					   doc->SetCurItemAdd(group_gobj);
				   }

				   if (doc->m_GroupIDUsed == 1)
				   {
					   Group* group = doc->m_groupSet.getGroup(doc->m_GroupID[0]);
					   group->selectPoint(point);
				   }

				   if (doc->current_gobj.GetCount() == 1)
				   {
					   GObject* gobj = (GObject*)doc->current_gobj.GetHead();
					   gobj->selectPoint(point);
				   }

				   break;

	}
	case RECTANGLE:
	{
					  RectangleG* g = new RectangleG();
					  g->setColor(doc->m_LineColor);
					  g->setRgnColor(doc->m_rgncolor);
					  g->setPoint(point.x, point.y, point.x, point.y);
					  g->setLinePattern(doc->m_Line_Pattern);
					  g->setRgnPattern(doc->m_rgnpattern);
					  g->setThickness(doc->m_Bold);
					  g->setAlpha(doc->m_alpha);

					  doc->m_bDrawing = TRUE;

					  doc->current_gobj.RemoveAll();
					  doc->current_gobj.AddTail(g);
					  break;
	}
	case ELLIPSE:
	{
					EllipseG* g = new EllipseG();
				   g->setColor(doc->m_LineColor);
				   g->setRgncolor(doc->m_rgncolor);
				   g->setPoint(point.x, point.y, point.x, point.y);
				   g->setLinePattern(doc->m_Line_Pattern);
				   g->setRgnPattern(doc->m_rgnpattern);
				   g->setThickness(doc->m_Bold);
				   g->setAlpha(doc->m_alpha);

				   doc->m_bDrawing = TRUE;

				   doc->current_gobj.RemoveAll();
				   doc->current_gobj.AddTail(g);
				   break;
	}
	case LINE:
	{
				 LineG* g = new LineG();
				 g->setColor(doc->m_LineColor);
				 g->setPoint(point.x, point.y, point.x, point.y);
				 g->setLinePattern(doc->m_Line_Pattern);
				 g->setThickness(doc->m_Bold);
				 g->setAlpha(doc->m_alpha);

				 doc->m_bDrawing = TRUE;

				 doc->current_gobj.RemoveAll();
				 doc->current_gobj.AddTail(g);
				 break;
	}
	case POLYLINE:
	{
					 if (!doc->m_bPolyDraw)
					 {
						 PolylineG* g = new PolylineG();
						 g->setColor(doc->m_LineColor);
						 g->setPoint(point.x, point.y, point.x, point.y);
						 g->setLinePattern(doc->m_Line_Pattern);
						 g->addTail(point);
						 g->setThickness(doc->m_Bold);
						 g->setAlpha(doc->m_alpha);

						 doc->current_gobj.RemoveAll();
						 doc->current_gobj.AddTail(g);
						 doc->m_bDrawing = TRUE;
						 doc->m_bPolyDraw = TRUE;
					 }
					 else
					 {
						 PolylineG* GPoly = (PolylineG*)doc->current_gobj.GetHead();
						 GPoly->setPoint(point.x, point.y, point.x, point.y);
						 doc->m_bDrawing = TRUE;
						 GPoly->addTail(point);
					 }
					 break;
	}
	case TEXT:
	{
				 doc->m_str.RemoveAll();
				 TextG* g = new TextG();
				 g->setColor(doc->m_LineColor);
				 g->setPoint(point.x, point.y, point.x, point.y);
				 g->setRgnColor(doc->m_rgncolor);
				 g->setRgnPattern(doc->m_rgnpattern);
				 g->setThickness(doc->m_Bold);
				 g->setSelected(TRUE);
				 g->setAlpha(doc->m_alpha);
				 g->setFont(doc->m_lf);

				 doc->current_gobj.RemoveAll();
				 doc->current_gobj.AddTail(g);
				 doc->m_bDrawing = TRUE;
				 break;
	}
	}


	CView::OnLButtonDown(nFlags, point);
}


void CGraphic_EditView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CGraphic_EditDoc* doc = (CGraphic_EditDoc*)GetDocument();

	if (doc->current_gobj.IsEmpty())
	{
		return;
	}

	GObject* tempObj = (GObject*)doc->current_gobj.GetHead();

	if (!(nFlags & MK_LBUTTON) && !doc->m_bPolyDraw)
	{
		return;
	}


	switch (doc->m_current_type)
	{
	case SELECTED:
	{
				   Group* group = NULL;

				   if (doc->m_GroupIDUsed == 1)
				   {
					   group = doc->m_groupSet.getGroup(doc->m_GroupID[0]);
				   }

				   if (group != NULL && group->getSelectPoint() != -1)
				   {
					   CPtrList* ObjList = group->getItemList();

					   CPoint tmpPoint = point - doc->m_EndPoint;

					   POSITION pos = ObjList->GetHeadPosition();

					   while (pos)
					   {
						   GObject* gobj = (GObject*)ObjList->GetNext(pos);
						   if (gobj->getObjectType() == RECTANGLE || gobj->getObjectType() == ELLIPSE)
						   {
							   gobj->setSelected(group->getSelectPoint());
							   gobj->move(tmpPoint.x, tmpPoint.y);
							   gobj->SetRgn();
						   }
					   }

					   doc->m_EndPoint = point;
				   }
				   else
				   {
					   CPoint tmpPoint = point - doc->m_EndPoint;
					   doc->m_EndPoint = point;

					   POSITION pos = doc->current_gobj.GetHeadPosition();

					   while (pos)
					   {
						   GObject* gobj = (GObject*)doc->current_gobj.GetNext(pos);
						   gobj->move(tmpPoint.x, tmpPoint.y);
						   gobj->SetRgn();
					   }
				   }

				   for (int i = 0; i < doc->m_GroupIDUsed; i++)
				   {
					   doc->m_groupSet.setRectAndRgn(doc->m_GroupID[i]);
				   }
				   break;
	}
	case POLYLINE:
	{
					 doc->m_EndPoint = point;
					 break;
	}
	case TEXT:
	{
				 break;
	}
	default:
	{
			   doc->m_EndPoint = point;
			   break;
	}
	}

	Invalidate(FALSE);
	
	CView::OnMouseMove(nFlags, point);
}


void CGraphic_EditView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CGraphic_EditDoc* doc = (CGraphic_EditDoc*)GetDocument();

	if (!doc->current_gobj.IsEmpty())
	{
		switch (doc->m_current_type)
		{
		case SELECTED:
		{
					   break;
		}
		case POLYLINE:
		{
						 break;
		}
		case TEXT:
		{
					 break;
		}
		default:
		{
				   POSITION pos = doc->gobj_list.GetHeadPosition();
				   while (pos)
				   {
					   GObject* curr_gobj = (GObject*)doc->current_gobj.GetNext(pos);
					   curr_gobj->setSelected(FALSE);
				   }

				   GObject* curr_gobj = (GObject*)doc->current_gobj.GetHead();

				   curr_gobj->SetRgn();
				   curr_gobj->setSelected(TRUE);

				   doc->gobj_list.AddTail(curr_gobj);
				   doc->current_gobj.RemoveAll();
				   doc->current_gobj.AddTail(curr_gobj);
				   doc->m_bDrawing = FALSE;


				   m_IsSelect = TRUE;
				   m_IsLine = FALSE;
				   m_IsPolyline = FALSE;
				   m_IsEllipse = FALSE;
				   m_IsRectangle = FALSE;
				   m_IsText = FALSE;

				   m_current_type = SELECTED;

				   ::ReleaseCapture();

				   break;
		}
		}
	}

	//�� �̵� Ǯ���ٷ���..
	if (doc->current_gobj.GetCount() > 1)
	{
		POSITION pos = doc->current_gobj.GetHeadPosition();

		for (int i = 0; i < doc->current_gobj.GetCount(); i++)
		{
			GObject* gobj = (GObject*)doc->current_gobj.GetNext(pos);
			gobj->selectPoint();
		}
	}


	if (doc->m_current_type != POLYLINE)
	{
		doc->SetModifiedFlag();
	}

	Invalidate(FALSE);


	CView::OnLButtonUp(nFlags, point);
}


void CGraphic_EditView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CGraphic_EditDoc* doc = (CGraphic_EditDoc*)GetDocument();

	if (doc->m_current_type == POLYLINE)
	{
		PolylineG* GPoly = (PolylineG*)doc->current_gobj.GetHead();
		//GPoly->addPoint(point);
		GPoly->setSelected(TRUE);
		GPoly->SetRgn();

		doc->current_gobj.RemoveAll();
		doc->current_gobj.AddTail(GPoly);

		doc->gobj_list.AddTail(GPoly);
		doc->m_bDrawing = FALSE;
		doc->m_bPolyDraw = FALSE;

		m_IsSelect = TRUE;
		m_IsLine = FALSE;
		m_IsPolyline = FALSE;
		m_IsEllipse = FALSE;
		m_IsRectangle = FALSE;
		m_IsText = FALSE;

		m_current_type = SELECTED;

		::ReleaseCapture();
		Invalidate(FALSE);

		doc->SetModifiedFlag();
	}


	CView::OnLButtonDblClk(nFlags, point);
}


void CGraphic_EditView::OnLinecolor()
{
	CGraphic_EditDoc* doc = (CGraphic_EditDoc*)GetDocument();

	CColorDialog dlgColor(doc->m_LineColor, CC_FULLOPEN);
	dlgColor.DoModal();
	doc->m_LineColor = dlgColor.GetColor();


	//���� ���õ� ������Ʈ�� ������ ��������.
	if (doc->current_gobj.GetCount() > 0)
	{
		POSITION pos = doc->current_gobj.GetHeadPosition();
		GObject* gobj;
		while (pos)
		{
			gobj = (GObject*)doc->current_gobj.GetNext(pos);			// element �����ͼ� 
			gobj->setColor(doc->m_LineColor);
		}

		Invalidate(FALSE);
	}
}


void CGraphic_EditView::OnRgncolor()
{
	CGraphic_EditDoc* doc = (CGraphic_EditDoc*)GetDocument();

	CColorDialog dlgColor(doc->m_rgncolor, CC_FULLOPEN);
	dlgColor.DoModal();
	doc->m_rgncolor = dlgColor.GetColor();

	//���� ���õ� ������Ʈ�� ������ ��������.
	if (doc->current_gobj.GetCount() > 0)
	{
		POSITION pos = doc->current_gobj.GetHeadPosition();
		GObject* gobj;
		while (pos)
		{
			gobj = (GObject*)doc->current_gobj.GetNext(pos);			// element �����ͼ�
			gobj->setRgnColor(doc->m_rgncolor);
		}
		Invalidate(FALSE);
	}
}

/*
void CGraphic_EditView::OnRegionpattern()
{
	CGraphic_EditDoc* doc = (CGraphic_EditDoc*)GetDocument();

	CMFCRibbonComboBox* pComboBox = (CMFCRibbonComboBox*)wndRibbonBar->FindByID(ID_REGIONPATTERN);
	// Get the selected index
	int	nCurSel = pComboBox->GetCurSel();

	doc->m_rgnpattern = nCurSel;
	
	CString item=pFontComboBox->GetItem(nCurSel);
	CString sMessage = _T("");
	sMesage.Format(_T("Current Selected Item is \"%s\"."),item);
	MessageBox(sMessage, _T("Combo Box Item"), MB_OK);
	*/
/*
	if (doc->current_gobj.GetCount() > 0)
	{
		POSITION pos = doc->current_gobj.GetHeadPosition();
		GObject* gobj;
		while (pos)
		{
			gobj = (GObject*)doc->current_gobj.GetNext(pos);			// element �����ͼ�
			gobj->setRgnPattern(doc->m_rgnpattern);
		}
		Invalidate(FALSE);
	}
}


void CGraphic_EditView::OnLinepattern()
{
	CGraphic_EditDoc* doc = (CGraphic_EditDoc*)GetDocument();

	CMFCRibbonComboBox* pComboBox = (CMFCRibbonComboBox*)wndRibbonBar->FindByID(ID_LINEPATTERN);

	// Get the selected index
	int	nCurSel = pComboBox->GetCurSel();

	doc->m_Line_Pattern = nCurSel;

	//���� ���õ� ������Ʈ�� ������ ��������.
	if (doc->current_gobj.GetCount() > 0)
	{
		POSITION pos = doc->current_gobj.GetHeadPosition();
		GObject* gobj;
		while (pos)
		{
			gobj = (GObject*)doc->current_gobj.GetNext(pos);			// element �����ͼ�
			gobj->setLinePattern(doc->m_Line_Pattern);
		}
		Invalidate(FALSE);
	}
}
*/

void CGraphic_EditView::OnUpdateSelected(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_IsSelect);
}


void CGraphic_EditView::OnUpdateLine(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_IsLine);
}


void CGraphic_EditView::OnUpdatePolyline(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_IsPolyline);
}


void CGraphic_EditView::OnUpdateEllipse(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_IsEllipse);
}


void CGraphic_EditView::OnUpdateRectangle(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_IsRectangle);
}


void CGraphic_EditView::OnUpdateText(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_IsText);
}


void CGraphic_EditView::OnPointdelete()
{
	CGraphic_EditDoc* doc = (CGraphic_EditDoc*)GetDocument();


	if (doc->current_gobj.GetCount() == 1)
	{
		PolylineG* gobj = (PolylineG*)doc->current_gobj.GetHead();
		gobj->deletePoint();
	}

	Invalidate(FALSE);
}


void CGraphic_EditView::OnUpdatePointdelete(CCmdUI *pCmdUI)
{
	CGraphic_EditDoc* doc = (CGraphic_EditDoc*)GetDocument();


	if (doc->current_gobj.GetCount() == 1)
	{
		GObject* gobj = (GObject*)doc->current_gobj.GetHead();
		if (gobj->getObjectType() == POLYLINE)
		{
			if (doc->current_gobj.GetCount() == 1)
			{
				PolylineG* poly = (PolylineG*)doc->current_gobj.GetHead();
				if (poly->isPointSelected())
				{
					pCmdUI->Enable(TRUE);
				}
				else
				{
					pCmdUI->Enable(FALSE);
				}
			}
			else
			{
				pCmdUI->Enable(FALSE);
			}
		}
		else
		{
			pCmdUI->Enable(FALSE);
		}
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}


void CGraphic_EditView::OnDelete()
{
	CGraphic_EditDoc* doc = (CGraphic_EditDoc*)GetDocument();

	//�׷쿡�� �ش� ������ �����ϰ� �ʿ䰡 ������ �׷��� �����ϴ� �۾��� ����.
//	doc->m_groupSet.GroupItemDelete(&doc->current_gobj);


	//���� ��ü ����Ʈ���� ��������� ��.
	GObject* currObj;
	GObject* allObj;

	POSITION currObjPos = doc->current_gobj.GetHeadPosition();

	while (currObjPos)
	{
		currObj = (GObject*)doc->current_gobj.GetNext(currObjPos);

		POSITION allObjPos = doc->gobj_list.GetHeadPosition();

		while (allObjPos)
		{
			POSITION nowPos = allObjPos;
			allObj = (GObject*)doc->gobj_list.GetNext(allObjPos);

			if (currObj == allObj)
			{
				doc->gobj_list.RemoveAt(nowPos);
				delete allObj;
				break;
			}
		}
	}

	doc->current_gobj.RemoveAll();
	doc->m_GroupIDUsed = 0;

	Invalidate(FALSE);
}


void CGraphic_EditView::OnUpdateDelete(CCmdUI *pCmdUI)
{
	CGraphic_EditDoc* doc = (CGraphic_EditDoc*)GetDocument();

	if (!doc->current_gobj.IsEmpty())
	{
		pCmdUI->Enable(true);
	}
	else
	{
		pCmdUI->Enable(false);
	}
}


void CGraphic_EditView::OnEditCut()
{
	CGraphic_EditDoc* doc = (CGraphic_EditDoc*)GetDocument();
	doc->setItemToBuffer();


//	doc->m_groupSet.GroupItemDelete(&doc->current_gobj);


	//���� ��ü ����Ʈ���� ��������� ��.
	GObject* currObj;
	GObject* allObj;

	POSITION currObjPos = doc->current_gobj.GetHeadPosition();

	while (currObjPos)
	{
		currObj = (GObject*)doc->current_gobj.GetNext(currObjPos);

		POSITION allObjPos = doc->gobj_list.GetHeadPosition();

		while (allObjPos)
		{
			POSITION nowPos = allObjPos;
			allObj = (GObject*)doc->gobj_list.GetNext(allObjPos);

			if (currObj == allObj)
			{
				doc->gobj_list.RemoveAt(nowPos);
				delete allObj;
				break;
			}
		}
	}

	doc->current_gobj.RemoveAll();
	doc->m_GroupIDUsed = 0;

	Invalidate(FALSE);
}


void CGraphic_EditView::OnEditCopy()
{
	CGraphic_EditDoc* doc = (CGraphic_EditDoc*)GetDocument();

	doc->setItemToBuffer();
}


void CGraphic_EditView::OnEditPaste()
{
	CGraphic_EditDoc* doc = (CGraphic_EditDoc*)GetDocument();

	if (doc->buffer_gobj.IsEmpty())
	{
		return;
	}

	CPoint point(-9999, -9999);
	doc->clearSelect(point);

	POSITION pos = doc->buffer_gobj.GetHeadPosition();

	while (pos)
	{
		GObject* gobj = (GObject*)doc->buffer_gobj.GetNext(pos);
		gobj->move(10, 10);

		switch (gobj->getObjectType())
		{
		case LINE:
		{
					 LineG* g = new LineG((LineG*)gobj);
					 g->SetRgn();
					 g->setSelected(TRUE);
					 doc->gobj_list.AddTail(g);
					 doc->current_gobj.AddTail(g);
					 break;
		}
		case RECTANGLE:
		{
						  RectangleG* g = new RectangleG((RectangleG*)gobj);
						  g->SetRgn();
						  g->setSelected(TRUE);
						  doc->gobj_list.AddTail(g);
						  doc->current_gobj.AddTail(g);
						  break;
		}
		case ELLIPSE:
		{
					   EllipseG* g = new EllipseG((EllipseG*)gobj);
					   g->SetRgn();
					   g->setSelected(TRUE);
					   doc->gobj_list.AddTail(g);
					   doc->current_gobj.AddTail(g);
					   break;
		}
		case POLYLINE:
		{
						 PolylineG* g = new PolylineG((PolylineG*)gobj);
						 g->SetRgn();
						 g->setSelected(TRUE);
						 doc->gobj_list.AddTail(g);
						 doc->current_gobj.AddTail(g);
						 break;
		}
		case TEXT:
		{
					 TextG* g = new TextG((TextG*)gobj);
					 g->SetRgn();
					 g->setSelected(TRUE);
					 doc->gobj_list.AddTail(g);
					 doc->current_gobj.AddTail(g);
					 break;
		}
		}
	}

	Invalidate(FALSE);
}


void CGraphic_EditView::OnUpdateEditCut(CCmdUI *pCmdUI)
{
	CGraphic_EditDoc* doc = (CGraphic_EditDoc*)GetDocument();

	if (!doc->current_gobj.IsEmpty())
	{
		pCmdUI->Enable(true);
	}
	else
	{
		pCmdUI->Enable(false);
	}
}


void CGraphic_EditView::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	CGraphic_EditDoc* doc = (CGraphic_EditDoc*)GetDocument();

	if (!doc->current_gobj.IsEmpty())
	{
		pCmdUI->Enable(true);
	}
	else
	{
		pCmdUI->Enable(false);
	}
}


void CGraphic_EditView::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	CGraphic_EditDoc* doc = (CGraphic_EditDoc*)GetDocument();

	if (!doc->buffer_gobj.IsEmpty())
	{
		pCmdUI->Enable(true);
	}
	else
	{
		pCmdUI->Enable(false);
	}
}
