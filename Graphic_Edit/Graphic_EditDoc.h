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

// Graphic_EditDoc.h : CGraphic_EditDoc 클래스의 인터페이스
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
protected: // serialization에서만 만들어집니다.
	CGraphic_EditDoc();
	DECLARE_DYNCREATE(CGraphic_EditDoc)

// 특성입니다.
public:

// 작업입니다.
public:
	CPtrList gobj_list;							//Graphic object를 저장해놓는 벡터
	CPtrList current_gobj;						//현재 선택된 GObject 객체
	CPtrList buffer_gobj;						//현재 선택된 GObject 객체
	Group m_groupSet;						//그룹세트 클래스
	Type m_current_type;				//현재 타입을 알기 위해서

	CPoint m_SPoint;							//시작점
	CPoint m_EPoint;							//끝점

	COLORREF m_LineColor;						//툴바에서 선택된 선택을 넣어 놓는다. 객체를 만들때 사용한다.
	COLORREF m_Brush_Color;						//브러쉬 색깔
	int m_Line_Pattern;
	int m_Brush_Pattern;
	int m_Bold;

	LOGFONT m_lf;								//font 정보
	COLORREF m_FontColor;
	CArray<TCHAR, TCHAR> m_str;

	BOOL m_bSelect_Object;						//선택되었나 확인하기 위해서
	BOOL m_bDrawing;							//선택되었나 확인하기 위해서
	BOOL m_bPolyDraw;

	int m_GroupID[100];
	int m_GroupIDUsed;
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
	virtual ~CGraphic_EditDoc();
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
	void SetCurItemAdd(CPtrList* Item);
	void clearSelect(CPoint p);
	void setItemToBuffer();
};
