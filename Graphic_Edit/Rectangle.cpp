// Rectangle.cpp : implementation file
//

#include "stdafx.h"
#include "Graphic_Edit.h"
#include "Rectangle.h"


// Rectangle

Rectangle::Rectangle()
{
	type = RECTANGLE;
	m_rgnpattern = 0;
	m_ID = -1;
}

Rectangle::Rectangle(const Rectangle* g)
{
	//정보를 옮김
	//오브젝트꺼
	type = g->type;
	m_Bold = g->m_Bold;
	m_LineColor = g->m_LineColor;
	m_OriginPoint = g->m_OriginPoint;
	m_StartPoint = g->m_StartPoint;
	m_EndPoint = g->m_EndPoint;
	m_rgn.CreateRectRgn(0, 0, 0, 0);
	m_rgn.CopyRgn(&g->m_rgn);
	//m_Alpha = g->m_Alpha;
	m_selected = FALSE;
	m_ID = -1;

	//객체 자신꺼
	m_rgncolor = g->m_rgncolor;
	m_linePattern = g->m_linePattern;
	m_rgnpattern = g->m_rgnpattern;
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
		SetRgn();
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

void Rectangle::SetRgn()
{
	pointSwap();
	m_rgn.DeleteObject();
	m_rgn.CreateRectRgn(static_cast<int>(m_StartPoint.x - (m_Bold * 0.9) - 4.5), static_cast<int>(m_StartPoint.y - (m_Bold* 0.9) - 4.5),
		static_cast<int>(m_EndPoint.x + (m_Bold * 0.9) + 4.5), static_cast<int>(m_EndPoint.y + (m_Bold* 0.9) + 4.5));
}

BOOL Rectangle::pointInRgn(CPoint point)
{
	if (m_rgn.PtInRegion(point))
	{
		return true;
	}

	return false;
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

void Rectangle::DrawPoint(CDC* pdc)
{
	CPoint t_Spoint;
	CPoint t_Epoint;
	t_Spoint.x = static_cast<int>(m_StartPoint.x - (m_Bold * 0.9) + 0.5);
	t_Spoint.y = static_cast<int>(m_StartPoint.y - (m_Bold * 0.9) + 0.5);
	t_Epoint.x = static_cast<int>(m_EndPoint.x + (m_Bold * 0.9) + 0.5);
	t_Epoint.y = static_cast<int>(m_EndPoint.y + (m_Bold * 0.9) + 0.5);

	CRect rect(t_Spoint, t_Epoint);

	selectedRect[0].SetRect(t_Spoint.x - 5, t_Spoint.y - 5,
		t_Spoint.x + 5, t_Spoint.y + 5);

	selectedRect[1].SetRect(rect.CenterPoint().x - 5, t_Spoint.y - 5,
		rect.CenterPoint().x + 5, t_Spoint.y + 5);

	selectedRect[2].SetRect(t_Epoint.x - 5, t_Spoint.y - 5,
		t_Epoint.x + 5, t_Spoint.y + 5);


	selectedRect[3].SetRect(t_Epoint.x - 5, rect.CenterPoint().y - 5,
		t_Epoint.x + 5, rect.CenterPoint().y + 5);


	selectedRect[4].SetRect(t_Epoint.x - 5, t_Epoint.y - 5,
		t_Epoint.x + 5, t_Epoint.y + 5);

	selectedRect[5].SetRect(rect.CenterPoint().x - 5, t_Epoint.y - 5,
		rect.CenterPoint().x + 5, t_Epoint.y + 5);

	selectedRect[6].SetRect(t_Spoint.x - 5, t_Epoint.y - 5,
		t_Spoint.x + 5, t_Epoint.y + 5);


	selectedRect[7].SetRect(t_Spoint.x - 5, rect.CenterPoint().y - 5,
		t_Spoint.x + 5, rect.CenterPoint().y + 5);

	for (int i = 0; i < 8; i++)
	{
		CPen pen(PS_SOLID, 1, RGB(0, 0, 255));
		pdc->SelectObject(pen);
		pdc->SelectStockObject(WHITE_BRUSH);
		pdc->Ellipse(selectedRect[i]);
	}
}