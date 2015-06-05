// Polyline.cpp : implementation file
//

#include "stdafx.h"
#include "Graphic_Edit.h"
#include "PolylineG.h"
#include "GDIPLUS.h"

using namespace Gdiplus;

// Polyline

PolylineG::PolylineG()
{
	type = POLYLINE;
	polylist.RemoveAll();
	drawing = true;
	m_ID = -1;
}

PolylineG::PolylineG(const PolylineG* g)
{
	type = g->type;
	m_Bold = g->m_Bold;
	m_LineColor = g->m_LineColor;
	m_OriginPoint = g->m_OriginPoint;
	m_StartPoint = g->m_StartPoint;
	m_EndPoint = g->m_EndPoint;
	m_Alpha = g->m_Alpha;
	m_rgn.CreateRectRgn(0, 0, 0, 0);
	m_rgn.CopyRgn(&g->m_rgn);
	m_selected = FALSE;
	m_ID = -1;

	//�ڱⲨ
	POSITION pos = g->polylist.GetHeadPosition();
	polylist.RemoveAll();

	while (pos)
	{
		CPoint* point = new CPoint(polylist.GetNext(pos));
		polylist.AddTail(*point);
	}

	m_Line_Pattern = g->m_Line_Pattern;
	drawing = g->drawing;
}


PolylineG::~PolylineG()
{
}


// Polyline member functions


void PolylineG::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	
		ar << polylist.GetCount();

		POSITION pos = polylist.GetHeadPosition();

		while (pos)
		{
			CPoint point = polylist.GetNext(pos);
			ar << point;
		}
		ar << m_Line_Pattern << drawing;
	}
	else
	{	
		int polyListCount;
		ar >> polyListCount;

		for (int i = 0; i < polyListCount; i++)
		{
			CPoint point;
			ar >> point;
			polylist.AddTail(point);
		}
		ar >> m_Line_Pattern >> drawing;
		SetRgn();
	}
}


void PolylineG::setPoint(int left, int top, int right, int bottom)
{
	m_OriginPoint.x = left;
	m_OriginPoint.y = top;

	m_StartPoint.x = left;
	m_StartPoint.y = top;
	m_EndPoint.x = right;
	m_EndPoint.y = bottom;
}


void PolylineG::move(int dx, int dy)
{
	POSITION p = polylist.GetHeadPosition();

	if (m_selectedIndex == -1)
	{
		while (p)
		{
			CPoint& curr_point = polylist.GetNext(p);
			curr_point.x += dx;
			curr_point.y += dy;
		}

		m_EndPoint.x += dx;
		m_EndPoint.y += dy;
	}
	else
	{
		int idx = 0;
		while (p)
		{
			CPoint& curr_point = polylist.GetNext(p);

			if (m_selectedIndex == idx)
			{
				curr_point.x += dx;
				curr_point.y += dy;
				break;
			}

			idx++;
		}
	}
}


void PolylineG::addTail(CPoint point)
{
	polylist.AddTail(point);
	m_StartPoint = point;
}


void PolylineG::draw(CDC* cdc)
{
	CPoint S_point, E_point;

	Graphics graphics(*cdc);
	graphics.SetSmoothingMode(SmoothingModeHighQuality);

	Pen pen(Color(m_Alpha, GetRValue(m_LineColor), GetGValue(m_LineColor), GetBValue(m_LineColor)), (float)(m_Bold + 1));
	pen.SetDashStyle((DashStyle)m_Line_Pattern);
	POSITION p = polylist.GetHeadPosition();

	Point* arrPoint;
	int arrPointUsed = 0;
	int arrPointMaxUsed = 0;;

	if (drawing)
	{
		arrPointMaxUsed = polylist.GetCount() + 1;
		arrPoint = new Point[polylist.GetCount() + 1];
		arrPoint[polylist.GetCount()].X = m_EndPoint.x;
		arrPoint[polylist.GetCount()].Y = m_EndPoint.y;
	}
	else
	{
		arrPointMaxUsed = polylist.GetCount();
		arrPoint = new Point[polylist.GetCount()];
	}

	while (p)
	{
		CPoint tPoint;
		tPoint = polylist.GetNext(p);

		arrPoint[arrPointUsed].X = tPoint.x;
		arrPoint[arrPointUsed++].Y = tPoint.y;
	}

	pen.SetLineJoin(LineJoinRound);
	graphics.DrawLines(&pen, arrPoint, arrPointMaxUsed);

	delete[] arrPoint;

	if (m_selected)
	{
		CPoint tSpoint;
		CPoint tEpoint;

		CPoint pointLeftTop, pointRightBottom;

		POSITION p = polylist.GetHeadPosition();

		pointLeftTop.x = 99999;
		pointLeftTop.y = 99999;
		pointRightBottom.x = -99999;
		pointRightBottom.y = -99999;


		while (p)
		{
			CPoint& dst = polylist.GetNext(p);

			if (pointLeftTop.x > dst.x)				{ pointLeftTop.x = dst.x; }
			if (pointRightBottom.x < dst.x)          { pointRightBottom.x = dst.x; }

			if (pointLeftTop.y > dst.y)              { pointLeftTop.y = dst.y; }
			if (pointRightBottom.y < dst.y)          { pointRightBottom.y = dst.y; }
		}

		m_StartPoint.x = pointLeftTop.x;
		m_StartPoint.y = pointLeftTop.y;
		m_EndPoint.x = pointRightBottom.x;
		m_EndPoint.y = pointRightBottom.y;



		tSpoint.x = static_cast<int>(m_StartPoint.x - (m_Bold * 0.9) + 0.5);
		tSpoint.y = static_cast<int>(m_StartPoint.y - (m_Bold * 0.9) + 0.5);
		tEpoint.x = static_cast<int>(m_EndPoint.x + (m_Bold * 0.9) + 0.5);
		tEpoint.y = static_cast<int>(m_EndPoint.y + (m_Bold * 0.9) + 0.5);

		CPen pen(PS_DOT, 1, BLACK_PEN);
		cdc->SelectObject(pen);

		cdc->MoveTo(tSpoint.x - 5, tSpoint.y - 5);
		cdc->LineTo(tEpoint.x + 5, tSpoint.y - 5);
		cdc->MoveTo(tEpoint.x + 5, tSpoint.y - 5);
		cdc->LineTo(tEpoint.x + 5, tEpoint.y + 5);
		cdc->MoveTo(tEpoint.x + 5, tEpoint.y + 5);
		cdc->LineTo(tSpoint.x - 5, tEpoint.y + 5);
		cdc->MoveTo(tSpoint.x - 5, tEpoint.y + 5);
		cdc->LineTo(tSpoint.x - 5, tSpoint.y - 5);
	}

}

CRect PolylineG::getBoundary()
{
	CRect crect;

	crect.left = m_StartPoint.x < m_EndPoint.x ? m_StartPoint.x : m_EndPoint.x;
	crect.top = m_StartPoint.y < m_EndPoint.y ? m_StartPoint.y : m_EndPoint.y;
	crect.right = m_StartPoint.x >= m_EndPoint.x ? m_StartPoint.x : m_EndPoint.x;
	crect.bottom = m_StartPoint.y >= m_EndPoint.y ? m_StartPoint.y : m_EndPoint.y;

	return crect;
}

void PolylineG::SetRgn()
{
	drawing = false;

	m_rgn.DeleteObject();

	CPoint pointLeftTop, pointRightBottom;

	POSITION p = polylist.GetHeadPosition();

	pointLeftTop.x = 99999;
	pointLeftTop.y = 99999;
	pointRightBottom.x = -99999;
	pointRightBottom.y = -99999;


	while (p)
	{
		CPoint& dst = polylist.GetNext(p);

		if (pointLeftTop.x > dst.x)  			{ pointLeftTop.x = dst.x; }
		if (pointRightBottom.x < dst.x)          { pointRightBottom.x = dst.x; }

		if (pointLeftTop.y > dst.y)              { pointLeftTop.y = dst.y; }
		if (pointRightBottom.y < dst.y)          { pointRightBottom.y = dst.y; }
	}

	m_StartPoint.x = pointLeftTop.x;
	m_StartPoint.y = pointLeftTop.y;
	m_EndPoint.x = pointRightBottom.x;
	m_EndPoint.y = pointRightBottom.y;

	m_rgn.CreateRectRgn(pointLeftTop.x - 5, pointLeftTop.y - 5, pointRightBottom.x + 5, pointRightBottom.y + 5);
}

BOOL PolylineG::pointInRgn(CPoint point)
{
	return m_rgn.PtInRegion(point);
}

void PolylineG::selectPoint(CPoint point)
{

	selectedRect = new CRect[polylist.GetCount()];

	POSITION pos = polylist.GetHeadPosition();
	int i = 0;
	while (pos)
	{
		CPoint p = polylist.GetNext(pos);

		selectedRect[i].left = p.x - 5;
		selectedRect[i].top = p.y - 5;
		selectedRect[i].right = p.x + 5;
		selectedRect[i].bottom = p.y + 5;

		i++;
	}

	m_selectedIndex = -1;

	for (int i = 0; i < polylist.GetCount(); i++)
	{
		if (selectedRect[i].PtInRect(point))
		{
			m_selectedIndex = i;
			break;
		}
	}

	delete[] selectedRect;
}

void PolylineG::DrawPoint(CDC* pdc)
{
	selectedRect = new CRect[polylist.GetCount()];

	POSITION pos = polylist.GetHeadPosition();
	int i = 0;
	while (pos)
	{
		CPoint p = polylist.GetNext(pos);

		selectedRect[i].left = p.x - 5;
		selectedRect[i].top = p.y - 5;
		selectedRect[i].right = p.x + 5;
		selectedRect[i].bottom = p.y + 5;

		i++;
	}

	for (int i = 0; i < polylist.GetCount(); i++)
	{
		CPen pen(PS_SOLID, 1, RGB(0, 0, 255));
		pdc->SelectObject(pen);
		pdc->SelectStockObject(WHITE_BRUSH);
		pdc->Ellipse(selectedRect[i]);
	}

	delete[] selectedRect;
}

BOOL PolylineG::isPointSelected()
{
	if (m_selectedIndex != -1 && polylist.GetCount() > 2)
	{
		return TRUE;
	}

	return FALSE;

}

void  PolylineG::deletePoint()
{
	POSITION pos = polylist.GetHeadPosition();
	int idx = 0;

	while (idx<m_selectedIndex)
	{
		CPoint& curr_point = polylist.GetNext(pos);
		idx++;
	}

	polylist.RemoveAt(pos);

	selectPoint();
}