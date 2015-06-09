// GPolyline.cpp : implementation file
//

#include "stdafx.h"
#include "GEditor.h"
#include "GPolyline.h"


// GPolyline

GPolyline::GPolyline()
{
	gtype = POLYLINE;

	pt_num = 0;
}

GPolyline::~GPolyline()
{
}


// GPolyline member functions

void GPolyline::setpoint(int left, int top, int right, int bottom)
{
	point.x = left;
	point.y = top;
	point_end.x = right;
	point_end.y = bottom;
}

void GPolyline::setArr(){

	pt_num++;

	pt_arr.SetSize(pt_num);
	pt_arr[pt_num - 1] = point;

}

void GPolyline::move(int dx, int dy)
{
	if (movemode == -1)
	{
		for (int i = 0; i<pt_arr.GetSize(); i++)
		{
			pt_arr[i].x += dx;
			pt_arr[i].y += dy;
		}
		point_end.x += dx;
		point_end.y += dy;
	}

	else if (0 <= movemode && movemode < pt_num)
	{
		pt_arr[movemode].x += dx;
		pt_arr[movemode].y += dy;
	}
	else if (movemode == pt_num){
		point_end.x += dx;
		point_end.y += dy;
	}
}

CRect GPolyline::getBoundary()
{
	CRect crect;

	int l, r, t, b;

	l = r = pt_arr[0].x;
	t = b = pt_arr[0].y;
	for (int i = 1; i<pt_arr.GetSize(); i++)
	{
		l = pt_arr[i].x < l ? pt_arr[i].x : l;
		r = pt_arr[i].x > r ? pt_arr[i].x : r;
		t = pt_arr[i].y < t ? pt_arr[i].y : t;
		b = pt_arr[i].y > b ? pt_arr[i].y : b;
	}

	l = point_end.x < l ? point_end.x : l;
	r = point_end.x > r ? point_end.x : r;
	t = point_end.y < t ? point_end.y : t;
	b = point_end.y > b ? point_end.y : b;

	crect.SetRect(l, t, r, b);

	return crect;
}

bool GPolyline::pointInrgn(CPoint p)
{
	for (int i = 0; i<pt_num; i++)
	{
		CRect rect(pt_arr[i].x - 5, pt_arr[i].y - 5, pt_arr[i].x + 5, pt_arr[i].y + 5);
		if (rect.PtInRect(p)){
			movemode = i;
			return true;
		}
	}

	if (CRect(point_end.x - 5, point_end.y - 5, point_end.x + 5, point_end.y + 5).PtInRect(p))
	{
		movemode = pt_num;
		return true;
	}

	else if (CRect(getBoundary()).PtInRect(p))
	{
		movemode = -1;
		return true;
	}
	else
		return false;
}

void GPolyline::complete()
{
	pt_num = pt_num - 1;

	pt_arr.SetSize(pt_num);
}

void GPolyline::draw(CDC* cdc)
{
	CPen pen(linepattern, m_Bold, color);
	CPen *oldpen = cdc->SelectObject(&pen);

	cdc->MoveTo(pt_arr[0]);

	for (int i = 1; i<pt_arr.GetSize(); i++)
		cdc->LineTo(pt_arr[i]);

	cdc->LineTo(point_end);

	cdc->SelectObject(&oldpen);
}


void GPolyline::drawBoundary(CDC* cdc)
{
	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen *oldpen = cdc->SelectObject(&pen);

	cdc->SelectStockObject(WHITE_BRUSH);

	for (int i = 0; i<pt_arr.GetSize(); i++)
	{
		CPoint p = pt_arr[i];
		if (movemode == i){
			CBrush brush(RGB(0, 0, 0));
			CBrush* oldbru = cdc->SelectObject(&brush);
			CRect rect(p.x - 5, p.y - 5, p.x + 5, p.y + 5);
			cdc->Rectangle(rect);
			cdc->SelectObject(oldbru);
		}
		else{
			CRect rect(p.x - 5, p.y - 5, p.x + 5, p.y + 5);
			cdc->Rectangle(rect);
		}
	}

	if (movemode == pt_num){
		CBrush brush(RGB(0, 0, 0));
		CBrush* oldbru = cdc->SelectObject(&brush);
		CRect rect(point_end.x - 5, point_end.y - 5, point_end.x + 5, point_end.y + 5);
		cdc->Rectangle(rect);
		cdc->SelectObject(oldbru);
	}
	else{
		CRect rect(point_end.x - 5, point_end.y - 5, point_end.x + 5, point_end.y + 5);
		cdc->Rectangle(rect);
	}

	cdc->SelectObject(&oldpen);
}

void GPolyline::onePointdel()
{
	if (movemode >= 0 && movemode <= pt_num){
		pt_arr.RemoveAt(movemode);
		pt_num--;
	}
}

void GPolyline::serialize(CArchive& ar, bool serialize_flag)
{
	serialize_P(ar, serialize_flag);
	if (serialize_flag)
	{	
		ar << point_end << linepattern << pt_num;
		pt_arr.Serialize(ar);
	}
	else
	{	
		ar >> point_end >> linepattern >> pt_num;
		pt_arr.Serialize(ar);
	}
}
