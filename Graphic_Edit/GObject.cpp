// GObject.cpp : implementation file
//

#include "stdafx.h"
#include "Graphic_Edit.h"
#include "GObject.h"


// GObject

GObject::GObject()
{
	m_Bold = 0;
	m_ID = 0;
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
}


void GObject::setPoint(CPoint point)
{
}


void GObject::move(int dx, int dy)
{
}


CPoint GObject::getPoint()
{
	return CPoint();
}
