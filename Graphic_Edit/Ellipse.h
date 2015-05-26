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

	void setLinePattern(int pattern){ m_linepattern = pattern; }
	void setRgnPattern(int pattern){ m_rgnpattern = pattern; }
	virtual void setPoint(int left, int top, int right, int bottom);
	virtual void move(int dx, int dy);
public:
	
	COLORREF m_rgncolor;
	int m_linepattern;
	int m_rgnpattern;
	CRect selectedRect[8];
	
};


