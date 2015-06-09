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
	void virtual set(int left, int top, int right, int bottom);
	void virtual move(int dx, int dy);

	CRect virtual getBoundary();
	// void virtual serialize(CArchive& ar, bool serialize_flag);

	void setFull_color(COLORREF c) { full_color = c; }
	COLORREF getFull_color(){ return full_color; }

	void setFull_pattern(int n){ full_pattern = n; }
	bool beenIn(CRect rect) { if (rect.PtInRect(point) && rect.PtInRect(point_end))return true; else return false; }

	CPoint getPoint_end(){ return point_end; }
	int getFull_pattern() { return full_pattern; }

	void virtual drawBoundary(CDC*);
	bool virtual isin(CPoint p);
public:
	int full_pattern;
	COLORREF full_color;
	virtual void Serialize(CArchive& ar);
};


