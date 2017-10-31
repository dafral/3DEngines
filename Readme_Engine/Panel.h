#pragma once

#include "Globals.h"

class Panel 
{
public:
	Panel(bool active = true) : active(active) {};
	virtual ~Panel() {};
	virtual void Draw() {};
	void SwitchActive() { active != active; };

public:
	bool active;
	int x = 0, y = 0, h = 0, w = 0;
	bool first = true;
};