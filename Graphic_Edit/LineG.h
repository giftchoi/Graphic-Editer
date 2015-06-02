#pragma once
#include "GObject.h"
// Line command target

class LineG : public GObject
{
public:
	LineG();
	LineG(const LineG* pline);
	virtual ~LineG();
	virtual void Serialize(CArchive& ar);
	void virtual setLinePattern(int Pattern) { m_linePattern = Pattern; }
	virtual void setPoint(int left, int top, int right, int bottom);
	virtual void move(int dx, int dy);
	void selectPoint(CPoint point);
	void DrawPoint(CDC* pdc);
	void virtual SetRgn();
	BOOL virtual pointInRgn(CPoint point);

//	virtual void DrawPoint(CDC* pdc);
public:
	CRect selectedIndex[2];
	
	
};


