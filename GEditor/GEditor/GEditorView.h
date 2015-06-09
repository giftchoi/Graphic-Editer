
// GEditorView.h : CGEditorView Ŭ������ �������̽�
//

#pragma once

#include "GObject.h"
#include "GLine.h"
#include "GPolyline.h"
#include "GRectangle.h"
#include "GEllipse.h"
#include "GText.h"

class CGEditorView : public CScrollView
{
protected: // serialization������ ��������ϴ�.
	CGEditorView();
	DECLARE_DYNCREATE(CGEditorView)

// Ư���Դϴ�.
public:
	CGEditorDoc* GetDocument() const;

// �۾��Դϴ�.
public:
	bool isdrawing;
	bool ismoving;
	bool isselect;
	bool m_ctrl;

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ���� �� ó�� ȣ��Ǿ����ϴ�.

// �����Դϴ�.
public:
	virtual ~CGEditorView();
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
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // GEditorView.cpp�� ����� ����
inline CGEditorDoc* CGEditorView::GetDocument() const
   { return reinterpret_cast<CGEditorDoc*>(m_pDocument); }
#endif

