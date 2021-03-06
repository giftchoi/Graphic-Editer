#pragma once


#include "stdafx.h"
#include "GObject.h"
// GRectangle command target

class GRectangle : public GObject
{
public:
	GRectangle();
	virtual ~GRectangle();
	void virtual draw(CDC*);
	void virtual setpoint(int left, int top, int right, int bottom);
	void virtual move(int dx, int dy);

	CRect virtual getBoundary();
	// void virtual serialize(CArchive& ar, bool serialize_flag);

	void setFull_color(COLORREF c) { regioncolor = c; }
	COLORREF getFull_color(){ return regioncolor; }

	void setFull_pattern(int n){ regionpattern = n; }
	bool beenIn(CRect rect) { if (rect.PtInRect(point) && rect.PtInRect(point_end))return true; else return false; }

	CPoint getPoint_end(){ return point_end; }
	int getFull_pattern() { return regionpattern; }

	void virtual drawBoundary(CDC*);
	bool virtual pointInrgn(CPoint p);
public:
	int regionpattern;
	COLORREF regioncolor;
	virtual void serialize(CArchive& ar, bool serialize_flag);
};


