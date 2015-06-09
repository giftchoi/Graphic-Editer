// GObject.cpp : implementation file
//

#include "stdafx.h"
#include "GEditor.h"
#include "GObject.h"


// GObject

GObject::GObject()
{
	thickness = 5;
	color = RGB(0, 0, 255);
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
		ar << temp << thickness << color << point << movemode << selected;
		
	}
	else
	{	
		ar >> thickness >> color >> point >> movemode >> selected;
	}
}
