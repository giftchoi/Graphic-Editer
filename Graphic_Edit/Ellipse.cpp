// Ellipse.cpp : implementation file
//

#include "stdafx.h"
#include "Graphic_Edit.h"
#include "Ellipse.h"


// Ellipse

Ellipse::Ellipse()
{
	m_linepattern = 0;
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
}
