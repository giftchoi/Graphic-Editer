
// GEditorDoc.cpp : CGEditorDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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


// CGEditorDoc 생성/소멸

CGEditorDoc::CGEditorDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
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
	fontName = "굴림";
	text_color = RGB(0, 0, 0);
}

CGEditorDoc::~CGEditorDoc()
{
}

BOOL CGEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.
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
	fontName = "굴림";
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
		// TODO: 여기에 로딩 코드를 추가합니다.
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

// 축소판 그림을 지원합니다.
void CGEditorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
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

// 검색 처리기를 지원합니다.
void CGEditorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
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

// CGEditorDoc 진단

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


// CGEditorDoc 명령


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
	fontName = "궁서";

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
	fontName = "굴림";

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
	fontName = "바탕";

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
