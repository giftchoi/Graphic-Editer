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
	GObject::Serialize(ar);
	
	if (ar.IsStoring())
	{
		ar << m_rgncolor << m_linePattern << m_rgnpattern;
	}
	else
	{
		ar >> m_rgncolor >> m_linePattern >> m_rgnpattern;
		setRgn();
	}
}


void Rectangle::AssertValid() const
{
	GObject::AssertValid();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}


void Rectangle::setPoint(int left, int top, int right, int bottom)
{
	m_OriginPoint.x = left;
	m_OriginPoint.y = top;

	if (left < right)
	{
		m_StartPoint.x = left;
		m_EndPoint.x = right;
	}
	else
	{
		m_StartPoint.x = right;
		m_EndPoint.x = left;
	}

	if (top < bottom)
	{
		m_StartPoint.y = top;
		m_EndPoint.y = bottom;
	}
	else
	{
		m_StartPoint.y = bottom;
		m_EndPoint.y = top;
	}
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

void SetRgn()
{
	pointSwap();
	m_rgn.DeleteObject();
	m_rgn.CreateRectRgn(static_cast<int>(m_StartPoint.x - (m_Bold * 0.9) - 4.5), static_cast<int>(m_StartPoint.y - (m_Bold* 0.9) - 4.5),
		static_cast<int>(m_EndPoint.x + (m_Bold * 0.9) + 4.5), static_cast<int>(m_EndPoint.y + (m_Bold* 0.9) + 4.5));
}

void Rectangle::SelectPoint(CPoint p)
{
	m_selectedIndex = -1;

	for (int i = 0; i < 8; i++)
	{
		if (selectedRect[i].PtInRect(p))
		{
			m_selectedIndex = i;
			break;
		}
	}
}
