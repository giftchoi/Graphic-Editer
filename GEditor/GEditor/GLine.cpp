// GLine.cpp : implementation file
//

#include "stdafx.h"
#include "GEditor.h"
#include "GLine.h"


// GLine

GLine::GLine()
{
	gtype = LINE;
}

GLine::~GLine()
{
}

void GLine::set(int left, int top, int right, int bottom)
{
	point.x = left;
	point.y = top;
	point_end.x = right;
	point_end.y = bottom;
}



void GLine::draw(CDC* cdc)
{
	CPen pen(linepattern, m_Bold, color);
	CPen *oldpen = cdc->SelectObject(&pen);

	cdc->MoveTo(point);
	cdc->LineTo(point_end);

	cdc->SelectObject(&oldpen);
}

CRect GLine::getBoundary(){
	CRect crect;

	crect.left = point.x < point_end.x ? point.x : point_end.x;
	crect.top = point.y < point_end.y ? point.y : point_end.y;
	crect.right = point.x >= point_end.x ? point.x : point_end.x;
	crect.bottom = point.y >= point_end.y ? point.y : point_end.y;

	return crect;
}

void GLine::move(int dx, int dy)
{
	if (movemode == 0)
	{
		point.x += dx;
		point.y += dy;
		point_end.x += dx;
		point_end.y += dy;
	}

	else if (movemode == 1)
	{
		point.x += dx;
		point.y += dy;
	}
	else if (movemode == 2)
	{
		point_end.x += dx;
		point_end.y += dy;
	}
}

bool GLine::isin(CPoint p)
{
	CRect rect = getBoundary();

	if (rect.PtInRect(p)){
		movemode = 0;
		return true;
	}

	else if (CRect(point.x - 5, point.y - 5, point.x + 5, point.y + 5).PtInRect(p))
	{
		movemode = 1;
		return true;
	}

	else if (CRect(point_end.x - 5, point_end.y - 5, point_end.x + 5, point_end.y + 5).PtInRect(p))
	{
		movemode = 2;
		return true;
	}
	else
		return false;

}

void GLine::drawBoundary(CDC* cdc)
{
	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen *oldpen = cdc->SelectObject(&pen);

	cdc->SelectStockObject(WHITE_BRUSH);

	CPoint p;

	p = point;
	CRect rect(p.x - 5, p.y - 5, p.x + 5, p.y + 5);
	cdc->Rectangle(rect);

	p = point_end;
	rect.SetRect(p.x - 5, p.y - 5, p.x + 5, p.y + 5);
	cdc->Rectangle(rect);

	cdc->SelectObject(&oldpen);
}


void GLine::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	
		ar << point_end << linepattern;
	}
	else
	{	
		ar >> point_end >> linepattern;
	}
}
