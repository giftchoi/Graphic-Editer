// GObject.cpp : implementation file
//

#include "stdafx.h"
#include "GEditor.h"
#include "GObject.h"


// GObject

GObject::GObject()
{
	m_Bold = 5;
	color = RGB(255, 255, 255);
	selected = false;
}

GObject::~GObject()
{
}


// GObject member functions

bool GObject::isin(CPoint p)
{
	CRect crect = getBoundary();

	if (crect.left <= p.x && p.x <= crect.right
		&& crect.top <= p.y && p.y <= crect.bottom)
		return true;
	else
		return false;
}




void GObject::serialize_P(CArchive& ar, bool serialize_flag)
{
	if (serialize_flag){
		int temp = gtype;
		ar << temp << m_Bold << color << point << movemode << selected;
		serialize_flag = true;
	}
	else {
		ar >> m_Bold >> color >> point >> movemode >> selected;
		serialize_flag = false;
	}
}