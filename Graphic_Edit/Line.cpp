// Line.cpp : implementation file
//

#include "stdafx.h"
#include "Graphic_Edit.h"
#include "Line.h"


// Line

Line::Line()
{
	type = LINE;
	m_ID = -1;
}

Line::~Line()
{
}


// Line member functions


void Line::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
	}
	else
	{	// loading code
	}
}


void Line::setPoint(int left, int top, int right, int bottom)
{
	m_OriginPoint.x = left;
	m_OriginPoint.y = top;
	m_StartPoint.x = left;
	m_StartPoint.y = top;
	m_EndPoint.x = right;
	m_EndPoint.y = bottom;
}


void Line::move(int dx, int dy)
{
	switch (m_selectedIndex){
	case -1:{
		m_OriginPoint.x += dx;
		m_OriginPoint.y += dy;
		m_EndPoint.x += dx;
		m_EndPoint.y += dy;
		break;
	}
	case 0:{
		m_OriginPoint.x += dx;
		m_OriginPoint.y += dy;
		break;
	}
	case 1:{
		m_EndPoint.x += dx;
		m_EndPoint.y += dy;
		break;
	}
	}

}


void Line::SetRgn()
{
	CRect rect;
	CPoint Spoint;
	CPoint Epoint;

	if (m_OriginPoint.x > m_EndPoint.x)
	{
		Spoint.x = m_EndPoint.x;
		Epoint.x = m_OriginPoint.x;
	}
	else
	{
		Epoint.x = m_EndPoint.x;
		Spoint.x = m_OriginPoint.x;
	}

	if (m_OriginPoint.y > m_EndPoint.y)
	{
		Spoint.y = m_EndPoint.y;
		Epoint.y = m_OriginPoint.y;
	}
	else
	{
		Epoint.y = m_EndPoint.y;
		Spoint.y = m_OriginPoint.y;
	}

	rect.SetRect(Spoint, Epoint);

	rect.left = static_cast<int>(rect.left - (m_Bold* 0.9) - 5);
	rect.top = static_cast<int>(rect.top - (m_Bold * 0.9) - 5);
	rect.right = static_cast<int>(rect.right + (m_Bold * 0.9) + 5);
	rect.bottom = static_cast<int>(rect.bottom + (m_Bold * 0.9) + 5);

	m_StartPoint.x = rect.left;
	m_StartPoint.y = rect.top;

	m_rgn.DeleteObject();
	m_rgn.CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
}


BOOL Line::pointInRgn(CPoint point)
{
	if (m_rgn.PtInRegion(point))
	{
		return true;
	}

	return false;
}
