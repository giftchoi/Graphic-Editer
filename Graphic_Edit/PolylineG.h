#pragma once
#include "GObject.h"

// Polyline command target

class PolylineG: public GObject
{
public:
	PolylineG();
	PolylineG(const PolylineG* pline);
	virtual ~PolylineG();
	virtual void Serialize(CArchive& ar);
	
	void setThickness(int Thickness) { m_Bold = Thickness; }
	void virtual setLinePattern(int Pattern) { m_linePattern = Pattern; }
	virtual void setPoint(int left, int top, int right, int bottom);
	virtual void move(int dx,  int dy);

	void virtual draw(CDC*);
	CRect virtual getBoundary();

	void virtual SetRgn();
	BOOL virtual pointInRgn(CPoint point);

	void virtual selectPoint(CPoint point = CPoint(-1, -1));
	void virtual DrawPoint(CDC* pdc);
	BOOL isPointSelected();
	void deletePoint();

public:
	CList<CPoint, POINT> polylist;
	CRect* selectedRect;
	int m_Line_Pattern;
	bool drawing;

	void addTail(CPoint point);
};


