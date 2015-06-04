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

// Graphic_EditView.h : CGraphic_EditView 클래스의 인터페이스
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
protected: // serialization에서만 만들어집니다.
	CGraphic_EditView();
	DECLARE_DYNCREATE(CGraphic_EditView)

// 특성입니다.
public:
	CGraphic_EditDoc* GetDocument() const;
	
	void Draw(CDC* pDC);

	Type m_current_type;
	LOGFONT m_lf;								//font 정보
	COLORREF m_FontColor;

	BOOL m_Menu_SELECT;
	BOOL m_Menu_LINE;
	BOOL m_Menu_POLYLINE;
	BOOL m_Menu_ELLIPSE;
	BOOL m_Menu_RECTANGLE;
	BOOL m_Menu_TEXT;
// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 구현입니다.
public:
	virtual ~CGraphic_EditView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
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

#ifndef _DEBUG  // Graphic_EditView.cpp의 디버그 버전
inline CGraphic_EditDoc* CGraphic_EditView::GetDocument() const
   { return reinterpret_cast<CGraphic_EditDoc*>(m_pDocument); }
#endif

