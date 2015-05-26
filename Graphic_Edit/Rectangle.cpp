// Rectangle.cpp : implementation file
//

#include "stdafx.h"
#include "Graphic_Edit.h"
#include "Rectangle.h"


// Rectangle

Rectangle::Rectangle()
{
	m_rgnpattern = 0;
}

Rectangle::~Rectangle()
{
}


// Rectangle member functions


void Rectangle::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
	}
	else
	{	// loading code
	}
}


void Rectangle::AssertValid() const
{
	GObject::AssertValid();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}


void Rectangle::setPoint(int left, int top, int right, int bottom)
{
}


void Rectangle::move(int dx, int dy)
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
