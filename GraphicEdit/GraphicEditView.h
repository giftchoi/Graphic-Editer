
// GraphicEditView.h : CGraphicEditView Ŭ������ �������̽�
//

#pragma once



class CGraphicEditView : public CView
{
protected: // serialization������ ��������ϴ�.
	CGraphicEditView();
	DECLARE_DYNCREATE(CGraphicEditView)

// Ư���Դϴ�.
public:
	CGraphicEditDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// �����Դϴ�.
public:
	virtual ~CGraphicEditView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // GraphicEditView.cpp�� ����� ����
inline CGraphicEditDoc* CGraphicEditView::GetDocument() const
   { return reinterpret_cast<CGraphicEditDoc*>(m_pDocument); }
#endif

