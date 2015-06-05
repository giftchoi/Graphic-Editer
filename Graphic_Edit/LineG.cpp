// Line.cpp : implementation file
//

#include "stdafx.h"
#include "Graphic_Edit.h"
#include "LineG.h"
#include "GDIPLUS.h"

using namespace Gdiplus;

// Line

LineG::LineG()
{
	type = LINE;
	m_ID = -1;
}

LineG::LineG(const LineG* pline)
{
	//모든 정보를 옮겨야 한다.
	//오브젝트꺼
	type = pline->type;
	m_Bold = pline->m_Bold;
	m_LineColor = pline->m_LineColor;
	m_OriginPoint = pline->m_OriginPoint;
	m_StartPoint = pline->m_StartPoint;
	m_EndPoint = pline->m_EndPoint;
	m_rgn.CreateRectRgn(0, 0, 0, 0);
	m_rgn.CopyRgn(&pline->m_rgn);
	m_Alpha = pline->m_Alpha;
	m_selected = FALSE;
	m_ID = -1;                  
	m_linePattern = pline->m_linePattern;
}

LineG::~LineG()
{
}


// Line member functions


void LineG::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar << m_Line_Pattern;
	}
	else
	{	// loading code
		ar >> m_Line_Pattern;
		SetRgn();
	}
}

void LineG::draw(CDC* cdc)
{
	Graphics graphics(*cdc);
	graphics.SetSmoothingMode(SmoothingModeHighQuality);

	Pen pen(Color(m_Alpha, GetRValue(m_LineColor), GetGValue(m_LineColor), GetBValue(m_LineColor)), (float)(m_Bold + 1));
	pen.SetDashStyle((DashStyle)m_Line_Pattern);

	graphics.DrawLine(&pen, static_cast<float>(m_OriginPoint.x - (m_Bold * 0.45) + 0.5),
		static_cast<float>(m_OriginPoint.y - (m_Bold * 0.45) + 0.5),
		static_cast<float>(m_EndPoint.x + (m_Bold * 0.8) + 0.5),
		static_cast<float>(m_EndPoint.y + (m_Bold * 0.8) + 0.5));

	if (m_selected)
	{

		CRect rect;
		CPoint tSpoint;
		CPoint tEpoint;

		if (m_OriginPoint.x > m_EndPoint.x)
		{
			tSpoint.x = m_EndPoint.x;
			tEpoint.x = m_OriginPoint.x;
		}
		else
		{
			tEpoint.x = m_EndPoint.x;
			tSpoint.x = m_OriginPoint.x;
		}

		if (m_OriginPoint.y > m_EndPoint.y)
		{
			tSpoint.y = m_EndPoint.y;
			tEpoint.y = m_OriginPoint.y;
		}
		else
		{
			tEpoint.y = m_EndPoint.y;
			tSpoint.y = m_OriginPoint.y;
		}

		rect.SetRect(tSpoint, tEpoint);

		rect.left = static_cast<int>(rect.left - (m_Bold * 0.9) + 0.5);
		rect.top = static_cast<int>(rect.top - (m_Bold * 0.9) + 0.5);
		rect.right = static_cast<int>(rect.right + (m_Bold * 0.9) + 0.5);
		rect.bottom = static_cast<int>(rect.bottom + (m_Bold * 0.9) + 0.5);

		CPen pen(PS_DOT, 1, BLACK_PEN);
		cdc->SelectObject(pen);
		cdc->SelectStockObject(NULL_BRUSH);

		cdc->Rectangle(rect);
		/*
		cdc->MoveTo(rect.left - 5, rect.top - 5);
		cdc->LineTo(rect.right + 5, rect.bottom - 5);
		cdc->MoveTo(rect.right + 5, rect.bottom - 5);
		cdc->LineTo(rect.right + 5, rect.bottom + 5);
		cdc->MoveTo(rect.right + 5, rect.bottom + 5);
		cdc->LineTo(rect.left - 5, rect.top + 5);
		cdc->MoveTo(rect.left - 5, rect.top + 5);
		cdc->LineTo(rect.left - 5, rect.top - 5);
		*/
	}
}

void LineG::setPoint(int left, int top, int right, int bottom)
{
	m_OriginPoint.x = left;
	m_OriginPoint.y = top;
	m_StartPoint.x = left;
	m_StartPoint.y = top;
	m_EndPoint.x = right;
	m_EndPoint.y = bottom;
}


void LineG::move(int dx, int dy)
{
	switch (m_selectedIndex){
	case -1:{
		m_OriginPoint.x += dx;
		m_OriginPoint.y += dy;
		m_EndPoint.x += dx;
		m_EndPoint.y += dy;
		break;
	}
	case 0:{
		m_OriginPoint.x += dx;
		m_OriginPoint.y += dy;
		break;
	}
	case 1:{
		m_EndPoint.x += dx;
		m_EndPoint.y += dy;
		break;
	}
	}

}


void LineG::SetRgn()
{
	CRect rect;
	CPoint Spoint;
	CPoint Epoint;

	if (m_OriginPoint.x > m_EndPoint.x)
	{
		Spoint.x = m_EndPoint.x;
		Epoint.x = m_OriginPoint.x;
	}
	else
	{
		Epoint.x = m_EndPoint.x;
		Spoint.x = m_OriginPoint.x;
	}

	if (m_OriginPoint.y > m_EndPoint.y)
	{
		Spoint.y = m_EndPoint.y;
		Epoint.y = m_OriginPoint.y;
	}
	else
	{
		Epoint.y = m_EndPoint.y;
		Spoint.y = m_OriginPoint.y;
	}

	rect.SetRect(Spoint, Epoint);

	rect.left = static_cast<int>(rect.left - (m_Bold* 0.9) - 5);
	rect.top = static_cast<int>(rect.top - (m_Bold * 0.9) - 5);
	rect.right = static_cast<int>(rect.right + (m_Bold * 0.9) + 5);
	rect.bottom = static_cast<int>(rect.bottom + (m_Bold * 0.9) + 5);

	m_StartPoint.x = rect.left;
	m_StartPoint.y = rect.top;

	m_rgn.DeleteObject();
	m_rgn.CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
}


BOOL LineG::pointInRgn(CPoint point)
{
	if (m_rgn.PtInRegion(point))
	{
		return true;
	}

	return false;
}

void LineG::selectPoint(CPoint point)
{
	m_selectedIndex = -1;

	for (int i = 0; i < 8; i++)
	{
		if (selectedIndex[i].PtInRect(point))
		{
			m_selectedIndex = i;
			break;
		}
	}
}

void LineG::DrawPoint(CDC* pdc)
{

	CRect rect(m_OriginPoint, m_EndPoint);

	rect.left = static_cast<int>(rect.left - (m_Bold * 0.9) + 0.5);
	rect.top = static_cast<int>(rect.top - (m_Bold * 0.9) + 0.5);
	rect.right = static_cast<int>(rect.right + (m_Bold * 0.9) + 0.5);
	rect.bottom = static_cast<int>(rect.bottom + (m_Bold * 0.9) + 0.5);

	selectedIndex[0].SetRect(rect.left - 5, rect.top - 5,
		rect.left + 5, rect.top + 5);

	selectedIndex[1].SetRect(rect.right - 5, rect.bottom - 5,
		rect.right + 5, rect.bottom + 5);


	for (int i = 0; i < 2; i++)
	{
		CPen pen(PS_SOLID, 1, RGB(0, 0, 255));
		pdc->SelectObject(pen);
		pdc->SelectStockObject(WHITE_BRUSH);
		pdc->Ellipse(selectedIndex[i]);
	}
}