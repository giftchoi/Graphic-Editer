#pragma once
#include "GObject.h"

// Ellipse command target

class Ellipse : public GObject
{
public:
	Ellipse();
	virtual ~Ellipse();
	virtual void Serialize(CArchive& ar);
	virtual COLORREF getRgncolor(){ return m_rgncolor; }
	virtual void setRgncolor(COLORREF color){ m_rgncolor = color; }

	void virtual setLinePattern(int pattern){ m_linePattern = pattern; }
	void setRgnPattern(int pattern){ m_rgnpattern = pattern; }
	
	void virtual SetRgn();
	virtual void setPoint(int left, int top, int right, int bottom);
	virtual void move(int dx, int dy);
	BOOL virtual pointInRgn(CPoint point);
	void virtual selectPoint(CPoint point = CPoint(-1, -1));
	void virtual DrawPoint(CDC* dc);
public:
	
	COLORREF m_rgncolor;
	int m_rgnpattern;
	CRect selectedRect[8];
	
};


