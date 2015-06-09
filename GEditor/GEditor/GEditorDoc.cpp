
// GEditorDoc.cpp : CGEditorDoc Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "GEditor.h"
#endif

#include "GEditorDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGEditorDoc

IMPLEMENT_DYNCREATE(CGEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CGEditorDoc, CDocument)
	ON_COMMAND(ID_LINERED, &CGEditorDoc::OnLineRed)
	ON_COMMAND(ID_LINEGREEN, &CGEditorDoc::OnLineGreen)
	ON_COMMAND(ID_LINEBLUE, &CGEditorDoc::OnLineBlue)
	ON_COMMAND(ID_BOLD10, &CGEditorDoc::OnBold1)
	ON_COMMAND(ID_BOLD5, &CGEditorDoc::OnBold2)
	ON_COMMAND(ID_BOLD1, &CGEditorDoc::OnBold3)
	ON_COMMAND(ID_LINEPATTERN1, &CGEditorDoc::OnLinePattern)
	ON_COMMAND(ID_LINEPATTERN2, &CGEditorDoc::OnLinePattern2)
	ON_COMMAND(ID_LINEPATTERN3, &CGEditorDoc::OnLinePatternDot)
	ON_COMMAND(ID_REGIONRED, &CGEditorDoc::OnRegionRed)
	ON_COMMAND(ID_REGIONGREEN, &CGEditorDoc::OnRegionGreen)
	ON_COMMAND(ID_REGIONBLUE, &CGEditorDoc::OnRegionBlue)
	ON_COMMAND(ID_REGIONPATTERN1, &CGEditorDoc::OnRegionPattern)
	ON_COMMAND(ID_REGIONPATTERN2, &CGEditorDoc::OnRegionPatternLine)
	ON_COMMAND(ID_REGIONPATTERN3, &CGEditorDoc::OnRegionPatternCheck)
	ON_COMMAND(ID_FONT1, &CGEditorDoc::OnFont1)
	ON_COMMAND(ID_FONT2, &CGEditorDoc::OnFont2)
	ON_COMMAND(ID_FONT3, &CGEditorDoc::OnFont3)
	ON_COMMAND(ID_FONTCOLORRED, &CGEditorDoc::OnFontColorRed)
	ON_COMMAND(ID_FONTCOLORGREEN, &CGEditorDoc::OnFontColorGreen)
	ON_COMMAND(ID_FONTCOLORBLUE, &CGEditorDoc::OnFontColorBlue)
	ON_COMMAND(ID_SIZE30, &CGEditorDoc::OnSize1)
	ON_COMMAND(ID_SIZE50, &CGEditorDoc::OnSize2)
	ON_COMMAND(ID_SIZE100, &CGEditorDoc::OnSize3)
	ON_COMMAND(ID_TEXTUNDERLINE, &CGEditorDoc::OnTextUnderline)
	ON_COMMAND(ID_TEXTSLIDE, &CGEditorDoc::OnTextSlide)
	ON_COMMAND(ID_SELECTED, &CGEditorDoc::OnSelected)
	ON_COMMAND(ID_LINE, &CGEditorDoc::OnLine)
	ON_COMMAND(ID_POLYLINE, &CGEditorDoc::OnPolyline)
	ON_COMMAND(ID_ELLIPSE, &CGEditorDoc::OnEllipse)
	ON_COMMAND(ID_RECTANGLE, &CGEditorDoc::OnRetangle)
	ON_COMMAND(ID_TEXT, &CGEditorDoc::OnText)
END_MESSAGE_MAP()


// CGEditorDoc ����/�Ҹ�

CGEditorDoc::CGEditorDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.
	cur_gobj = NULL;

	cur_type = LINE;
	cur_linepattern = PS_SOLID;
	cur_color = RGB(255, 0, 0);
	cur_thickness = 1;

	cur_fullcolor = RGB(255, 0, 0);
	cur_fullpattern = 6;

	bItalic = false;
	bUnderline = false;
	size = 30;
	fontName = "����";
	text_color = RGB(0, 0, 0);
}

CGEditorDoc::~CGEditorDoc()
{
}

BOOL CGEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.
	cur_gobj = NULL;

	cur_type = LINE;
	cur_linepattern = PS_SOLID;
	cur_color = RGB(255, 0, 0);
	cur_thickness = 1;

	cur_fullcolor = RGB(255, 0, 0);
	cur_fullpattern = 6;

	bItalic = false;
	bUnderline = false;
	size = 30;
	fontName = "����";
	text_color = RGB(0, 0, 0);

	gobj_list.RemoveAll();
	gobj_selected_list.RemoveAll();
	return TRUE;
}




// CGEditorDoc serialization

void CGEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		int t = cur_type;
		//object_list.Serialize(ar);
		ar << t << point << cur_color << cur_fullcolor << cur_thickness << cur_linepattern << cur_fullpattern << gobj_context_menu_enabled << bItalic << bUnderline << size << fontName << text_color;
		POSITION pos = gobj_list.GetHeadPosition();
		ar << gobj_list.GetCount();
		while (pos){
			GObject* temp = (GObject*)gobj_list.GetNext(pos);
			temp->serialize(ar, true);
		}
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
		int t; int count;
		//object_list.Serialize(ar);
		ar >> t >> point >> cur_color >> cur_fullcolor >> cur_thickness >> cur_linepattern >> cur_fullpattern >> gobj_context_menu_enabled >> bItalic >> bUnderline >> size >> fontName >> text_color;
		cur_type=(GType)t;
		POSITION pos = gobj_list.GetHeadPosition();
		ar>>count;
		for(int i=0;i<count;i++){
			ar>>t;
			switch (t){
			case TEXT:{
				GText* temp = new GText();
				gobj_list.AddTail((void*)temp);
				temp->serialize(ar, false);
				cur_gobj= temp;
				break;}
			case ELLIPSE:{
				GEllipse* temp = new GEllipse();
				gobj_list.AddTail((void*)temp);
				temp->serialize(ar, false);
				cur_gobj= temp;
				break;}
			case RECTANGLE:{
				GRectangle* temp = new GRectangle();
				gobj_list.AddTail((void*)temp);
				temp->serialize(ar, false);
				cur_gobj= temp;
				break;}
			case LINE:{
				GLine* temp = new GLine();
				gobj_list.AddTail((void*)temp);
				temp->serialize(ar, false);
				cur_gobj= temp;
				break;}
			case POLYLINE:{
				GPolyline* temp = new GPolyline();
				gobj_list.AddTail((void*)temp);
				temp->serialize(ar, false);
				cur_gobj= temp;
				break;}
			}
		}
	}
}

#ifdef SHARED_HANDLERS

// ����� �׸��� �����մϴ�.
void CGEditorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// ������ �����͸� �׸����� �� �ڵ带 �����Ͻʽÿ�.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// �˻� ó���⸦ �����մϴ�.
void CGEditorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ������ �����Ϳ��� �˻� �������� �����մϴ�.
	// ������ �κ��� ";"�� ���еǾ�� �մϴ�.

	// ��: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CGEditorDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CGEditorDoc ����

#ifdef _DEBUG
void CGEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGEditorDoc ���


void CGEditorDoc::OnLineRed()
{
	cur_color = RGB(255, 0, 0);

	if (gobj_context_menu_enabled == true)
	{
		POSITION pos = gobj_selected_list.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)gobj_selected_list.GetNext(pos);
			gobj->setColor(cur_color);
		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnLineGreen()
{
	cur_color = RGB(0, 255, 0);

	if (gobj_context_menu_enabled == true)
	{
		POSITION pos = gobj_selected_list.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)gobj_selected_list.GetNext(pos);
			gobj->setColor(cur_color);
		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnLineBlue()
{
	cur_color = RGB(0, 0, 255);

	if (gobj_context_menu_enabled == true)
	{
		POSITION pos = gobj_selected_list.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)gobj_selected_list.GetNext(pos);
			gobj->setColor(cur_color);
		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnBold1()
{
	cur_thickness = 10;

	if (gobj_context_menu_enabled == true)
	{
		POSITION pos = gobj_selected_list.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)gobj_selected_list.GetNext(pos);
			gobj->setThickness(cur_thickness);
		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnBold2()
{
	cur_thickness = 5;

	if (gobj_context_menu_enabled == true)
	{
		POSITION pos = gobj_selected_list.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)gobj_selected_list.GetNext(pos);
			gobj->setThickness(cur_thickness);
		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnBold3()
{
	cur_thickness = 1;

	if (gobj_context_menu_enabled == true)
	{
		POSITION pos = gobj_selected_list.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)gobj_selected_list.GetNext(pos);
			gobj->setThickness(cur_thickness);
		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnLinePattern()
{
	cur_linepattern = PS_SOLID;

	if (gobj_context_menu_enabled == true)
	{
		POSITION pos = gobj_selected_list.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)gobj_selected_list.GetNext(pos);
			gobj->setLinePattern(cur_linepattern);
		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnLinePattern2()
{
	cur_linepattern = PS_DASH;

	if (gobj_context_menu_enabled == true)
	{
		POSITION pos = gobj_selected_list.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)gobj_selected_list.GetNext(pos);
			gobj->setLinePattern(cur_linepattern);
		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnLinePatternDot()
{
	cur_linepattern = PS_DOT;

	if (gobj_context_menu_enabled == true)
	{
		POSITION pos = gobj_selected_list.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)gobj_selected_list.GetNext(pos);
			gobj->setLinePattern(cur_linepattern);
		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnRegionRed()
{
	cur_fullcolor = RGB(255, 0, 0);

	if (gobj_context_menu_enabled == true)
	{
		POSITION pos = gobj_selected_list.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)gobj_selected_list.GetNext(pos);

			if (gobj->type() == RECTANGLE)
			{
				GRectangle* g = (GRectangle*)gobj;
				g->setFull_color(cur_fullcolor);
			}
			else if (gobj->type() == ELLIPSE)
			{
				GEllipse *g = (GEllipse*)gobj;
				g->setFull_color(cur_fullcolor);
			}

		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnRegionGreen()
{
	cur_fullcolor = RGB(0, 255, 0);

	if (gobj_context_menu_enabled == true)
	{
		POSITION pos = gobj_selected_list.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)gobj_selected_list.GetNext(pos);

			if (gobj->type() == RECTANGLE)
			{
				GRectangle* g = (GRectangle*)gobj;
				g->setFull_color(cur_fullcolor);
			}
			else if (gobj->type() == ELLIPSE)
			{
				GEllipse *g = (GEllipse*)gobj;
				g->setFull_color(cur_fullcolor);
			}

		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnRegionBlue()
{
	cur_fullcolor = RGB(0, 0, 255);

	if (gobj_context_menu_enabled == true)
	{
		POSITION pos = gobj_selected_list.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)gobj_selected_list.GetNext(pos);

			if (gobj->type() == RECTANGLE)
			{
				GRectangle* g = (GRectangle*)gobj;
				g->setFull_color(cur_fullcolor);
			}
			else if (gobj->type() == ELLIPSE)
			{
				GEllipse *g = (GEllipse*)gobj;
				g->setFull_color(cur_fullcolor);
			}

		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnRegionPattern()
{
	cur_fullpattern = 6;

	if (gobj_context_menu_enabled == true)
	{
		POSITION pos = gobj_selected_list.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)gobj_selected_list.GetNext(pos);

			if (gobj->type() == RECTANGLE)
			{
				GRectangle* g = (GRectangle*)gobj;
				g->setFull_pattern(cur_fullpattern);
			}
			else if (gobj->type() == ELLIPSE)
			{
				GEllipse *g = (GEllipse*)gobj;
				g->setFull_pattern(cur_fullpattern);
			}

		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnRegionPatternLine()
{
	cur_fullpattern = HS_FDIAGONAL;

	if (gobj_context_menu_enabled == true)
	{
		POSITION pos = gobj_selected_list.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)gobj_selected_list.GetNext(pos);

			if (gobj->type() == RECTANGLE)
			{
				GRectangle* g = (GRectangle*)gobj;
				g->setFull_pattern(cur_fullpattern);
			}
			else if (gobj->type() == ELLIPSE)
			{
				GEllipse *g = (GEllipse*)gobj;
				g->setFull_pattern(cur_fullpattern);
			}

		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnRegionPatternCheck()
{
	cur_fullpattern = HS_CROSS;

	if (gobj_context_menu_enabled == true)
	{
		POSITION pos = gobj_selected_list.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)gobj_selected_list.GetNext(pos);

			if (gobj->type() == RECTANGLE)
			{
				GRectangle* g = (GRectangle*)gobj;
				g->setFull_pattern(cur_fullpattern);
			}
			else if (gobj->type() == ELLIPSE)
			{
				GEllipse *g = (GEllipse*)gobj;
				g->setFull_pattern(cur_fullpattern);
			}

		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnFont1()
{
	fontName = "�ü�";

	if (gobj_context_menu_enabled == true)
	{
		POSITION pos = gobj_selected_list.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)gobj_selected_list.GetNext(pos);

			if (gobj->type() == TEXT)
			{
				GText* g = (GText*)gobj;
				g->setFont(fontName);
			}

		}


		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnFont2()
{
	fontName = "����";

	if (gobj_context_menu_enabled == true)
	{
		POSITION pos = gobj_selected_list.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)gobj_selected_list.GetNext(pos);

			if (gobj->type() == TEXT)
			{
				GText* g = (GText*)gobj;
				g->setFont(fontName);
			}

		}


		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnFont3()
{
	fontName = "����";

	if (gobj_context_menu_enabled == true)
	{
		POSITION pos = gobj_selected_list.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)gobj_selected_list.GetNext(pos);

			if (gobj->type() == TEXT)
			{
				GText* g = (GText*)gobj;
				g->setFont(fontName);
			}

		}


		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnFontColorRed()
{
	text_color = RGB(255, 0, 0);

	if (gobj_context_menu_enabled == true)
	{
		POSITION pos = gobj_selected_list.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)gobj_selected_list.GetNext(pos);

			if (gobj->type() == TEXT)
			{
				GText* g = (GText*)gobj;
				g->setColor(text_color);
			}


		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnFontColorGreen()
{
	text_color = RGB(0, 255, 0);

	if (gobj_context_menu_enabled == true)
	{
		POSITION pos = gobj_selected_list.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)gobj_selected_list.GetNext(pos);

			if (gobj->type() == TEXT)
			{
				GText* g = (GText*)gobj;
				g->setColor(text_color);
			}


		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnFontColorBlue()
{
	text_color = RGB(0, 0, 255);

	if (gobj_context_menu_enabled == true)
	{
		POSITION pos = gobj_selected_list.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)gobj_selected_list.GetNext(pos);

			if (gobj->type() == TEXT)
			{
				GText* g = (GText*)gobj;
				g->setColor(text_color);
			}


		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnSize1()
{
	size = 30;

	if (gobj_context_menu_enabled == true)
	{
		POSITION pos = gobj_selected_list.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)gobj_selected_list.GetNext(pos);

			if (gobj->type() == TEXT)
			{
				GText* g = (GText*)gobj;
				g->setSize(size);
			}

		}


		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnSize2()
{
	size = 50;

	if (gobj_context_menu_enabled == true)
	{
		POSITION pos = gobj_selected_list.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)gobj_selected_list.GetNext(pos);

			if (gobj->type() == TEXT)
			{
				GText* g = (GText*)gobj;
				g->setSize(size);
			}

		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnSize3()
{
	size = 100;

	if (gobj_context_menu_enabled == true)
	{
		POSITION pos = gobj_selected_list.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)gobj_selected_list.GetNext(pos);

			if (gobj->type() == TEXT)
			{
				GText* g = (GText*)gobj;
				g->setSize(size);
			}

		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnTextUnderline()
{
	bUnderline = !bUnderline;

	if (gobj_context_menu_enabled == true)
	{
		POSITION pos = gobj_selected_list.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)gobj_selected_list.GetNext(pos);

			if (gobj->type() == TEXT)
			{
				GText* g = (GText*)gobj;
				g->setUnderline(bUnderline);
			}

		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnTextSlide()
{
	bItalic = !bItalic;

	if (gobj_context_menu_enabled == true)
	{
		POSITION pos = gobj_selected_list.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)gobj_selected_list.GetNext(pos);

			if (gobj->type() == TEXT)
			{
				GText* g = (GText*)gobj;
				g->setItalic(bItalic);
			}

		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnSelected()
{
	cur_type = SELECTED;
}


void CGEditorDoc::OnLine()
{
	cur_type = LINE;

	POSITION pos = gobj_selected_list.GetHeadPosition();

	while (pos != NULL)
	{
		GObject* g = (GObject*)gobj_selected_list.GetNext(pos);
		g->resetSelected();
	}

	gobj_selected_list.RemoveAll();

	CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
	p->Invalidate();
}


void CGEditorDoc::OnPolyline()
{
	cur_type = POLYLINE;

	POSITION pos = gobj_selected_list.GetHeadPosition();

	while (pos != NULL)
	{
		GObject* g = (GObject*)gobj_selected_list.GetNext(pos);
		g->resetSelected();
	}
	gobj_selected_list.RemoveAll();

	CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
	p->Invalidate();
}


void CGEditorDoc::OnEllipse()
{
	cur_type = ELLIPSE;

	POSITION pos = gobj_selected_list.GetHeadPosition();

	while (pos != NULL)
	{
		GObject* g = (GObject*)gobj_selected_list.GetNext(pos);
		g->resetSelected();
	}

	gobj_selected_list.RemoveAll();

	CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
	p->Invalidate();
}


void CGEditorDoc::OnRetangle()
{
	cur_type = RECTANGLE;

	POSITION pos = gobj_selected_list.GetHeadPosition();

	while (pos != NULL)
	{
		GObject* g = (GObject*)gobj_selected_list.GetNext(pos);
		g->resetSelected();
	}

	gobj_selected_list.RemoveAll();

	CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
	p->Invalidate();
}


void CGEditorDoc::OnText()
{
	cur_type = TEXT;

	POSITION pos = gobj_selected_list.GetHeadPosition();

	while (pos != NULL)
	{
		GObject* g = (GObject*)gobj_selected_list.GetNext(pos);
		g->resetSelected();
	}

	gobj_selected_list.RemoveAll();

	CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
	p->Invalidate();
}
