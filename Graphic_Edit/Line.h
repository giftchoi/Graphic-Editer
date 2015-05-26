#pragma once
#include "GObject.h"
// Line command target

class Line : public GObject
{
public:
	Line();
	virtual ~Line();
	virtual void Serialize(CArchive& ar);
	void virtual setLinePattern(int Pattern) { m_linePattern = Pattern; }
	virtual void setPoint(int left, int top, int right, int bottom);
	virtual void move(int dx, int dy);

public:
	CRect selectedIndex[2];
	
	
};


