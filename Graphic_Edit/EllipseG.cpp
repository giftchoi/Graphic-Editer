// Ellipse.cpp : implementation file
//

#include "stdafx.h"
#include "Graphic_Edit.h"
#include "EllipseG.h"


// Ellipse

EllipseG::EllipseG()
{
	type = ELLIPSE;
	m_ID = -1;
	m_rgnpattern = 0;
}

EllipseG::EllipseG(const EllipseG* g)
{
	type = g->type;
	m_Bold = g->m_Bold;
	m_LineColor = g->m_LineColor;
	m_OriginPoint = g->m_OriginPoint;
	m_StartPoint = g->m_StartPoint;
	m_EndPoint = g->m_EndPoint;
	m_rgn.CreateRectRgn(0, 0, 0, 0);
	m_rgn.CopyRgn(&g->m_rgn);
	m_selected = FALSE;
	m_ID = -1;

	//�ڱⲨ
	m_rgncolor = g->m_rgncolor;
	m_linePattern = g->m_linePattern;
	m_rgnpattern = g->m_rgnpattern;

}

EllipseG::~EllipseG()
{
}


// Ellipse member functions


void EllipseG::Serialize(CArchive& ar)
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


void EllipseG::setPoint(int left, int top, int right, int bottom)
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


void EllipseG::move(int dx, int dy)
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

void EllipseG::SetRgn()
{
	pointSwap();
	m_rgn.DeleteObject();
	m_rgn.CreateRectRgn(static_cast<int>(m_StartPoint.x - (m_Bold * 0.9) - 4.5), static_cast<int>(m_StartPoint.y - (m_Bold* 0.9) - 4.5),
		static_cast<int>(m_EndPoint.x + (m_Bold * 0.9) + 4.5), static_cast<int>(m_EndPoint.y + (m_Bold* 0.9) + 4.5));
}

BOOL EllipseG::pointInRgn(CPoint point)
{
	if (m_rgn.PtInRegion(point))
	{
		return true;
	}

	return false;
}

void EllipseG::selectPoint(CPoint point)
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

void EllipseG::DrawPoint(CDC* dc)
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

void EllipseG::draw(CDC* cdc)
{
	pointSwap();

	/*Graphics graphics(*cdc);
	graphics.SetSmoothingMode(SmoothingModeHighQuality);

	Point dPoint;
	dPoint.X = m_EPoint.x - m_SPoint.x;
	dPoint.Y = m_EPoint.y - m_SPoint.y;

	Pen pen(Color(m_Alpha, GetRValue(m_LineColor), GetGValue(m_LineColor), GetBValue(m_LineColor)), (float)(m_Thickness + 1));
	pen.SetDashStyle((DashStyle)m_Line_Pattern);

	graphics.DrawEllipse(&pen, static_cast<int>(m_SPoint.x - (m_Thickness * 0.45) + 0.5),
		static_cast<int>(m_SPoint.y - (m_Thickness * 0.45) + 0.5),
		static_cast<int>(dPoint.X + (m_Thickness * 0.8) + 0.5),
		static_cast<int>(dPoint.Y + (m_Thickness * 0.8) + 0.5));


	if (m_Brush_Pattern == 0)
	{
		SolidBrush solidBrush(Color(m_Alpha, GetRValue(m_Brush_Color), GetGValue(m_Brush_Color), GetBValue(m_Brush_Color)));
		graphics.FillEllipse(&solidBrush, m_SPoint.x, m_SPoint.y, dPoint.X, dPoint.Y);
	}
	else if (m_Brush_Pattern >= 2)
	{
		HatchBrush hatchBrush((HatchStyle)(m_Brush_Pattern - 2), Color(m_Alpha, GetRValue(m_Brush_Color), GetGValue(m_Brush_Color), GetBValue(m_Brush_Color)), Color::Transparent);
		graphics.FillEllipse(&hatchBrush, m_SPoint.x, m_SPoint.y, dPoint.X, dPoint.Y);
	}


	if (m_Selected)
	{
		CPoint tSpoint;
		CPoint tEpoint;
		tSpoint.x = static_cast<int>(m_SPoint.x - (m_Thickness * 0.9) + 0.5);
		tSpoint.y = static_cast<int>(m_SPoint.y - (m_Thickness * 0.9) + 0.5);
		tEpoint.x = static_cast<int>(m_EPoint.x + (m_Thickness * 0.9) + 0.5);
		tEpoint.y = static_cast<int>(m_EPoint.y + (m_Thickness * 0.9) + 0.5);

		CPen pen(PS_DOT, 1, BLACK_PEN);
		cdc->SelectObject(pen);



		cdc->MoveTo(tSpoint.x - 5, tSpoint.y - 5);
		cdc->LineTo(tEpoint.x + 5, tSpoint.y - 5);
		cdc->MoveTo(tEpoint.x + 5, tSpoint.y - 5);
		cdc->LineTo(tEpoint.x + 5, tEpoint.y + 5);
		cdc->MoveTo(tEpoint.x + 5, tEpoint.y + 5);
		cdc->LineTo(tSpoint.x - 5, tEpoint.y + 5);
		cdc->MoveTo(tSpoint.x - 5, tEpoint.y + 5);
		cdc->LineTo(tSpoint.x - 5, tSpoint.y - 5);
	}*/
}