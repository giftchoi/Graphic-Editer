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
	GType type() { return gtype; }
	void virtual setpoint(int left, int top, int right, int bottom) { }
	void setpoint(int right, int bottom){ setpoint(point.x, point.y, right, bottom); }

	bool virtual pointInrgn(CPoint p);
	void virtual draw(CDC*) { }
	void virtual move(int dx, int dy) { }

	void setColor(COLORREF c) { color = c; }
	COLORREF getColor() { return color; }

	void setBold(int t) { m_Bold = t; }
	int getBold(){return m_Bold;}

	void setLinePattern(int lp) { linepattern = lp; }
	int getLinePattern() { return linepattern; }

	void virtual drawBoundary(CDC*) {}

	void setSelected() { selected = true; }
	void resetSelected() { selected = false; }
	bool isSelected() { return selected; }

	CRect virtual getBoundary() { return 0; };

	CPoint getPoint() const { return point; }
	CPoint getPoint_end() const { return point_end; }

	void virtual serialize(CArchive& ar,bool serialize_flag){}
	void serialize_P(CArchive& ar, bool serialize_flag);

	int getmovemode() { return movemode; }
public:
	GType gtype;
	CPoint point;
	CPoint point_end;

	int linepattern;
	int m_Bold;
	COLORREF color;

	bool selected;
	int movemode;
//	virtual void Serialize(CArchive& ar);
};


