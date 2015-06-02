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

// Graphic_EditDoc.h : CGraphic_EditDoc Ŭ������ �������̽�
//


#pragma once

#include "GObject.h"
//#include "GroupSet.h"

#include "GObject.h"
#include "EllipseG.h"
#include "LineG.h"
#include "PolylineG.h"
#include "RectangleG.h"
#include "TextG.h"
#include "Group.h"

class CGraphic_EditDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CGraphic_EditDoc();
	DECLARE_DYNCREATE(CGraphic_EditDoc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:
	CPtrList gobj_list;							//Graphic object�� �����س��� ����
	CPtrList current_gobj;						//���� ���õ� GObject ��ü
	CPtrList buffer_gobj;						//���� ���õ� GObject ��ü
	Group m_groupSet;						//�׷켼Ʈ Ŭ����
	Type m_current_type;				//���� Ÿ���� �˱� ���ؼ�

	CPoint m_SPoint;							//������
	CPoint m_EPoint;							//����

	COLORREF m_LineColor;						//���ٿ��� ���õ� ������ �־� ���´�. ��ü�� ���鶧 ����Ѵ�.
	COLORREF m_Brush_Color;						//�귯�� ����
	int m_Line_Pattern;
	int m_Brush_Pattern;
	int m_Bold;

	LOGFONT m_lf;								//font ����
	COLORREF m_FontColor;
	CArray<TCHAR, TCHAR> m_str;

	BOOL m_bSelect_Object;						//���õǾ��� Ȯ���ϱ� ���ؼ�
	BOOL m_bDrawing;							//���õǾ��� Ȯ���ϱ� ���ؼ�
	BOOL m_bPolyDraw;

	int m_GroupID[100];
	int m_GroupIDUsed;
// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// �����Դϴ�.
public:
	virtual ~CGraphic_EditDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// �˻� ó���⿡ ���� �˻� �������� �����ϴ� ����� �Լ�
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	void SetCurItemAdd(CPtrList* Item);
	void clearSelect(CPoint p);
	void setItemToBuffer();
};
