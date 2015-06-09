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

// Graphic_EditDoc.cpp : CGraphic_EditDoc Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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


// CGraphic_EditDoc ����/�Ҹ�

CGraphic_EditDoc::CGraphic_EditDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.
	gobj_list.RemoveAll();
	current_gobj.RemoveAll();

	m_current_type = SELECTED;							//���� Ÿ���� �˱� ���ؼ�
	m_LineColor = RGB(255, 128, 0);						//�⺻�� �Ķ�������
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
	wsprintf(m_lf.lfFaceName, TEXT("%s"), TEXT("�޸տ�ü"));

}

CGraphic_EditDoc::~CGraphic_EditDoc()
{
}

BOOL CGraphic_EditDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.
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
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
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
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
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

// ����� �׸��� �����մϴ�.
void CGraphic_EditDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CGraphic_EditDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ������ �����Ϳ��� �˻� �������� �����մϴ�.
	// ������ �κ��� ";"�� ���еǾ�� �մϴ�.

	// ��: strSearchContent = _T("point;rectangle;circle;ole object;");
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

// CGraphic_EditDoc ����

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


// CGraphic_EditDoc ���


void CGraphic_EditDoc::SetCurItemAdd(CPtrList* Item)
{
	GObject* new_gobj;

	POSITION NewPos = Item->GetHeadPosition();
	while (NewPos)
	{
		new_gobj = (GObject*)Item->GetNext(NewPos);			// element �����ͼ� 

		GObject* curr_obj;
		POSITION currPos = current_gobj.GetHeadPosition();

		bool isDuplication = false;

		while (currPos)
		{
			curr_obj = (GObject*)current_gobj.GetNext(currPos);			// element �����ͼ� 
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

		if (gobj->pointInRgn(p))			//���� ����Ʈ�� ������
		{
			isBreak = TRUE;
			break;
		}
	}

	if (isBreak)
	{
		return;
	}


	//�׷��� ���õǾ� ������ �� ������ �����Ѵٸ� ����� �ȵȴ�
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