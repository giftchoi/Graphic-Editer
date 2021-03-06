
// GEditorDoc.h : CGEditorDoc 클래스의 인터페이스
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
protected: // serialization에서만 만들어집니다.
	CGEditorDoc();
	DECLARE_DYNCREATE(CGEditorDoc)

// 특성입니다.
public:
	bool isEnabled;
	GObject* gobj;
	CPtrList Glist;
	GType type;
	CPoint point;
	CPtrList temp_list;

	int bold;
	COLORREF linecolor;
	COLORREF regioncolor;
	int linepattern;
	int regionpattern;

	// text
	bool isSlide,isUnderline;
	int size;
	CString font;
	COLORREF textcolor;

	CPtrList selectedlist;
// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CGEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
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
	afx_msg void OnLineYellow();
	afx_msg void OnLineBlack();
	afx_msg void OnLineWhite();
	afx_msg void OnRegionYellow();
	afx_msg void OnRegionBlack();
	afx_msg void OnRegionWhite();
	afx_msg void OnRegionPatternH();
	afx_msg void OnRegionPatternV();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
};
