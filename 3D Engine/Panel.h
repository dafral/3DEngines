#pragma once

#include "Globals.h"

class Panel 
{
public:
	Panel();
	virtual ~Panel();
	virtual void Draw() {};
	void SwitchActive();

public:
	bool active;
	int x, y, h, w;
	bool first = true;
};