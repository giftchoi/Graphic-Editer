// Text.cpp : implementation file
//

#include "stdafx.h"
#include "Graphic_Edit.h"
#include "TextG.h"
#include "GDIPLUS.h"

using namespace Gdiplus;

// Text

TextG::TextG()
{
	type = TEXT;
	m_font.DeleteObject();
	m_ID = -1;
}

TextG::TextG(const TextG* g)
{
	//모든 정보를 옮겨야 한다.
	//오브젝트꺼
	type = g->type;
	m_Bold = g->m_Bold;
	m_LineColor = g->m_LineColor;
	m_OriginPoint = g->m_OriginPoint;
	m_StartPoint = g->m_StartPoint;
	m_EndPoint = g->m_EndPoint;
	m_rgn.CreateRectRgn(0, 0, 0, 0);
	m_rgn.CopyRgn(&g->m_rgn);
	m_Alpha = g->m_Alpha;
	m_selected = FALSE;
	m_ID = -1;

	//자기꺼
	m_rgncolor = g->m_rgncolor;
	m_rgnpattern = g->m_rgnpattern;
	m_lf = g->m_lf;
	m_font.CreateFontIndirectW(&m_lf);
	m_str.Copy(g->m_str);
}

TextG::~TextG()
{
}


// Text member functions
IMPLEMENT_SERIAL(TextG, GObject, 1);

void TextG::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar << m_str.GetCount();
		for (int i = 0; i < m_str.GetCount(); i++)
		{
			UINT ch = m_str.GetAt(i);
			ar << ch;
		}

		ar << m_rgncolor << m_Line_Pattern << m_rgnpattern;

		ar << m_lf.lfHeight;
		ar << m_lf.lfWidth;
		ar << m_lf.lfEscapement;
		ar << m_lf.lfOrientation;
		ar << m_lf.lfWeight;
		ar << m_lf.lfItalic;
		ar << m_lf.lfUnderline;
		ar << m_lf.lfStrikeOut;
		ar << m_lf.lfCharSet;
		ar << m_lf.lfOutPrecision;
		ar << m_lf.lfClipPrecision;
		ar << m_lf.lfQuality;
		ar << m_lf.lfPitchAndFamily;
		CString str;
		str = m_lf.lfFaceName;
		ar << str;
	}
	else
	{	// loading code
		m_str.RemoveAll();

		int m_strCount;
		ar >> m_strCount;
		for (int i = 0; i < m_strCount; i++)
		{
			UINT ch;
			ar >> ch;
			m_str.Add(ch);
		}

		ar >> m_rgncolor >> m_Line_Pattern >> m_rgnpattern;

		::ZeroMemory(&m_lf, sizeof(m_lf));

		ar >> m_lf.lfHeight;
		ar >> m_lf.lfWidth;
		ar >> m_lf.lfEscapement;
		ar >> m_lf.lfOrientation;
		ar >> m_lf.lfWeight;
		ar >> m_lf.lfItalic;
		ar >> m_lf.lfUnderline;
		ar >> m_lf.lfStrikeOut;
		ar >> m_lf.lfCharSet;
		ar >> m_lf.lfOutPrecision;
		ar >> m_lf.lfClipPrecision;
		ar >> m_lf.lfQuality;
		ar >> m_lf.lfPitchAndFamily;
		CString str;
		ar >> str;
		wsprintf(m_lf.lfFaceName, TEXT("%s"), str);

		setFont(m_lf);
		SetRgn();
	}
}

CRect TextG::getBoundary()
{
	CRect crect;

	crect.left = m_StartPoint.x < m_EndPoint.x ? m_StartPoint.x : m_EndPoint.x;
	crect.top = m_StartPoint.y < m_EndPoint.y ? m_StartPoint.y : m_EndPoint.y;
	crect.right = m_StartPoint.x >= m_EndPoint.x ? m_StartPoint.x : m_EndPoint.x;
	crect.bottom = m_StartPoint.y >= m_EndPoint.y ? m_StartPoint.y : m_EndPoint.y;

	return crect;
}

void TextG::setPoint(int left, int top, int right, int bottom)
{
	m_OriginPoint.x = left;
	m_OriginPoint.y = top;

	m_StartPoint.x = left;
	m_StartPoint.y = top;
	m_EndPoint.x = right;
	m_EndPoint.y = bottom;
}

void TextG::move(int dx, int dy)
{
	m_StartPoint.x += dx;
	m_StartPoint.y += dy;
	m_EndPoint.x += dx;
	m_EndPoint.y += dy;
};

void TextG::draw(CDC* cdc)
{

	Graphics graphics(*cdc);
	graphics.SetSmoothingMode(SmoothingModeHighQuality);

	//setFont(m_lf);

	RECT rect;
	CSize strSize;
	strSize = cdc->GetTextExtent(m_str.GetData());
	rect.left = m_StartPoint.x;
	rect.top = m_StartPoint.y;
	rect.right = rect.left + 10;
	rect.bottom = rect.top + 20;

	cdc->SetTextColor(m_LineColor);
	cdc->SetBkColor(m_rgncolor);
	cdc->SetBkMode(TRANSPARENT);
	cdc->SelectObject(&m_font);

	cdc->DrawTextW((LPCTSTR)m_str.GetData(), m_str.GetSize(), &rect, DT_LEFT | DT_SINGLELINE | DT_CALCRECT);
	cdc->DrawTextW((LPCTSTR)m_str.GetData(), m_str.GetSize(), &rect, DT_LEFT | DT_SINGLELINE);


	/*CPoint p;
	// 커서의 위치를 수정하여 SetCaretPos로 위치 지정
	// x 축 커서의 위치는 문자의 길이 * 현재 폰트의 한 문자 넓이
	p.x = text.tmAveCharWidth*m_str.GetSize();
	// y 축 커서의 위치는 항상 한줄만 입력가능하므로 항상 0
	p.y = 0;
	// 커서의 위치를 p로 지정
	SetCaretPos(p.x,p.y);
	*/


	Point dPoint;
	dPoint.X = rect.right - rect.left;
	dPoint.Y = rect.bottom - rect.top;


	if (m_rgnpattern == 0)
	{
		SolidBrush solidBrush(Color(m_Alpha, GetRValue(m_rgncolor), GetGValue(m_rgncolor), GetBValue(m_rgncolor)));
		graphics.FillRectangle(&solidBrush, rect.left, rect.top, dPoint.X, dPoint.Y);
	}
	else if (m_rgnpattern >= 2)
	{
		HatchBrush hatchBrush((HatchStyle)(m_rgnpattern - 2), Color(m_Alpha, GetRValue(m_rgncolor), GetGValue(m_rgncolor), GetBValue(m_rgncolor)), Color::Transparent);
		graphics.FillRectangle(&hatchBrush, rect.left, rect.top, dPoint.X, dPoint.Y);
	}

	cdc->DrawTextW((LPCTSTR)m_str.GetData(), m_str.GetSize(), &rect, DT_LEFT | DT_SINGLELINE | DT_CALCRECT);
	cdc->DrawTextW((LPCTSTR)m_str.GetData(), m_str.GetSize(), &rect, DT_LEFT | DT_SINGLELINE);


	CPoint tSpoint;
	CPoint tEpoint;
	tSpoint.x = static_cast<int>(rect.left - (m_Bold * 0.9) + 0.5);
	tSpoint.y = static_cast<int>(rect.top - (m_Bold * 0.9) + 0.5);
	tEpoint.x = static_cast<int>(rect.right + (m_Bold * 0.9) + 0.5);
	tEpoint.y = static_cast<int>(rect.bottom + (m_Bold * 0.9) + 0.5);

	m_StartPoint.x = rect.left;
	m_StartPoint.y = rect.top;
	m_EndPoint.x = rect.right;
	m_EndPoint.y = rect.bottom;

	if (m_selected)
	{
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
	}

	SetRgn();

}

void TextG::SetRgn()
{
	pointSwap();
	m_rgn.DeleteObject();
	m_rgn.CreateRectRgn(m_StartPoint.x - 5, m_StartPoint.y - 5, m_EndPoint.x + 5, m_EndPoint.y + 5);
}

BOOL TextG::pointInRgn(CPoint point)
{
	return m_rgn.PtInRegion(point);
}


