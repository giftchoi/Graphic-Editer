
// GEditorDoc.h : CGEditorDoc Ŭ������ �������̽�
//


#pragma once

#include "GObject.h"
#include "MainFrm.h"
#include "GLine.h"
#include "GPolyline.h"
#include "GEllipse.h"
#include "GRectangle.h"
#include "GText.h"

class CGEditorDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CGEditorDoc();
	DECLARE_DYNCREATE(CGEditorDoc)

// Ư���Դϴ�.
public:
	bool gobj_context_menu_enabled;
	GObject* cur_gobj;
	CPtrList gobj_list;
	GType cur_type;
	CPoint point;
	CPtrList temp_list;

	int cur_thickness;
	COLORREF cur_color;
	COLORREF cur_fullcolor;
	int cur_linepattern;
	int cur_fullpattern;

	// text
	bool bItalic, bUnderline;
	int size;
	CString fontName;
	COLORREF text_color;


	CPtrList gobj_selected_list;
// �۾��Դϴ�.
public:

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
	virtual ~CGEditorDoc();
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
	afx_msg void OnLineRed();
	afx_msg void OnLineGreen();
	afx_msg void OnLineBlue();
	afx_msg void OnBold1();
	afx_msg void OnBold2();
	afx_msg void OnBold3();
	afx_msg void OnLinePattern();
	afx_msg void OnLinePattern2();
	afx_msg void OnLinePatternDot();
	afx_msg void OnRegionRed();
	afx_msg void OnRegionGreen();
	afx_msg void OnRegionBlue();
	afx_msg void OnRegionPattern();
	afx_msg void OnRegionPatternLine();
	afx_msg void OnRegionPatternCheck();
	afx_msg void OnFont1();
	afx_msg void OnFont2();
	afx_msg void OnFont3();
	afx_msg void OnFontColorRed();
	afx_msg void OnFontColorGreen();
	afx_msg void OnFontColorBlue();
	afx_msg void OnSize1();
	afx_msg void OnSize2();
	afx_msg void OnSize3();
	afx_msg void OnTextUnderline();
	afx_msg void OnTextSlide();
	afx_msg void OnSelected();
	afx_msg void OnLine();
	afx_msg void OnPolyline();
	afx_msg void OnEllipse();
	afx_msg void OnRetangle();
	afx_msg void OnText();
};
