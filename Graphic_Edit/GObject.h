#pragma once

// GObject command target

enum Type { SELECTED, LINE, POLYLINE , ELLIPSE, RECTANGLE, TEXT, GROUPING};


class GObject : public CObject
{
public:
	GObject();
	virtual ~GObject();
	virtual void Serialize(CArchive& ar);
	
	virtual void setPoint(int left, int top, int right, int bottom);
	void setPoint(int right, int bottom);
	void setPoint(CPoint point);
	virtual void move(int dx, int dy);
	CPoint getPoint();

	void virtual setLinePattern(int pattern){ m_linePattern = pattern; }

public:
	Type type;
	int m_Bold;
	COLORREF m_LineColor;
	CPoint m_StartPoint;
	CPoint m_EndPoint;
	CPoint m_OriginPoint;
	CRgn m_rgn;
	BOOL m_selected;
	int m_ID;
	int m_selectedIndex;
	int m_linePattern;

	CPoint getEndPoint();
	void setEndPoint(CPoint point);

};
