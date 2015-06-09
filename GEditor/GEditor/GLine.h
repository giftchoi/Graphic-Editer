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
	void virtual set(int left, int top, int right, int bottom);
	void virtual move(int dx, int dy);

	CRect virtual getBoundary();
	void virtual drawBoundary(CDC*);

	bool virtual isin(CPoint p);

	virtual void Serialize(CArchive& ar);
};


