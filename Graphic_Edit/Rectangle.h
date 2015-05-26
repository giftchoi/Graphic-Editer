#pragma once
#include "GObject.h"

// Rectangle command target

class Rectangle : public GObject
{
public:
	Rectangle();
	virtual ~Rectangle();
	virtual void Serialize(CArchive& ar);
	COLORREF getRgnColor(){ return m_rgncolor; }
	void setRgnColor(COLORREF color){ m_rgncolor = color; }
	void virtual setLinePattern(int pattern){ m_linePattern = pattern; }
	void setRgnPattern(int pattern){ m_rgnpattern = pattern; }

	virtual void setPoint(int left, int top, int right, int bottom);
	virtual void move(int dx, int dy);

public:
	COLORREF m_rgncolor;
	int m_rgnpattern;
	CRect selectedRect[8];
	virtual void AssertValid() const;

};


