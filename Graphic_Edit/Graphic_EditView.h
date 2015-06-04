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

// Graphic_EditView.h : CGraphic_EditView Ŭ������ �������̽�
//


#pragma once
#include "GObject.h"
#include "LineG.h"
#include "PolylineG.h"
#include "EllipseG.h"
#include "RectangleG.h"
#include "TextG.h"
#include "Graphic_EditDoc.h"
class CGraphic_EditView : public CView
{
protected: // serialization������ ��������ϴ�.
	CGraphic_EditView();
	DECLARE_DYNCREATE(CGraphic_EditView)

// Ư���Դϴ�.
public:
	CGraphic_EditDoc* GetDocument() const;
	
	void Draw(CDC* pDC);

	Type m_current_type;
	LOGFONT m_lf;								//font ����
	COLORREF m_FontColor;

	BOOL m_Menu_SELECT;
	BOOL m_Menu_LINE;
	BOOL m_Menu_POLYLINE;
	BOOL m_Menu_ELLIPSE;
	BOOL m_Menu_RECTANGLE;
	BOOL m_Menu_TEXT;
// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// �����Դϴ�.
public:
	virtual ~CGraphic_EditView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSelected();
	afx_msg void OnLine();
	afx_msg void OnPolyline();
	afx_msg void OnEllipse();
	afx_msg void OnRectangle();
	afx_msg void OnText();
	bool m_IsSelect;
	bool m_IsLine;
	bool m_IsPolyline;
	bool m_IsEllipse;
	bool m_IsRectangle;
	bool m_IsText;
	Type m_current_type;
};

#ifndef _DEBUG  // Graphic_EditView.cpp�� ����� ����
inline CGraphic_EditDoc* CGraphic_EditView::GetDocument() const
   { return reinterpret_cast<CGraphic_EditDoc*>(m_pDocument); }
#endif

