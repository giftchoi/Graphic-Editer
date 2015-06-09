#pragma once

#include "stdafx.h"
#include "GObject.h"
// GLine command target

class GLine : public GObject
{
public:
	GLine();
	virtual ~GLine();
	void virtual draw(CDC*);
	void virtual setpoint(int left, int top, int right, int bottom);
	void virtual move(int dx, int dy);

	CRect virtual getBoundary();
	void virtual drawBoundary(CDC*);

	bool virtual pointInrgn(CPoint p);

	virtual void serialize(CArchive& ar, bool serialize_flag);
};


