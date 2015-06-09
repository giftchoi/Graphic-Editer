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

// Graphic_EditDoc.cpp : CGraphic_EditDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Graphic_Edit.h"
#endif

#include "Graphic_EditDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGraphic_EditDoc

IMPLEMENT_DYNCREATE(CGraphic_EditDoc, CDocument)

BEGIN_MESSAGE_MAP(CGraphic_EditDoc, CDocument)
END_MESSAGE_MAP()


// CGraphic_EditDoc 생성/소멸

CGraphic_EditDoc::CGraphic_EditDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	gobj_list.RemoveAll();
	current_gobj.RemoveAll();

	m_current_type = SELECTED;							//현재 타입을 알기 위해서
	m_LineColor = RGB(255, 128, 0);						//기본은 파랑색으로
	m_rgncolor = RGB(130, 190, 250);
	m_Line_Pattern = 0;
	m_rgnpattern = 0;
	m_Bold = 1;
	m_GroupIDUsed = 0;

	m_bSelect_Object = FALSE;
	m_bDrawing = FALSE;
	m_bPolyDraw = FALSE;

	m_str.RemoveAll();
	buffer_gobj.RemoveAll();

	::ZeroMemory(&m_lf, sizeof(m_lf));

	m_lf.lfHeight = -19;
	m_lf.lfWidth = 0;
	m_lf.lfEscapement = 0;
	m_lf.lfOrientation = 0;
	m_lf.lfWeight = 400;
	m_lf.lfItalic = 0;
	m_lf.lfUnderline = 0;
	m_lf.lfStrikeOut = 0;
	m_lf.lfCharSet = 129;
	m_lf.lfOutPrecision = 3;
	m_lf.lfClipPrecision = 2;
	m_lf.lfQuality = 1;
	m_lf.lfPitchAndFamily = 18;
	wsprintf(m_lf.lfFaceName, TEXT("%s"), TEXT("휴먼옛체"));

}

CGraphic_EditDoc::~CGraphic_EditDoc()
{
}

BOOL CGraphic_EditDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.
	gobj_list.RemoveAll();
	current_gobj.RemoveAll();
	m_str.RemoveAll();
	buffer_gobj.RemoveAll();

	m_GroupIDUsed = 0;

	m_bSelect_Object = FALSE;
	m_bDrawing = FALSE;

	return TRUE;
}




// CGraphic_EditDoc serialization

void CGraphic_EditDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
		GObject* gobj;
		POSITION pos;

		ar << gobj_list.GetCount();

		pos = gobj_list.GetHeadPosition();

		while (pos)
		{
			gobj = (GObject*)gobj_list.GetNext(pos);
			gobj->setSelected(FALSE);
			ar << gobj->getObjectType();
			gobj->Serialize(ar);
		}
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
		gobj_list.RemoveAll();
		current_gobj.RemoveAll();
		buffer_gobj.RemoveAll();

		GObject* gobj;
		int count;
		int ObjectType;

		ar >> count;
		
		for (int i = 0; i < count; i++)
		{
			ar >> ObjectType;

			switch (ObjectType)
			{
			case LINE:
			{
						 gobj = new LineG();
						 break;
			}
			case RECTANGLE:
			{
						gobj = new RectangleG();
						break;
			}
			case ELLIPSE:
			{
						 gobj = new EllipseG();
						 break;
			}
			case POLYLINE:
			{
						 gobj = new PolylineG();
						 break;
			}
			case TEXT:
			{
						 gobj = new TextG();
						 break;
			}
			}
			gobj->Serialize(ar);
			gobj_list.AddTail(gobj);
		}

	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CGraphic_EditDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CGraphic_EditDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CGraphic_EditDoc::SetSearchContent(const CString& value)
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

// CGraphic_EditDoc 진단

#ifdef _DEBUG
void CGraphic_EditDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGraphic_EditDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGraphic_EditDoc 명령


void CGraphic_EditDoc::SetCurItemAdd(CPtrList* Item)
{
	GObject* new_gobj;

	POSITION NewPos = Item->GetHeadPosition();
	while (NewPos)
	{
		new_gobj = (GObject*)Item->GetNext(NewPos);			// element 가져와서 

		GObject* curr_obj;
		POSITION currPos = current_gobj.GetHeadPosition();

		bool isDuplication = false;

		while (currPos)
		{
			curr_obj = (GObject*)current_gobj.GetNext(currPos);			// element 가져와서 
			if (curr_obj == new_gobj)
			{
				isDuplication = true;
			}
		}

		if (!isDuplication)
		{
			current_gobj.AddTail(new_gobj);
		}
	}
}


void CGraphic_EditDoc::clearSelect(CPoint p)
{
	POSITION pos = current_gobj.GetHeadPosition();
	GObject* gobj;
	BOOL isBreak = FALSE;

	while (pos)
	{
		gobj = (GObject*)current_gobj.GetNext(pos);

		if (gobj->pointInRgn(p))			//만약 포인트가 있으면
		{
			isBreak = TRUE;
			break;
		}
	}

	if (isBreak)
	{
		return;
	}


	//그룹이 선택되어 있을때 그 영역을 선택한다면 지우면 안된다
	CRgn* rgb;

	for (int i = 0; i < m_GroupIDUsed; i++)
	{
		rgb = m_groupSet.getRgn(m_GroupID[i]);
		if (rgb && rgb->PtInRegion(p))
		{
			isBreak = TRUE;
			break;
		}
	}

	if (isBreak)
	{
		return;
	}


	pos = current_gobj.GetHeadPosition();

	while (pos)
	{
		gobj = (GObject*)current_gobj.GetNext(pos);
		gobj->setSelected(FALSE);
	}

	m_GroupIDUsed = 0;

	current_gobj.RemoveAll();
}


void CGraphic_EditDoc::setItemToBuffer()
{
	if (!buffer_gobj.IsEmpty())
	{
		POSITION deletePos = buffer_gobj.GetHeadPosition();
		GObject* gobj;

		while (deletePos)
		{
			gobj = (GObject*)buffer_gobj.GetNext(deletePos);
			delete gobj;
		}

		buffer_gobj.RemoveAll();
	}

	POSITION pos = current_gobj.GetHeadPosition();

	while (pos)
	{
		GObject* gobj = (GObject*)current_gobj.GetNext(pos);

		switch (gobj->getObjectType())
		{
		case LINE:
		{

			LineG* g = new LineG((LineG*)gobj);
			buffer_gobj.AddTail(g);
			break;
		}
		case ELLIPSE:
		{
			EllipseG* g = new EllipseG((EllipseG*)gobj);
			buffer_gobj.AddTail(g);
			break;
		}
		case POLYLINE:
		{
			PolylineG* g = new PolylineG((PolylineG*)gobj);
			buffer_gobj.AddTail(g);
			break;
		}
		case TEXT:
		{
			TextG* g = new TextG((TextG*)gobj);
			buffer_gobj.AddTail(g);
			break;
		}
		}
	}
}