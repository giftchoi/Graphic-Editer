#pragma once

// GObject command target
#include "stdafx.h"

enum GType
{
	SELECTED,
	LINE,
	POLYLINE,
	RECTANGLE,
	ELLIPSE,
	TEXT
};


class GObject : public CObject
{
public:
	GObject();
	virtual ~GObject();
	GType type() { return gobj_type; }
	void virtual set(int left, int top, int right, int bottom) { }
	void set(int right, int bottom) { set(point.x, point.y, right, bottom); }

	bool virtual isin(CPoint p);
	void virtual draw(CDC*) { }
	void virtual move(int dx, int dy) { }

	void setColor(COLORREF c) { color = c; }
	COLORREF getColor() { return color; }

	void setThickness(int t) { thickness = t; }
	int getThickness() { return thickness; }

	void setLinePattern(int lp) { linepattern = lp; }
	int getLinePattern() { return linepattern; }

	void virtual drawBoundary(CDC*) {}

	void setSelected() { selected = true; }
	void resetSelected() { selected = false; }
	bool isSelected() { return selected; }

	CRect virtual getBoundary() { return 0; };

	CPoint getPoint() const { return point; }
	CPoint getPoint_end() const { return point_end; }

	void virtual serialize(CArchive& ar, bool serialize_flag){}
	void serialize_P(CArchive& ar, bool serialize_flag);

	int getmovemode() { return movemode; }
public:
	GType gobj_type;
	CPoint point;
	CPoint point_end;

	int linepattern;
	int thickness;
	COLORREF color;

	bool selected;
	int movemode;
	virtual void Serialize(CArchive& ar);
};


