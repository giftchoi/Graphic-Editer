
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
	ON_COMMAND(ID_LINEYELLOW, &CGEditorDoc::OnLineYellow)
	ON_COMMAND(ID_LINEBLACK, &CGEditorDoc::OnLineBlack)
	ON_COMMAND(ID_LINEWHITE, &CGEditorDoc::OnLineWhite)
	ON_COMMAND(ID_REGIONYELLOW, &CGEditorDoc::OnRegionYellow)
	ON_COMMAND(ID_REGIONBLACK, &CGEditorDoc::OnRegionBlack)
	ON_COMMAND(ID_REGIONWHITE, &CGEditorDoc::OnRegionWhite)
	ON_COMMAND(ID_REGIONPATTERN4, &CGEditorDoc::OnRegionPatternH)
	ON_COMMAND(ID_REGIONPATTERN5, &CGEditorDoc::OnRegionPatternV)
	ON_COMMAND(ID_EDIT_CUT, &CGEditorDoc::OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, &CGEditorDoc::OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CGEditorDoc::OnEditPaste)
END_MESSAGE_MAP()


// CGEditorDoc 생성/소멸

CGEditorDoc::CGEditorDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	gobj = NULL;

	type = LINE;
	linepattern = PS_SOLID;
	linecolor = RGB(0, 0, 0);
	bold = 1;

	regioncolor = RGB(255, 255, 255);
	regionpattern = 6;

	isSlide = false;
	isUnderline = false;
	size = 30;
	font = "굴림";
	textcolor = RGB(0, 0, 0);
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
	gobj = NULL;

	type = LINE;
	linepattern = PS_SOLID;
	linecolor = RGB(0, 0, 0);
	bold = 1;

	regioncolor = RGB(255, 255, 255);
	regionpattern = 6;

	isSlide = false;
	isUnderline = false;
	size = 30;
	font = "굴림";
	textcolor = RGB(0, 0, 0);

	Glist.RemoveAll();
	selectedlist.RemoveAll();
	return TRUE;
}




// CGEditorDoc serialization

void CGEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		int t = type;
		//object_list.Serialize(ar);
		ar << t << point << linecolor << regioncolor << bold << linepattern << regionpattern << isEnabled << isSlide << isUnderline << size << font << textcolor;
		POSITION pos = Glist.GetHeadPosition();
		ar << Glist.GetCount();
		while (pos){
			GObject* temp = (GObject*)Glist.GetNext(pos);
			temp->serialize(ar, true);
		}
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
		int t; int count;
		//object_list.Serialize(ar);
		ar >> t >> point >> linecolor >> regioncolor >> bold >> linepattern >> regionpattern >> isEnabled >> isSlide >> isUnderline >> size >> font >> textcolor;
		type = (GType)t;
		POSITION pos = Glist.GetHeadPosition();
		ar >> count;
		for (int i = 0; i<count; i++){
			ar >> t;
			switch (t){
			case TEXT:{
						  GText* temp = new GText();
						  Glist.AddTail((void*)temp);
						  temp->serialize(ar, false);
						  gobj = temp;
						  break; }
			case ELLIPSE:{
							 GEllipse* temp = new GEllipse();
							 Glist.AddTail((void*)temp);
							 temp->serialize(ar, false);
							 gobj = temp;
							 break; }
			case RECTANGLE:{
							   GRectangle* temp = new GRectangle();
							   Glist.AddTail((void*)temp);
							   temp->serialize(ar, false);
							   gobj = temp;
							   break; }
			case LINE:{
						  GLine* temp = new GLine();
						  Glist.AddTail((void*)temp);
						  temp->serialize(ar, false);
						  gobj = temp;
						  break; }
			case POLYLINE:{
							  GPolyline* temp = new GPolyline();
							  Glist.AddTail((void*)temp);
							  temp->serialize(ar, false);
							  gobj = temp;
							  break; }
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
	linecolor = RGB(255, 0, 0);

	if (isEnabled == true)
	{
		POSITION pos = selectedlist.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)selectedlist.GetNext(pos);
			gobj->setColor(linecolor);
		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnLineGreen()
{
	linecolor = RGB(0, 255, 0);

	if (isEnabled == true)
	{
		POSITION pos = selectedlist.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)selectedlist.GetNext(pos);
			gobj->setColor(linecolor);
		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnLineBlue()
{
	linecolor = RGB(0, 0, 255);

	if (isEnabled == true)
	{
		POSITION pos = selectedlist.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)selectedlist.GetNext(pos);
			gobj->setColor(linecolor);
		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnBold1()
{
	bold = 10;

	if (isEnabled == true)
	{
		POSITION pos = selectedlist.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)selectedlist.GetNext(pos);
			gobj->setThickness(bold);
		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnBold2()
{
	bold = 5;

	if (isEnabled == true)
	{
		POSITION pos = selectedlist.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)selectedlist.GetNext(pos);
			gobj->setThickness(bold);
		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnBold3()
{
	bold = 1;

	if (isEnabled == true)
	{
		POSITION pos = selectedlist.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)selectedlist.GetNext(pos);
			gobj->setThickness(bold);
		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnLinePattern()
{
	linepattern = PS_SOLID;

	if (isEnabled == true)
	{
		POSITION pos = selectedlist.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)selectedlist.GetNext(pos);
			gobj->setLinePattern(linepattern);
		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnLinePattern2()
{
	linepattern = PS_DASH;

	if (isEnabled == true)
	{
		POSITION pos = selectedlist.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)selectedlist.GetNext(pos);
			gobj->setLinePattern(linepattern);
		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnLinePatternDot()
{
	linepattern = PS_DOT;

	if (isEnabled == true)
	{
		POSITION pos = selectedlist.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)selectedlist.GetNext(pos);
			gobj->setLinePattern(linepattern);
		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnRegionRed()
{
	regioncolor = RGB(255, 0, 0);

	if (isEnabled == true)
	{
		POSITION pos = selectedlist.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)selectedlist.GetNext(pos);

			if (gobj->type() == RECTANGLE)
			{
				GRectangle* g = (GRectangle*)gobj;
				g->setFull_color(regioncolor);
			}
			else if (gobj->type() == ELLIPSE)
			{
				GEllipse *g = (GEllipse*)gobj;
				g->setFull_color(regioncolor);
			}

		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnRegionGreen()
{
	regioncolor = RGB(0, 255, 0);

	if (isEnabled == true)
	{
		POSITION pos = selectedlist.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)selectedlist.GetNext(pos);

			if (gobj->type() == RECTANGLE)
			{
				GRectangle* g = (GRectangle*)gobj;
				g->setFull_color(regioncolor);
			}
			else if (gobj->type() == ELLIPSE)
			{
				GEllipse *g = (GEllipse*)gobj;
				g->setFull_color(regioncolor);
			}

		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnRegionBlue()
{
	regioncolor = RGB(0, 0, 255);

	if (isEnabled == true)
	{
		POSITION pos = selectedlist.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)selectedlist.GetNext(pos);

			if (gobj->type() == RECTANGLE)
			{
				GRectangle* g = (GRectangle*)gobj;
				g->setFull_color(regioncolor);
			}
			else if (gobj->type() == ELLIPSE)
			{
				GEllipse *g = (GEllipse*)gobj;
				g->setFull_color(regioncolor);
			}

		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnRegionPattern()
{
	regionpattern = 6;

	if (isEnabled == true)
	{
		POSITION pos = selectedlist.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)selectedlist.GetNext(pos);

			if (gobj->type() == RECTANGLE)
			{
				GRectangle* g = (GRectangle*)gobj;
				g->setFull_pattern(regionpattern);
			}
			else if (gobj->type() == ELLIPSE)
			{
				GEllipse *g = (GEllipse*)gobj;
				g->setFull_pattern(regionpattern);
			}

		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnRegionPatternLine()
{
	regionpattern = HS_FDIAGONAL;

	if (isEnabled == true)
	{
		POSITION pos = selectedlist.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)selectedlist.GetNext(pos);

			if (gobj->type() == RECTANGLE)
			{
				GRectangle* g = (GRectangle*)gobj;
				g->setFull_pattern(regionpattern);
			}
			else if (gobj->type() == ELLIPSE)
			{
				GEllipse *g = (GEllipse*)gobj;
				g->setFull_pattern(regionpattern);
			}

		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnRegionPatternCheck()
{
	regionpattern = HS_CROSS;

	if (isEnabled == true)
	{
		POSITION pos = selectedlist.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)selectedlist.GetNext(pos);

			if (gobj->type() == RECTANGLE)
			{
				GRectangle* g = (GRectangle*)gobj;
				g->setFull_pattern(regionpattern);
			}
			else if (gobj->type() == ELLIPSE)
			{
				GEllipse *g = (GEllipse*)gobj;
				g->setFull_pattern(regionpattern);
			}

		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnFont1()
{
	font = "궁서";

	if (isEnabled == true)
	{
		POSITION pos = selectedlist.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)selectedlist.GetNext(pos);

			if (gobj->type() == TEXT)
			{
				GText* g = (GText*)gobj;
				g->setFont(font);
			}

		}


		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnFont2()
{
	font = "굴림";

	if (isEnabled == true)
	{
		POSITION pos = selectedlist.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)selectedlist.GetNext(pos);

			if (gobj->type() == TEXT)
			{
				GText* g = (GText*)gobj;
				g->setFont(font);
			}

		}


		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnFont3()
{
	font = "바탕";

	if (isEnabled == true)
	{
		POSITION pos = selectedlist.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)selectedlist.GetNext(pos);

			if (gobj->type() == TEXT)
			{
				GText* g = (GText*)gobj;
				g->setFont(font);
			}

		}


		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnFontColorRed()
{
	textcolor = RGB(255, 0, 0);

	if (isEnabled == true)
	{
		POSITION pos = selectedlist.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)selectedlist.GetNext(pos);

			if (gobj->type() == TEXT)
			{
				GText* g = (GText*)gobj;
				g->setColor(textcolor);
			}


		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnFontColorGreen()
{
	textcolor = RGB(0, 255, 0);

	if (isEnabled == true)
	{
		POSITION pos = selectedlist.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)selectedlist.GetNext(pos);

			if (gobj->type() == TEXT)
			{
				GText* g = (GText*)gobj;
				g->setColor(textcolor);
			}


		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnFontColorBlue()
{
	textcolor = RGB(0, 0, 255);

	if (isEnabled == true)
	{
		POSITION pos = selectedlist.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)selectedlist.GetNext(pos);

			if (gobj->type() == TEXT)
			{
				GText* g = (GText*)gobj;
				g->setColor(textcolor);
			}


		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnSize1()
{
	size = 30;

	if (isEnabled == true)
	{
		POSITION pos = selectedlist.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)selectedlist.GetNext(pos);

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

	if (isEnabled == true)
	{
		POSITION pos = selectedlist.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)selectedlist.GetNext(pos);

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

	if (isEnabled == true)
	{
		POSITION pos = selectedlist.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)selectedlist.GetNext(pos);

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
	isUnderline = !isUnderline;

	if (isEnabled == true)
	{
		POSITION pos = selectedlist.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)selectedlist.GetNext(pos);

			if (gobj->type() == TEXT)
			{
				GText* g = (GText*)gobj;
				g->setUnderline(isUnderline);
			}

		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnTextSlide()
{
	isSlide = !isSlide;

	if (isEnabled == true)
	{
		POSITION pos = selectedlist.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)selectedlist.GetNext(pos);

			if (gobj->type() == TEXT)
			{
				GText* g = (GText*)gobj;
				g->setItalic(isSlide);
			}

		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnSelected()
{
	type = SELECTED;
}


void CGEditorDoc::OnLine()
{
	type = LINE;

	POSITION pos = selectedlist.GetHeadPosition();

	while (pos != NULL)
	{
		GObject* g = (GObject*)selectedlist.GetNext(pos);
		g->resetSelected();
	}

	selectedlist.RemoveAll();

	CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
	p->Invalidate();
}


void CGEditorDoc::OnPolyline()
{
	type = POLYLINE;

	POSITION pos = selectedlist.GetHeadPosition();

	while (pos != NULL)
	{
		GObject* g = (GObject*)selectedlist.GetNext(pos);
		g->resetSelected();
	}
	selectedlist.RemoveAll();

	CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
	p->Invalidate();
}


void CGEditorDoc::OnEllipse()
{
	type = ELLIPSE;

	POSITION pos = selectedlist.GetHeadPosition();

	while (pos != NULL)
	{
		GObject* g = (GObject*)selectedlist.GetNext(pos);
		g->resetSelected();
	}

	selectedlist.RemoveAll();

	CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
	p->Invalidate();
}


void CGEditorDoc::OnRetangle()
{
	type = RECTANGLE;

	POSITION pos = selectedlist.GetHeadPosition();

	while (pos != NULL)
	{
		GObject* g = (GObject*)selectedlist.GetNext(pos);
		g->resetSelected();
	}

	selectedlist.RemoveAll();

	CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
	p->Invalidate();
}


void CGEditorDoc::OnText()
{
	type = TEXT;

	POSITION pos = selectedlist.GetHeadPosition();

	while (pos != NULL)
	{
		GObject* g = (GObject*)selectedlist.GetNext(pos);
		g->resetSelected();
	}

	selectedlist.RemoveAll();

	CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
	p->Invalidate();
}


void CGEditorDoc::OnLineYellow()
{
	linecolor = RGB(255, 255, 0);

	if (isEnabled == true)
	{
		POSITION pos = selectedlist.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)selectedlist.GetNext(pos);
			gobj->setColor(linecolor);
		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnLineBlack()
{
	linecolor = RGB(0, 0, 0);

	if (isEnabled == true)
	{
		POSITION pos = selectedlist.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)selectedlist.GetNext(pos);
			gobj->setColor(linecolor);
		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnLineWhite()
{
	linecolor = RGB(255, 255, 255);

	if (isEnabled == true)
	{
		POSITION pos = selectedlist.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)selectedlist.GetNext(pos);
			gobj->setColor(linecolor);
		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnRegionYellow()
{
	regioncolor = RGB(255, 255, 0);

	if (isEnabled == true)
	{
		POSITION pos = selectedlist.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)selectedlist.GetNext(pos);

			if (gobj->type() == RECTANGLE)
			{
				GRectangle* g = (GRectangle*)gobj;
				g->setFull_color(regioncolor);
			}
			else if (gobj->type() == ELLIPSE)
			{
				GEllipse *g = (GEllipse*)gobj;
				g->setFull_color(regioncolor);
			}

		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnRegionBlack()
{
	regioncolor = RGB(0, 0, 0);

	if (isEnabled == true)
	{
		POSITION pos = selectedlist.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)selectedlist.GetNext(pos);

			if (gobj->type() == RECTANGLE)
			{
				GRectangle* g = (GRectangle*)gobj;
				g->setFull_color(regioncolor);
			}
			else if (gobj->type() == ELLIPSE)
			{
				GEllipse *g = (GEllipse*)gobj;
				g->setFull_color(regioncolor);
			}

		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnRegionWhite()
{
	regioncolor = RGB(255, 255, 255);

	if (isEnabled == true)
	{
		POSITION pos = selectedlist.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)selectedlist.GetNext(pos);

			if (gobj->type() == RECTANGLE)
			{
				GRectangle* g = (GRectangle*)gobj;
				g->setFull_color(regioncolor);
			}
			else if (gobj->type() == ELLIPSE)
			{
				GEllipse *g = (GEllipse*)gobj;
				g->setFull_color(regioncolor);
			}

		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnRegionPatternH()
{
	regionpattern = HS_HORIZONTAL;

	if (isEnabled == true)
	{
		POSITION pos = selectedlist.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)selectedlist.GetNext(pos);

			if (gobj->type() == RECTANGLE)
			{
				GRectangle* g = (GRectangle*)gobj;
				g->setFull_pattern(regionpattern);
			}
			else if (gobj->type() == ELLIPSE)
			{
				GEllipse *g = (GEllipse*)gobj;
				g->setFull_pattern(regionpattern);
			}

		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnRegionPatternV()
{
	regionpattern = HS_VERTICAL;

	if (isEnabled == true)
	{
		POSITION pos = selectedlist.GetHeadPosition();

		while (pos != NULL)
		{
			GObject* gobj = (GObject*)selectedlist.GetNext(pos);

			if (gobj->type() == RECTANGLE)
			{
				GRectangle* g = (GRectangle*)gobj;
				g->setFull_pattern(regionpattern);
			}
			else if (gobj->type() == ELLIPSE)
			{
				GEllipse *g = (GEllipse*)gobj;
				g->setFull_pattern(regionpattern);
			}

		}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
	}
}


void CGEditorDoc::OnEditCut()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	temp_list.RemoveAll();
	if (!selectedlist.IsEmpty()){
		POSITION pos = selectedlist.GetHeadPosition();
		while (pos){
			GObject* temp = (GObject*)(selectedlist.GetNext(pos));
			temp_list.AddTail((void*)temp);
		}
	}
	else if (selectedlist.IsEmpty())
	{
		return;
	}
	else
		temp_list.AddTail((void*)gobj);

	POSITION pos = temp_list.GetHeadPosition();
	while (pos){
		GObject* temp = (GObject*)(temp_list.GetNext(pos));

		switch (temp->type())
		{
		case LINE:
		{
					 GLine* lin = (GLine*)temp;

					 selectedlist.RemoveAll();
					 GLine* g = new GLine();
					 g->set(lin->getPoint().x, lin->getPoint().y, lin->getPoint_end().x, lin->getPoint_end().y);
					 g->setColor(lin->getColor());
					 g->setThickness(lin->getThickness());
					 g->setLinePattern(lin->getLinePattern());
					 g->move(50, 50);
					 POSITION p = temp_list.Find(temp);
					 temp_list.SetAt(p, (void*)g);
					 p = Glist.Find(temp);
					 delete(temp);
					 Glist.RemoveAt(p);
					 gobj = NULL;
					 break;
		}
		case POLYLINE:
		{
						 GPolyline* plin = (GPolyline*)temp;

						 selectedlist.RemoveAll();
						 GPolyline* g = new GPolyline();
						 g->set(plin->getPoint().x, plin->getPoint().y, plin->getPoint_end().x, plin->getPoint_end().y);
						 g->setColor(plin->getColor());
						 g->setThickness(plin->getThickness());
						 g->setLinePattern(plin->getLinePattern());
						 g->move(50, 50);
						 POSITION p = temp_list.Find(temp);
						 temp_list.SetAt(p, (void*)g);
						 p = Glist.Find(temp);
						 delete(temp);
						 Glist.RemoveAt(p);
						 gobj = NULL;
						 break;
		}
		case RECTANGLE:
		{

						  GRectangle* rec = (GRectangle*)temp;
						  selectedlist.RemoveAll();
						  GRectangle* g = new GRectangle();
						  g->set(temp->getPoint().x, temp->getPoint().y, temp->getPoint_end().x, temp->getPoint_end().y);
						  g->setColor(temp->getColor());
						  g->setFull_color(rec->getFull_color());
						  g->setThickness(temp->getThickness());
						  g->setLinePattern(rec->getLinePattern());
						  g->setFull_pattern(rec->getFull_pattern());
						  g->move(50, 50);
						  POSITION p = temp_list.Find(temp);
						  temp_list.SetAt(p, (void*)g);
						  p = Glist.Find(temp);
						  delete(temp);
						  Glist.RemoveAt(p);
						  gobj = NULL;
						  break;
		}
		case ELLIPSE:
		{
						GEllipse* ell = (GEllipse*)temp;

						selectedlist.RemoveAll();
						GEllipse* g = new GEllipse();
						g->set(temp->getPoint().x, temp->getPoint().y, temp->getPoint_end().x, temp->getPoint_end().y);
						g->setColor(temp->getColor());
						g->setFull_color(ell->getFull_color());
						g->setThickness(temp->getThickness());
						g->setLinePattern(ell->getLinePattern());
						g->setFull_pattern(ell->getFull_pattern());
						g->move(50, 50);
						POSITION p = temp_list.Find(temp);
						temp_list.SetAt(p, (void*)g);
						p = Glist.Find(temp);
						delete(temp);
						Glist.RemoveAt(p);
						gobj = NULL;
						break;
		}
		case TEXT:
		{
					 GText* txt = (GText*)temp;

					 selectedlist.RemoveAll();
					 GText* g = new GText();
					 g->set(temp->getPoint().x, temp->getPoint().y, temp->getPoint_end().x, temp->getPoint_end().y);
					 g->setColor(temp->getColor());
					 g->setFont(txt->getFontName());
					 g->setSize(txt->getSize());
					 g->setItalic(txt->getItalic());
					 g->setUnderline(txt->getUnderline());
					 g->move(50, 50);
					 POSITION p = temp_list.Find(temp);
					 temp_list.SetAt(p, (void*)g);
					 p = Glist.Find(temp);
					 delete(temp);
					 Glist.RemoveAt(p);
					 gobj = NULL;
					 break;
		}
		}
	}

		CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
		p->Invalidate();
}



void CGEditorDoc::OnEditCopy()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
    temp_list.RemoveAll();
	if (!selectedlist.IsEmpty()){
		POSITION pos = selectedlist.GetHeadPosition();
		while (pos){
			GObject* temp = (GObject*)(selectedlist.GetNext(pos));
			temp_list.AddTail((void*)temp);
		}
	}
	else if (selectedlist.IsEmpty())
	{
		return;
	}
	else
		temp_list.AddTail((void*)gobj);
	POSITION pos =temp_list.GetHeadPosition();
	while (pos){
		GObject* temp = (GObject*)(temp_list.GetNext(pos));

		switch (temp->type())
		{
		case LINE:
		{
					 GLine* lin = (GLine*)temp;

					 selectedlist.RemoveAll();
					 GLine* g = new GLine();
					 g->set(lin->getPoint().x, lin->getPoint().y, lin->getPoint_end().x, lin->getPoint_end().y);
					 g->setColor(lin->getColor());
					 g->setThickness(lin->getThickness());
					 g->setLinePattern(lin->getLinePattern());
					 g->move(50, 50);
					 POSITION p = temp_list.Find(temp);
					 temp_list.SetAt(p, (void*)g);
					 break;
		}
		case POLYLINE:
		{
						 GPolyline* plin = (GPolyline*)temp;
						 
						 selectedlist.RemoveAll();
						 GPolyline* g = new GPolyline();
						 g->set(plin->getPoint().x, plin->getPoint().y, plin->getPoint_end().x, plin->getPoint_end().y);
						 g->setColor(plin->getColor());
						 g->setThickness(plin->getThickness());
						 g->setLinePattern(plin->getLinePattern());
						 g->move(50, 50);
						 POSITION p = temp_list.Find(temp);
						 temp_list.SetAt(p, (void*)g);
						 break;
		}
		case RECTANGLE:
		{
						  GRectangle* rec = (GRectangle*)temp;

						  selectedlist.RemoveAll();
						  GRectangle* g = new GRectangle();
						  g->set(rec->getPoint().x, rec->getPoint().y, rec->getPoint_end().x, rec->getPoint_end().y);
						  g->setColor(rec->getColor());
						  g->setFull_color(rec->getFull_color());
						  g->setThickness(rec->getThickness());
						  g->setLinePattern(rec->getLinePattern());
						  g->setFull_pattern(rec->getFull_pattern());
						  g->move(50, 50);
						  POSITION p = temp_list.Find(temp);
						  temp_list.SetAt(p, (void*)g);
						  break;
		}
		case ELLIPSE:
		{
						GEllipse* ell = (GEllipse*)temp;

						selectedlist.RemoveAll();
						GEllipse* g = new GEllipse();
						g->set(temp->getPoint().x, temp->getPoint().y, temp->getPoint_end().x, temp->getPoint_end().y);
						g->setColor(temp->getColor());
						g->setFull_color(ell->getFull_color());
						g->setThickness(temp->getThickness());
						g->setLinePattern(ell->getLinePattern());
						g->setFull_pattern(ell->getFull_pattern());
						g->move(50, 50);
						POSITION p = temp_list.Find(temp);
						temp_list.SetAt(p, (void*)g);
						break;
		}
		case TEXT:
		{
					 GText* txt = (GText*)temp;

					 selectedlist.RemoveAll();
					 GText* g = new GText();
					 g->set(temp->getPoint().x, temp->getPoint().y, temp->getPoint_end().x, temp->getPoint_end().y);
					 g->setColor(temp->getColor());
					 g->setFont(txt->getFontName());
					 g->setSize(txt->getSize());
					 g->setItalic(txt->getItalic());
					 g->setUnderline(txt->getUnderline());
					 g->move(50, 50);
					 POSITION p = temp_list.Find(temp);
					 temp_list.SetAt(p, (void*)g);
					 break;
		}
		}
	}
}


void CGEditorDoc::OnEditPaste()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	POSITION pos = temp_list.GetHeadPosition();
	while (pos){
		GObject* temp = (GObject*)(temp_list.GetNext(pos));
		Glist.AddTail((void*)temp);
	}
	CMainFrame* p = (CMainFrame*)AfxGetMainWnd();
	p->Invalidate();
}
