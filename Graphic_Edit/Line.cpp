// Line.cpp : implementation file
//

#include "stdafx.h"
#include "Graphic_Edit.h"
#include "Line.h"


// Line

Line::Line()
{
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
