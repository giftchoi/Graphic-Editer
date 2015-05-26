// Ellipse.cpp : implementation file
//

#include "stdafx.h"
#include "Graphic_Edit.h"
#include "Ellipse.h"


// Ellipse

Ellipse::Ellipse()
{
	m_rgnpattern = 0;
}

Ellipse::~Ellipse()
{
}


// Ellipse member functions


void Ellipse::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
	}
	else
	{	// loading code
	}
}


void Ellipse::setPoint(int left, int top, int right, int bottom)
{

}


void Ellipse::move(int dx, int dy)
{
	switch (m_selectedIndex){
	case -1:{
				m_StartPoint.x += dx;
				m_StartPoint.y += dy;
				m_EndPoint.x += dx;
				m_EndPoint.y += dy;
				break;
		}
	case 0:{
			   m_StartPoint.x += dx;
			   m_StartPoint.y += dy;
				break;
		}
	case 1:{
			   m_StartPoint.y += dy;
			   break;
		}
	case 2:{
			   m_StartPoint.y += dy;
			   m_EndPoint.x += dx;
			   break;
		}
	case 3:{
			   m_EndPoint.x += dx;
			   break;
		}
	case 4:{
			   m_EndPoint.x += dx;
			   m_EndPoint.y += dy;
				break;
		}
	case 5:{
			   m_EndPoint.y += dy;
				break;
		}
	case 6:{
			   m_StartPoint.x += dx;
			   m_EndPoint.y += dy;
				break;
		}
	case 7:{
			   m_StartPoint.x += dx;
			   break;
		}
	}
}
