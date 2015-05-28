// Ellipse.cpp : implementation file
//

#include "stdafx.h"
#include "Graphic_Edit.h"
#include "Ellipse.h"


// Ellipse

Ellipse::Ellipse()
{
	type = ELLIPSE;
	m_ID = -1;
	m_rgnpattern = 0;
}

Ellipse::~Ellipse()
{
}


// Ellipse member functions


void Ellipse::Serialize(CArchive& ar)
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


void Ellipse::setPoint(int left, int top, int right, int bottom)
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

void Ellipse::SetRgn()
{
	pointSwap();
	m_rgn.DeleteObject();
	m_rgn.CreateRectRgn(static_cast<int>(m_StartPoint.x - (m_Bold * 0.9) - 4.5), static_cast<int>(m_StartPoint.y - (m_Bold* 0.9) - 4.5),
		static_cast<int>(m_EndPoint.x + (m_Bold * 0.9) + 4.5), static_cast<int>(m_EndPoint.y + (m_Bold* 0.9) + 4.5));
}

BOOL Ellipse::pointInRgn(CPoint point)
{
	if (m_rgn.PtInRegion(point))
	{
		return true;
	}

	return false;
}

void Ellipse::selectPoint(CPoint point)
{
	m_selectedIndex = -1;

	for (int i = 0; i < 8; i++)
	{
		if (selectedRect[i].PtInRect(point))
		{
			m_selectedIndex = i;
			break;
		}
	}
}

void Ellipse::DrawPoint(CDC* dc)
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
		dc->SelectObject(pen);
		dc->SelectStockObject(WHITE_BRUSH);
		dc->Ellipse(selectedRect[i]);
	}
}