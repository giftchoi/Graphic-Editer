#pragma once
#include "GObject.h"

// Rectangle command target

class Rectangle : public GObject
{
public:
	Rectangle();
	Rectangle(const Rectangle* g);
	virtual ~Rectangle();
	virtual void Serialize(CArchive& ar);
	COLORREF getRgnColor(){ return m_rgncolor; }
	void setRgnColor(COLORREF color){ m_rgncolor = color; }
	void virtual setLinePattern(int pattern){ m_linePattern = pattern; }
	void setRgnPattern(int pattern){ m_rgnpattern = pattern; }
	
	void virtual SetRgn();
	BOOL virtual pointInRgn(CPoint point);
	virtual void setPoint(int left, int top, int right, int bottom);
	virtual void move(int dx, int dy);
	virtual void DrawPoint(CDC* pdc);
public:
	COLORREF m_rgncolor;
	int m_rgnpattern;
	CRect selectedRect[8];
	virtual void AssertValid() const;

	virtual void SelectPoint(CPoint p = CPoint(-1, -1));
};


