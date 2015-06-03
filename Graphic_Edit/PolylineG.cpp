// Polyline.cpp : implementation file
//

#include "stdafx.h"
#include "Graphic_Edit.h"
#include "PolylineG.h"


// Polyline

PolylineG::PolylineG()
{
}

PolylineG::PolylineG(const PolylineG* pline)
{

}


PolylineG::~PolylineG()
{
}


// Polyline member functions


void PolylineG::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
	}
	else
	{	// loading code
	}
}


void PolylineG::setPoint(int left, int top, int right, int bottom)
{
}


void PolylineG::move(int dx, int dy)
{
}


void PolylineG::addTail(CPoint point)
{
	polylist.AddTail(point);
	m_StartPoint = point;
}
