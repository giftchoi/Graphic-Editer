#pragma once

#include "stdafx.h"
// GObject command target
enum Type{selected,line,polyline,ellipse,rectangle,text,group};

class GObject : public CObject
{
public:
	GObject();
	virtual ~GObject();
	Type type;
};


