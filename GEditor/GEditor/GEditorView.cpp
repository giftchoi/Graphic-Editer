
// GEditorView.cpp : CGEditorView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "GEditor.h"
#endif

#include "GEditorDoc.h"
#include "GEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGEditorView

IMPLEMENT_DYNCREATE(CGEditorView, CScrollView)

BEGIN_MESSAGE_MAP(CGEditorView, CScrollView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_CHAR()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// CGEditorView ����/�Ҹ�

CGEditorView::CGEditorView()
{
	isdrawing = false;
	ismoving = false;
	isselect = false;

	m_ctrl = false;

}

CGEditorView::~CGEditorView()
{
}

BOOL CGEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CScrollView::PreCreateWindow(cs);
}

// CGEditorView �׸���

void CGEditorView::OnDraw(CDC* pDC)
{
	CGEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CGEditorDoc* doc = (CGEditorDoc*)GetDocument();

	POSITION pos = doc->Glist.GetHeadPosition();

	while (pos != NULL)
	{
		GObject* gobj = (GObject*)doc->Glist.GetNext(pos);

		CPen pen(gobj->getLinePattern(), gobj->getThickness(), gobj->getColor());
		CPen *oldpen = pDC->SelectObject(&pen);

		gobj->draw(pDC);

		if (gobj->isSelected())
			gobj->drawBoundary(pDC);

		pDC->SelectObject(&oldpen);
	}
	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}

void CGEditorView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: �� ���� ��ü ũ�⸦ ����մϴ�.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CGEditorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CGEditorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CGEditorView ����

#ifdef _DEBUG
void CGEditorView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CGEditorView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CGEditorDoc* CGEditorView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGEditorDoc)));
	return (CGEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CGEditorView �޽��� ó����


void CGEditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CGEditorDoc* doc = (CGEditorDoc*)GetDocument();

	switch (doc->type){
	case TEXT:
	{
		isdrawing = true;

		GText* g = new GText();
		g->set(point.x, point.y, point.x, point.y);
		g->setColor(doc->textcolor);
		g->setFont(doc->font);
		g->setSize(doc->size);
		g->setItalic(doc->isSlide);
		g->setUnderline(doc->isUnderline);

		doc->gobj = g;
		doc->Glist.AddTail((void*)g);
		break;
	}
	case ELLIPSE:
	{
		isdrawing = true;

		GEllipse* g = new GEllipse();
		g->set(point.x, point.y, point.x, point.y);
		g->setColor(doc->linecolor);
		g->setFull_color(doc->regioncolor);
		g->setThickness(doc->bold);
		g->setLinePattern(doc->linepattern);
		g->setFull_pattern(doc->regionpattern);
		doc->gobj = g;
		doc->Glist.AddTail((void*)g);
		break;
	}
	case RECTANGLE:
	{
		isdrawing = true;

		GRectangle* g = new GRectangle();
		g->set(point.x, point.y, point.x, point.y);
		g->setColor(doc->linecolor);
		g->setThickness(doc->bold);
		g->setLinePattern(doc->linepattern);

		g->setFull_color(doc->regioncolor);
		g->setFull_pattern(doc->regionpattern);

		doc->gobj = g;
		doc->Glist.AddTail((void*)g);
		break;
	}
	case LINE:
	{
		isdrawing = true;

		GLine* g = new GLine();
		g->set(point.x, point.y, point.x, point.y);
		g->setColor(doc->linecolor);
		g->setThickness(doc->bold);
		g->setLinePattern(doc->linepattern);

		doc->gobj = g;
		doc->Glist.AddTail((void*)g);

		break;
	}

	case POLYLINE:
	{
		isdrawing = true;

		GPolyline* g;
		if (doc->gobj != NULL)
		{
			g = (GPolyline*)doc->gobj;
			g->set(point.x, point.y, point.x, point.y);
			g->setArr();

		}

		else
		{
			g = new GPolyline();
			g->set(point.x, point.y, point.x, point.y);
			g->setArr();
			g->setColor(doc->linecolor);
			g->setThickness(doc->bold);
			g->setLinePattern(doc->linepattern);

			doc->gobj = g;
			doc->Glist.AddTail((void*)g);
		}
		break;
	}

	case SELECTED:
	{
		{
			POSITION pos = doc->Glist.GetTailPosition();

			bool found = false;
			GObject* gobj = NULL;
			while (pos != NULL)
			{
				gobj = (GObject*)doc->Glist.GetPrev(pos);
				if (gobj->isin(point))
				{
					found = true;
					doc->gobj = gobj;

					// �� ���θ� CTRLŰ�� ����ä Ŭ������ ���
					if (m_ctrl)
					{
						// �̹� ���� ������ ���, ����Ʈ ����
						if (gobj->isSelected())
						{
							POSITION temp = doc->selectedlist.Find(gobj);
							doc->selectedlist.RemoveAt(temp);
						}

						// ��ü�� ������ �ȵ����� ��� �����ϰ� ����Ʈ�� �߰�
						else
							doc->selectedlist.AddTail(gobj);

					}
					// �� ���θ� CTRLŰ�� �� ä Ŭ������ ���
					else if (!m_ctrl)
					{
						ismoving = true;
						isselect = true;
						// ������ up���� ó��
					}
					break;
				} // ���� ���� ��

			}
			// �ܺθ� �������� ���
			if (!found)
			{
				if (m_ctrl)
					;
				else if (!m_ctrl)
					doc->selectedlist.RemoveAll();
			}

			// gobj_list ����, bool selected
			// ALL gobj->selected = false;
			// ButtonUP
		}

	}
	Invalidate();
	}
	CScrollView::OnLButtonDown(nFlags, point);
}


void CGEditorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CGEditorDoc* doc = (CGEditorDoc*)GetDocument();

	if (isdrawing)
	{
		CClientDC dc(this);

		CPen pen(doc->gobj->getLinePattern(), doc->gobj->getThickness(), doc->gobj->getColor());
		CPen *oldpen = dc.SelectObject(&pen);

		doc->gobj->draw(&dc);

		dc.SelectObject(&oldpen);
	}

	if (doc->type == POLYLINE)
		;
	else if (doc->type == TEXT)
		isdrawing = false;

	else if (doc->type == SELECTED){

		if (!m_ctrl && doc->gobj != NULL && isselect)
		{
			doc->selectedlist.RemoveAll();
			doc->selectedlist.AddTail(doc->gobj);
			isselect = !isselect;
		}

		POSITION p = doc->Glist.GetHeadPosition();
		while (p != NULL)
		{
			GObject* obj = (GObject*)doc->Glist.GetNext(p);
			obj->resetSelected();
		}

		p = doc->selectedlist.GetHeadPosition();
		while (p != NULL)
		{
			GObject* obj = (GObject*)doc->selectedlist.GetNext(p);
			obj->setSelected();
		}

		ismoving = false;
		doc->gobj = NULL;
	}

	else{
		isdrawing = false;
		doc->gobj = NULL;
	}

	Invalidate();
	CScrollView::OnLButtonUp(nFlags, point);
}


void CGEditorView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CGEditorDoc* doc = (CGEditorDoc*)GetDocument();

	isdrawing = false;
	if (doc->type == POLYLINE)
	{
		GPolyline *g = (GPolyline*)doc->gobj;
		g->complete();
	}
	doc->gobj = NULL;
	CScrollView::OnLButtonDblClk(nFlags, point);
}


void CGEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
	CClientDC dc(this);

	CGEditorDoc* doc = (CGEditorDoc*)GetDocument();
	int dx = point.x - doc->point.x;
	int dy = point.y - doc->point.y;
	doc->point = point;

	isselect = false;
	GObject* cur_gobj = NULL;
	if (isdrawing || ismoving)
	{
		if (isdrawing)
		{
			int oldrop2 = dc.GetROP2();
			dc.SetROP2(R2_XORPEN);


			cur_gobj = doc->gobj;

			cur_gobj->draw(&dc);
			cur_gobj->set(point.x, point.y);

			cur_gobj->draw(&dc);

			dc.SetROP2(oldrop2);
		}

		else if (ismoving){
			POSITION pos = doc->selectedlist.GetHeadPosition();

			while (pos != NULL)
			{
				int oldrop2 = dc.GetROP2();
				dc.SetROP2(R2_XORPEN);

				cur_gobj = (GObject*)doc->selectedlist.GetNext(pos);

				if (cur_gobj->type() == TEXT)
				{
					COLORREF temp = cur_gobj->getColor();
					cur_gobj->setColor(RGB(255, 255, 255));
					cur_gobj->draw(&dc);
					cur_gobj->move(dx, dy);
					cur_gobj->draw(&dc);
					Invalidate();

					cur_gobj->setColor(temp);

				}
				else{

					cur_gobj->draw(&dc);
					cur_gobj->move(dx, dy);
					cur_gobj->draw(&dc);
					dc.SetROP2(oldrop2);
				}
			}
		}
	}
	CScrollView::OnMouseMove(nFlags, point);
}


void CGEditorView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CGEditorDoc* doc = (CGEditorDoc*)GetDocument();

	if (nChar == VK_CONTROL)
	{
		m_ctrl = true;
	}

	else if (nChar == VK_DELETE)
	{
		if (doc->type == SELECTED)
		{
			POSITION pos = doc->selectedlist.GetHeadPosition();

			GObject* pobj = (GObject*)doc->selectedlist.GetHead();

			if (doc->selectedlist.GetCount() == 1 && pobj->type() == POLYLINE && pobj->getmovemode() != -1)
			{
				GPolyline* pline = (GPolyline*)pobj;
				pline->onePointdel();
			}
			else{
				while (pos != NULL)
				{
					GObject* obj = (GObject*)doc->selectedlist.GetNext(pos);

					POSITION p = doc->Glist.GetHeadPosition();
					while (p != NULL)
					{
						POSITION temp = p;
						GObject* g = (GObject*)doc->Glist.GetNext(p);

						if (obj == g){
							doc->Glist.RemoveAt(temp);
							delete g;
						}
					}
				}

				doc->selectedlist.RemoveAll();
			}
		}
	}
	Invalidate();
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CGEditorView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_ctrl = false;
	CScrollView::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CGEditorView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CGEditorDoc* doc = (CGEditorDoc*)GetDocument();
	CClientDC dc(this);

	bool found = false;
	if (doc->selectedlist.GetCount() == 1){
		GObject* obj = (GObject*)doc->selectedlist.GetHead();

		if (obj->type() == TEXT){
			doc->gobj = obj;
			found = true;
		}
	}

	if (doc->type == TEXT || found){
		GText *g = (GText*)doc->gobj;
		g->setChar(nChar);
		g->draw(&dc);
		Invalidate();
		g->setChar(NULL);
	}
	CScrollView::OnChar(nChar, nRepCnt, nFlags);
}


void CGEditorView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CGEditorDoc* doc = (CGEditorDoc*)GetDocument();

	switch (doc->type)
	{
	case SELECTED:
	{
					 POSITION pos = doc->Glist.GetTailPosition();
		bool found = false;

		GObject* gobj = NULL;

		while (pos != NULL)
		{
			gobj = (GObject*)doc->Glist.GetPrev(pos);
			if (gobj->isin(point))
			{
				found = true;
				break;
			}
		}

		if (found) {
			doc->gobj = gobj;
			doc->isEnabled = true;

			CMenu menu;
			menu.LoadMenu(IDR_MAINFRAME);
			CMenu* pMenu = menu.GetSubMenu(4);

			POINT screen_coord_pt = point;
			ClientToScreen(&screen_coord_pt);

			pMenu->TrackPopupMenu(
				TPM_LEFTALIGN | TPM_RIGHTBUTTON,
				screen_coord_pt.x, screen_coord_pt.y, this);

			break;
		}

	}
	}
	CScrollView::OnRButtonDown(nFlags, point);
}
