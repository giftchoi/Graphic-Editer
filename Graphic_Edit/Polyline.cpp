// Polyline.cpp : implementation file
//

#include "stdafx.h"
#include "Graphic_Edit.h"
#include "Polyline.h"


// Polyline

Polyline::Polyline()
{
}

Polyline::~Polyline()
{
}


// Polyline member functions


void Polyline::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
	}
	else
	{	// loading code
	}
}


void Polyline::setPoint(int left, int top, int right, int bottom)
{
}


void Polyline::move(int dx, int dy)
{
}


void Polyline::addTail(CPoint point)
{
	polylist.AddTail(point);
	m_StartPoint = point;
}
