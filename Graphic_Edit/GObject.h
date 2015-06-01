#pragma once

// GObject command target

enum Type { SELECTED, LINE, POLYLINE , ELLIPSE, RECTANGLE, TEXT, GROUPING};


class GObject : public CObject
{
public:
	GObject();
	virtual ~GObject();
	virtual void Serialize(CArchive& ar);
	
	void virtual draw(CDC*) { }
	virtual void setPoint(int left, int top, int right, int bottom);
	void setPoint(int right, int bottom);
	void setPoint(CPoint point);
	virtual void move(int dx, int dy);
	virtual void SetRgn(){}
	CPoint getPoint();
	void pointSwap();
	void setEndPoint(CPoint point);
	
	void virtual selectPoint(CPoint point = CPoint(-1, -1)) { }
	virtual BOOL pointInRgn(CPoint point){ return FALSE; }
	virtual void DrawPoint(CDC* dc){}
	void virtual setLinePattern(int pattern){ m_linePattern = pattern; }
	void setSelected(BOOL selected) { m_selected = selected; }
	BYTE getSelected() const { return m_selected; }
	Type getObjectType() { return type; }
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
};
