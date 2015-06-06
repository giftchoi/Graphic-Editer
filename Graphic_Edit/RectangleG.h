#pragma once
#include "GObject.h"

// Rectangle command target

class RectangleG : public GObject
{
public:
	RectangleG();
	RectangleG(const RectangleG* g);
	virtual ~RectangleG();
	virtual void Serialize(CArchive& ar);
	
	COLORREF getRgnColor(){ return m_rgncolor; }
	void setRgnColor(COLORREF color){ m_rgncolor = color; }
	void virtual setLinePattern(int pattern){ m_linePattern = pattern; }
	void setRgnPattern(int pattern){ m_rgnpattern = pattern; }
	void setThickness(int Thickness) { m_Bold = Thickness; }
	
	void virtual draw(CDC*);

	void virtual SetRgn();
	BOOL virtual pointInRgn(CPoint point);
	virtual void setPoint(int left, int top, int right, int bottom);
	virtual void move(int dx, int dy);
	virtual void DrawPoint(CDC* pdc);
public:
	COLORREF m_rgncolor;
	int m_rgnpattern;
	int m_Line_Pattern;
	CRect selectedRect[8];
	virtual void AssertValid() const;

	virtual void SelectPoint(CPoint p = CPoint(-1, -1));
};


