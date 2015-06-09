// GRectangle.cpp : implementation file
//

#include "stdafx.h"
#include "GEditor.h"
#include "GRectangle.h"


// GRectangle

GRectangle::GRectangle()
{
	full_color = RGB(0, 0, 0);
	gobj_type = RECTANGLE;
	linepattern = PS_SOLID;
	full_pattern = 6;
	movemode = 0;
}

GRectangle::~GRectangle()
{
}


// GRectangle member functions

CRect GRectangle::getBoundary()
{
	CRect crect;

	crect.left = point.x < point_end.x ? point.x : point_end.x;
	crect.top = point.y < point_end.y ? point.y : point_end.y;
	crect.right = point.x >= point_end.x ? point.x : point_end.x;
	crect.bottom = point.y >= point_end.y ? point.y : point_end.y;

	return crect;
}

void GRectangle::drawBoundary(CDC* cdc)
{
	CRect rect = getBoundary();
	CPen pen(PS_DOT, 1, RGB(0, 0, 0));

	cdc->SelectObject(&pen);
	cdc->SelectStockObject(NULL_BRUSH);
	cdc->Rectangle(rect);

	cdc->SelectStockObject(WHITE_BRUSH);
	CRect box(rect.BottomRight().x - 5, rect.BottomRight().y - 5, rect.BottomRight().x + 5, rect.BottomRight().y + 5);
	cdc->Rectangle(box);

	box.SetRect(rect.TopLeft().x - 5, rect.TopLeft().y - 5, rect.TopLeft().x + 5, rect.TopLeft().y + 5);
	cdc->Rectangle(box);

	box.SetRect(rect.left - 5, rect.bottom - 5, rect.left + 5, rect.bottom + 5);
	cdc->Rectangle(box);

	box.SetRect(rect.right - 5, rect.top - 5, rect.right + 5, rect.top + 5);
	cdc->Rectangle(box);

	box.SetRect(rect.CenterPoint().x - 5, rect.bottom - 5, rect.CenterPoint().x + 5, rect.bottom + 5);
	cdc->Rectangle(box);

	box.SetRect(rect.CenterPoint().x - 5, rect.top - 5, rect.CenterPoint().x + 5, rect.top + 5);
	cdc->Rectangle(box);

	box.SetRect(rect.right - 5, rect.CenterPoint().y - 5, rect.right + 5, rect.CenterPoint().y + 5);
	cdc->Rectangle(box);

	box.SetRect(rect.left - 5, rect.CenterPoint().y - 5, rect.left + 5, rect.CenterPoint().y + 5);
	cdc->Rectangle(box);
}

void GRectangle::set(int left, int top, int right, int bottom)
{
	point.x = left;
	point.y = top;
	point_end.x = right;
	point_end.y = bottom;
}

void GRectangle::move(int dx, int dy)
{
	switch (movemode){
	case 0:
		point.x += dx;
		point.y += dy;
		point_end.x += dx;
		point_end.y += dy;
		break;
	case 1:
		point.x += dx;
		point.y += dy;
		break;
	case 2:
		point.y += dy;
		break;
	case 3:
		point.y += dy;
		point_end.x += dx;
		break;
	case 4:
		point_end.x += dx;
		break;
	case 5:
		point_end.x += dx;
		point_end.y += dy;
		break;
	case 6:
		point_end.y += dy;
		break;
	case 7:
		point.x += dx;
		point_end.y += dy;
		break;
	case 8:
		point.x += dx;
		break;
	}
}
bool GRectangle::isin(CPoint p){
	CRect rect = getBoundary();

	point = rect.TopLeft();
	point_end = rect.BottomRight();

	if (rect.left - p.x < 5 && rect.left - p.x > -5){
		if (rect.top - p.y <5 && rect.top - p.y> -5)
			movemode = 1;
		else if (rect.bottom - p.y <5 && rect.bottom - p.y>-5)
			movemode = 7;
		else
			movemode = 8;

		return true;
	}
	else if (rect.right - p.x < 5 && rect.right - p.x >-5){
		if (rect.top - p.y <5 && rect.top - p.y> -5)
			movemode = 3;
		else if (rect.bottom - p.y <5 && rect.bottom - p.y>-5)
			movemode = 5;
		else
			movemode = 4;
		return true;
	}
	else{
		if (rect.top - p.y <5 && rect.top - p.y> -5)
		{
			movemode = 2;
			return true;
		}
		else if (rect.bottom - p.y <5 && rect.bottom - p.y>-5)
		{
			movemode = 6;
			return true;
		}
		else if (CRect(getBoundary()).PtInRect(p))
		{
			movemode = 0;
			return true;
		}
		else
			return false;
	}
}

void GRectangle::draw(CDC* cdc)
{
	CRect rect;

	rect.left = point.x;
	rect.top = point.y;
	rect.right = point_end.x;
	rect.bottom = point_end.y;

	CPen pen(linepattern, thickness, color);
	CPen *oldpen = cdc->SelectObject(&pen);
	CBrush brush(full_pattern, full_color);
	CBrush *oldbrush = cdc->SelectObject(&brush);

	cdc->Rectangle(&rect);

	cdc->SelectObject(&oldpen);
	cdc->SelectObject(&oldbrush);
}

void GRectangle::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	
		ar << point_end << linepattern << full_pattern << full_color;
	}
	else
	{	
		ar >> point_end >> linepattern >> full_pattern >> full_color;
	}
}
