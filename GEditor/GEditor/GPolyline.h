#pragma once

#include "stdafx.h"
#include "GObject.h"
// GPolyline command target

class GPolyline : public GObject
{
public:
	GPolyline();
	virtual ~GPolyline();
	void virtual draw(CDC*);
	void virtual move(int dx, int dy);
	void virtual setpoint(int left, int top, int right, int bottom);

	void drawBoundary(CDC*);
	CRect virtual getBoundary();

	bool virtual pointInrgn(CPoint p);

	void setArr();
	void complete();

	void onePointdel();
public:
	CArray<CPoint, CPoint&> pt_arr;
	int pt_num;
	virtual void serialize(CArchive& ar, bool serialize_flag);
};


