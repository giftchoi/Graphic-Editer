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
	void virtual setLinePattern(int Pattern) { m_linePattern = Pattern; }
	virtual void setPoint(int left, int top, int right, int bottom);
	virtual void move(int dx,  int dy);



public:
	CList<CPoint, POINT> polylist;
	CRect* selectedRect;
	
	void addTail(CPoint point);
};


