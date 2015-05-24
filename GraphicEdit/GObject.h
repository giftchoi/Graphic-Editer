#pragma once

#include "stdafx.h"
// GObject command target
enum Type{Selected,Line,Polyline,Ellipse,Rectangle,Text,Group};

class GObject : public CObject
{
public:
	GObject();
	virtual ~GObject();
	Type type;
};


