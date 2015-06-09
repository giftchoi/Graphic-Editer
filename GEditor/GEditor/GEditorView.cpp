
// GEditorView.cpp : CGEditorView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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

// CGEditorView 생성/소멸

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
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CGEditorView 그리기

void CGEditorView::OnDraw(CDC* pDC)
{
	CGEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CGEditorDoc* doc = (CGEditorDoc*)GetDocument();

	POSITION pos = doc->gobj_list.GetHeadPosition();

	while (pos != NULL)
	{
		GObject* gobj = (GObject*)doc->gobj_list.GetNext(pos);

		CPen pen(gobj->getLinePattern(), gobj->getThickness(), gobj->getColor());
		CPen *oldpen = pDC->SelectObject(&pen);

		gobj->draw(pDC);

		if (gobj->isSelected())
			gobj->drawBoundary(pDC);

		pDC->SelectObject(&oldpen);
	}
	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}

void CGEditorView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
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


// CGEditorView 진단

#ifdef _DEBUG
void CGEditorView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CGEditorView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CGEditorDoc* CGEditorView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGEditorDoc)));
	return (CGEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CGEditorView 메시지 처리기


void CGEditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CGEditorDoc* doc = (CGEditorDoc*)GetDocument();

	switch (doc->cur_type){
	case TEXT:
	{
		isdrawing = true;

		GText* g = new GText();
		g->set(point.x, point.y, point.x, point.y);
		g->setColor(doc->text_color);
		g->setFont(doc->fontName);
		g->setSize(doc->size);
		g->setItalic(doc->bItalic);
		g->setUnderline(doc->bUnderline);

		doc->cur_gobj = g;
		doc->gobj_list.AddTail((void*)g);
		break;
	}
	case ELLIPSE:
	{
		isdrawing = true;

		GEllipse* g = new GEllipse();
		g->set(point.x, point.y, point.x, point.y);
		g->setColor(doc->cur_color);
		g->setFull_color(doc->cur_fullcolor);
		g->setThickness(doc->cur_thickness);
		g->setLinePattern(doc->cur_linepattern);
		g->setFull_pattern(doc->cur_fullpattern);
		doc->cur_gobj = g;
		doc->gobj_list.AddTail((void*)g);
		break;
	}
	case RECTANGLE:
	{
		isdrawing = true;

		GRectangle* g = new GRectangle();
		g->set(point.x, point.y, point.x, point.y);
		g->setColor(doc->cur_color);
		g->setThickness(doc->cur_thickness);
		g->setLinePattern(doc->cur_linepattern);

		g->setFull_color(doc->cur_fullcolor);
		g->setFull_pattern(doc->cur_fullpattern);

		doc->cur_gobj = g;
		doc->gobj_list.AddTail((void*)g);
		break;
	}
	case LINE:
	{
		isdrawing = true;

		GLine* g = new GLine();
		g->set(point.x, point.y, point.x, point.y);
		g->setColor(doc->cur_color);
		g->setThickness(doc->cur_thickness);
		g->setLinePattern(doc->cur_linepattern);

		doc->cur_gobj = g;
		doc->gobj_list.AddTail((void*)g);

		break;
	}

	case POLYLINE:
	{
		isdrawing = true;

		GPolyline* g;
		if (doc->cur_gobj != NULL)
		{
			g = (GPolyline*)doc->cur_gobj;
			g->set(point.x, point.y, point.x, point.y);
			g->setArr();

		}

		else
		{
			g = new GPolyline();
			g->set(point.x, point.y, point.x, point.y);
			g->setArr();
			g->setColor(doc->cur_color);
			g->setThickness(doc->cur_thickness);
			g->setLinePattern(doc->cur_linepattern);

			doc->cur_gobj = g;
			doc->gobj_list.AddTail((void*)g);
		}
		break;
	}

	case SELECTED:
	{
		{
			POSITION pos = doc->gobj_list.GetTailPosition();

			bool found = false;
			GObject* gobj = NULL;
			while (pos != NULL)
			{
				gobj = (GObject*)doc->gobj_list.GetPrev(pos);
				if (gobj->isin(point))
				{
					found = true;
					doc->cur_gobj = gobj;

					// 점 내부를 CTRL키를 누른채 클릭했을 경우
					if (m_ctrl)
					{
						// 이미 선택 되있을 경우, 리스트 해제
						if (gobj->isSelected())
						{
							POSITION temp = doc->gobj_selected_list.Find(gobj);
							doc->gobj_selected_list.RemoveAt(temp);
						}

						// 객체가 선택이 안되있을 경우 선택하고 리스트에 추가
						else
							doc->gobj_selected_list.AddTail(gobj);

					}
					// 점 내부를 CTRL키를 뗀 채 클릭했을 경우
					else if (!m_ctrl)
					{
						ismoving = true;
						isselect = true;
						// 선택은 up에서 처리
					}
					break;
				} // 내부 선택 끝

			}
			// 외부를 선택했을 경우
			if (!found)
			{
				if (m_ctrl)
					;
				else if (!m_ctrl)
					doc->gobj_selected_list.RemoveAll();
			}

			// gobj_list 갱신, bool selected
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

		CPen pen(doc->cur_gobj->getLinePattern(), doc->cur_gobj->getThickness(), doc->cur_gobj->getColor());
		CPen *oldpen = dc.SelectObject(&pen);

		doc->cur_gobj->draw(&dc);

		dc.SelectObject(&oldpen);
	}

	if (doc->cur_type == POLYLINE)
		;
	else if (doc->cur_type == TEXT)
		isdrawing = false;

	else if (doc->cur_type == SELECTED){

		if (!m_ctrl && doc->cur_gobj != NULL && isselect)
		{
			doc->gobj_selected_list.RemoveAll();
			doc->gobj_selected_list.AddTail(doc->cur_gobj);
			isselect = !isselect;
		}

		POSITION p = doc->gobj_list.GetHeadPosition();
		while (p != NULL)
		{
			GObject* obj = (GObject*)doc->gobj_list.GetNext(p);
			obj->resetSelected();
		}

		p = doc->gobj_selected_list.GetHeadPosition();
		while (p != NULL)
		{
			GObject* obj = (GObject*)doc->gobj_selected_list.GetNext(p);
			obj->setSelected();
		}

		ismoving = false;
		doc->cur_gobj = NULL;
	}

	else{
		isdrawing = false;
		doc->cur_gobj = NULL;
	}

	Invalidate();
	CScrollView::OnLButtonUp(nFlags, point);
}


void CGEditorView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CGEditorDoc* doc = (CGEditorDoc*)GetDocument();

	isdrawing = false;
	if (doc->cur_type == POLYLINE)
	{
		GPolyline *g = (GPolyline*)doc->cur_gobj;
		g->complete();
	}
	doc->cur_gobj = NULL;
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


			cur_gobj = doc->cur_gobj;

			cur_gobj->draw(&dc);
			cur_gobj->set(point.x, point.y);

			cur_gobj->draw(&dc);

			dc.SetROP2(oldrop2);
		}

		else if (ismoving){
			POSITION pos = doc->gobj_selected_list.GetHeadPosition();

			while (pos != NULL)
			{
				int oldrop2 = dc.GetROP2();
				dc.SetROP2(R2_XORPEN);

				cur_gobj = (GObject*)doc->gobj_selected_list.GetNext(pos);

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
		if (doc->cur_type == SELECTED)
		{
			POSITION pos = doc->gobj_selected_list.GetHeadPosition();

			GObject* pobj = (GObject*)doc->gobj_selected_list.GetHead();

			if (doc->gobj_selected_list.GetCount() == 1 && pobj->type() == POLYLINE && pobj->getmovemode() != -1)
			{
				GPolyline* pline = (GPolyline*)pobj;
				pline->onePointdel();
			}
			else{
				while (pos != NULL)
				{
					GObject* obj = (GObject*)doc->gobj_selected_list.GetNext(pos);

					POSITION p = doc->gobj_list.GetHeadPosition();
					while (p != NULL)
					{
						POSITION temp = p;
						GObject* g = (GObject*)doc->gobj_list.GetNext(p);

						if (obj == g){
							doc->gobj_list.RemoveAt(temp);
							delete g;
						}
					}
				}

				doc->gobj_selected_list.RemoveAll();
			}
		}
	}

	if (nChar == 'C'){
		doc->temp_list.RemoveAll();
		if (!doc->gobj_selected_list.IsEmpty()){
			POSITION pos = doc->gobj_selected_list.GetHeadPosition();
			while (pos){
				GObject* temp = (GObject*)(doc->gobj_selected_list.GetNext(pos));
				doc->temp_list.AddTail((void*)temp);
			}
		}
		else
			doc->temp_list.AddTail((void*)doc->cur_gobj);
		POSITION pos = doc->temp_list.GetHeadPosition();
		while (pos){
			GObject* temp = (GObject*)(doc->temp_list.GetNext(pos));

			switch (temp->type())
			{
			case RECTANGLE:
			{
				GRectangle* rec = (GRectangle*)temp;

				doc->gobj_selected_list.RemoveAll();
				GRectangle* g = new GRectangle();
				g->set(rec->getPoint().x, rec->getPoint().y, rec->getPoint_end().x, rec->getPoint_end().y);
				g->setColor(rec->getColor());
				g->setFull_color(rec->getFull_color());
				g->setThickness(rec->getThickness());
				g->setLinePattern(rec->getLinePattern());
				g->setFull_pattern(rec->getFull_pattern());
				g->move(50, 50);
				POSITION p = doc->temp_list.Find(temp);
				doc->temp_list.SetAt(p, (void*)g);
				break;
			}
			case ELLIPSE:
			{
				GEllipse* ell = (GEllipse*)temp;

				doc->gobj_selected_list.RemoveAll();
				GEllipse* g = new GEllipse();
				g->set(temp->getPoint().x, temp->getPoint().y, temp->getPoint_end().x, temp->getPoint_end().y);
				g->setColor(temp->getColor());
				g->setFull_color(ell->getFull_color());
				g->setThickness(temp->getThickness());
				g->setLinePattern(ell->getLinePattern());
				g->setFull_pattern(ell->getFull_pattern());
				g->move(50, 50);
				POSITION p = doc->temp_list.Find(temp);
				doc->temp_list.SetAt(p, (void*)g);
				break;
			}
			case TEXT:
			{
				GText* txt = (GText*)temp;

				doc->gobj_selected_list.RemoveAll();
				GText* g = new GText();
				g->set(temp->getPoint().x, temp->getPoint().y, temp->getPoint_end().x, temp->getPoint_end().y);
				g->setColor(temp->getColor());
				g->setFont(txt->getFontName());
				g->setSize(txt->getSize());
				g->setItalic(txt->getItalic());
				g->setUnderline(txt->getUnderline());
				g->move(50, 50);
				POSITION p = doc->temp_list.Find(temp);
				doc->temp_list.SetAt(p, (void*)g);
				break;
			}
			}
		}
	}
	if (nChar == 'V'){
		POSITION pos = doc->temp_list.GetHeadPosition();
		while (pos){
			GObject* temp = (GObject*)(doc->temp_list.GetNext(pos));
			doc->gobj_list.AddTail((void*)temp);
			//POSITION p = doc->temp_list.Find(temp);
			//doc->temp_list.SetAt(p,(void*)temp);
		}
		doc->temp_list.RemoveAll();
		Invalidate();
	}

	if (nChar == 'X'){
		doc->temp_list.RemoveAll();
		if (!doc->gobj_selected_list.IsEmpty()){
			POSITION pos = doc->gobj_selected_list.GetHeadPosition();
			while (pos){
				GObject* temp = (GObject*)(doc->gobj_selected_list.GetNext(pos));
				doc->temp_list.AddTail((void*)temp);
			}
		}
		else
			doc->temp_list.AddTail((void*)doc->cur_gobj);

		POSITION pos = doc->temp_list.GetHeadPosition();
		while (pos){
			GObject* temp = (GObject*)(doc->temp_list.GetNext(pos));

			switch (temp->type())
			{
			case RECTANGLE:
			{

				GRectangle* rec = (GRectangle*)temp;
				doc->gobj_selected_list.RemoveAll();
				GRectangle* g = new GRectangle();
				g->set(temp->getPoint().x, temp->getPoint().y, temp->getPoint_end().x, temp->getPoint_end().y);
				g->setColor(temp->getColor());
				g->setFull_color(rec->getFull_color());
				g->setThickness(temp->getThickness());
				g->setLinePattern(rec->getLinePattern());
				g->setFull_pattern(rec->getFull_pattern());
				g->move(50, 50);
				POSITION p = doc->temp_list.Find(temp);
				doc->temp_list.SetAt(p, (void*)g);
				p = doc->gobj_list.Find(temp);
				delete(temp);
				doc->gobj_list.RemoveAt(p);
				doc->cur_gobj = NULL;
				break;
			}
			case ELLIPSE:
			{
				GEllipse* ell = (GEllipse*)temp;

				doc->gobj_selected_list.RemoveAll();
				GEllipse* g = new GEllipse();
				g->set(temp->getPoint().x, temp->getPoint().y, temp->getPoint_end().x, temp->getPoint_end().y);
				g->setColor(temp->getColor());
				g->setFull_color(ell->getFull_color());
				g->setThickness(temp->getThickness());
				g->setLinePattern(ell->getLinePattern());
				g->setFull_pattern(ell->getFull_pattern());
				g->move(50, 50);
				POSITION p = doc->temp_list.Find(temp);
				doc->temp_list.SetAt(p, (void*)g);
				p = doc->gobj_list.Find(temp);
				delete(temp);
				doc->gobj_list.RemoveAt(p);
				doc->cur_gobj = NULL;
				break;
			}
			case TEXT:
			{
				GText* txt = (GText*)temp;

				doc->gobj_selected_list.RemoveAll();
				GText* g = new GText();
				g->set(temp->getPoint().x, temp->getPoint().y, temp->getPoint_end().x, temp->getPoint_end().y);
				g->setColor(temp->getColor());
				g->setFont(txt->getFontName());
				g->setSize(txt->getSize());
				g->setItalic(txt->getItalic());
				g->setUnderline(txt->getUnderline());
				g->move(50, 50);
				POSITION p = doc->temp_list.Find(temp);
				doc->temp_list.SetAt(p, (void*)g);
				p = doc->gobj_list.Find(temp);
				delete(temp);
				doc->gobj_list.RemoveAt(p);
				doc->cur_gobj = NULL;
				break;
			}
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
	if (doc->gobj_selected_list.GetCount() == 1){
		GObject* obj = (GObject*)doc->gobj_selected_list.GetHead();

		if (obj->type() == TEXT){
			doc->cur_gobj = obj;
			found = true;
		}
	}

	if (doc->cur_type == TEXT || found){
		GText *g = (GText*)doc->cur_gobj;
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

	switch (doc->cur_type)
	{
	case SELECTED:
	{
		POSITION pos = doc->gobj_list.GetTailPosition();
		bool found = false;

		GObject* gobj = NULL;

		while (pos != NULL)
		{
			gobj = (GObject*)doc->gobj_list.GetPrev(pos);
			if (gobj->isin(point))
			{
				found = true;
				break;
			}
		}

		if (found) {
			doc->cur_gobj = gobj;
			doc->gobj_context_menu_enabled = true;

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
