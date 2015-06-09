// 이 MFC 샘플 소스 코드는 MFC Microsoft Office Fluent 사용자 인터페이스("Fluent UI")를 
// 사용하는 방법을 보여 주며, MFC C++ 라이브러리 소프트웨어에 포함된 
// Microsoft Foundation Classes Reference 및 관련 전자 문서에 대해 
// 추가적으로 제공되는 내용입니다.  
// Fluent UI를 복사, 사용 또는 배포하는 데 대한 사용 약관은 별도로 제공됩니다.  
// Fluent UI 라이선싱 프로그램에 대한 자세한 내용은 
// http://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// Graphic_EditView.cpp : CGraphic_EditView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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

// CGraphic_EditView 생성/소멸

CGraphic_EditView::CGraphic_EditView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	
	m_current_type = SELECTED;

	//메뉴 선택 됨 아님을 바꿔주기 위해서..
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
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CGraphic_EditView 그리기

void CGraphic_EditView::OnDraw(CDC* pDC)
{
	CGraphic_EditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
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

	//현재 그리고 있는 객체 화면 출력
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


// CGraphic_EditView 진단

#ifdef _DEBUG
void CGraphic_EditView::AssertValid() const
{
	CView::AssertValid();
}

void CGraphic_EditView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGraphic_EditDoc* CGraphic_EditView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGraphic_EditDoc)));
	return (CGraphic_EditDoc*)m_pDocument;
}
#endif //_DEBUG


// CGraphic_EditView 메시지 처리기


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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CGraphic_EditDoc* doc = (CGraphic_EditDoc*)GetDocument();

	//텍스트 저장 처리
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

	//다중 선택이 아니고 현재 포인트에 객체가 없으면 없으면 객체리스트를 정리하고 비움
	if ((!(nFlags & MK_SHIFT) || doc->m_current_type != SELECTED) && !doc->m_bPolyDraw)
	{
		doc->clearSelect(point);
	}

	switch (doc->m_current_type)
	{
	case SELECTED:
	{
				   //리스트 전체를 돌면서 만약 객체가 선택되었으면 현재 선택리스트에 넣어 줌
				   POSITION pos = doc->gobj_list.GetTailPosition();

				   while (pos)
				   {
					   GObject* gobj = (GObject*)doc->gobj_list.GetPrev(pos);
					   BOOL isDuplication = FALSE;

					   if (gobj->pointInRgn(point))			//만약 포인트가 있으면
					   {
						   POSITION currListPos = doc->current_gobj.GetHeadPosition();

						   while (currListPos)			//현재의 리스트에 있는지 검사한 후 있으면 isDuplication TRUE;
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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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

	//점 이동 풀어줄려고..
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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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


	//현재 선택된 오프젝트가 있으면 변경해줌.
	if (doc->current_gobj.GetCount() > 0)
	{
		POSITION pos = doc->current_gobj.GetHeadPosition();
		GObject* gobj;
		while (pos)
		{
			gobj = (GObject*)doc->current_gobj.GetNext(pos);			// element 가져와서 
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

	//현재 선택된 오프젝트가 있으면 변경해줌.
	if (doc->current_gobj.GetCount() > 0)
	{
		POSITION pos = doc->current_gobj.GetHeadPosition();
		GObject* gobj;
		while (pos)
		{
			gobj = (GObject*)doc->current_gobj.GetNext(pos);			// element 가져와서
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
			gobj = (GObject*)doc->current_gobj.GetNext(pos);			// element 가져와서
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

	//현재 선택된 오프젝트가 있으면 변경해줌.
	if (doc->current_gobj.GetCount() > 0)
	{
		POSITION pos = doc->current_gobj.GetHeadPosition();
		GObject* gobj;
		while (pos)
		{
			gobj = (GObject*)doc->current_gobj.GetNext(pos);			// element 가져와서
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

	//그룹에서 해당 아이템 삭제하고 필요가 없어진 그룹을 삭제하는 작업을 하자.
//	doc->m_groupSet.GroupItemDelete(&doc->current_gobj);


	//이제 전체 리스트에서 삭제해줘야 함.
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


	//이제 전체 리스트에서 삭제해줘야 함.
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
