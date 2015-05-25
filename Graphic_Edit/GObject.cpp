// GObject.cpp : implementation file
//

#include "stdafx.h"
#include "Graphic_Edit.h"
#include "GObject.h"


// GObject

GObject::GObject()
{
	m_Bold = 1;
	m_ID = 0;
	m_LineColor = RGB(0, 0, 0);
}

GObject::~GObject()
{
}


// GObject member functions


void GObject::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
	}
	else
	{	// loading code
	}
}


void GObject::setPoint(int left, int top, int right, int bottom)
{
}


void GObject::setPoint(int right, int bottom)
{
	setPoint(m_OriginPoint.x, m_OriginPoint.y, right, bottom);
}


void GObject::setPoint(CPoint point)
{
	m_StartPoint = point;
}


void GObject::move(int dx, int dy)
{
}


CPoint GObject::getPoint()
{
	return m_StartPoint;
}


CPoint GObject::getEndPoint()
{
	return m_EndPoint;
}


void GObject::setEndPoint(CPoint point)
{
	m_EndPoint = point;
}
