#pragma once
#include "GObject.h"
#include <afxtempl.h>
// Text command target

class TextG : public GObject
{
	DECLARE_SERIAL(TextG)
public:
	TextG();
	TextG(const TextG* g);
	virtual ~TextG();

	COLORREF getBrushColor() { return m_rgncolor; }
	void setBrushColor(COLORREF c) { m_rgncolor = c; }

	void setLinePattern(int PatternIndex) { m_Line_Pattern = PatternIndex; }
	void setBrushPattern(int PatternIndex) { m_rgnpattern = PatternIndex; }

	void setThickness(int Thickness) { m_Bold = Thickness; }

	void setFont(LOGFONT lf) { m_lf = lf; m_font.DeleteObject(); m_font.CreateFontIndirect(&m_lf); }

	void virtual setPoint(int left, int top, int right, int bottom);
	void virtual move(int deltax, int deltay);
	void virtual draw(CDC*);
	CRect virtual getBoundary();

	void virtual SetRgn();
	BOOL virtual pointInRgn(CPoint point);

	void setStr(CArray <TCHAR, TCHAR> &str) { m_str.Copy(str); }

private:
	COLORREF m_rgncolor;
	int m_rgnpattern;
	int m_Line_Pattern;
	CFont m_font;								//폰트 
	CArray <TCHAR, TCHAR> m_str;				// 문자열
	LOGFONT m_lf;
public:
	virtual void Serialize(CArchive& ar);
};


