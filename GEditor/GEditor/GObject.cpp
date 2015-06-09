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



void GObject::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	
		int temp = gobj_type;
		ar << temp << m_Bold << color << point << movemode << selected;
		
	}
	else
	{	
		ar >> m_Bold >> color >> point >> movemode >> selected;
	}
}
