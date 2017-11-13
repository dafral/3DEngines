#pragma once

#include "Globals.h"
#define MARGIN_X 20
#define MARGIN_Y 50

class Panel 
{
public:
	Panel(bool active = true) : active(active) {};
	virtual ~Panel() {};

	virtual void Draw() {};
	virtual void AdjustPanel() {};

	void SwitchActive() { active != active; };

public:
	bool active;
	int x = 0, y = 0, h = 0, w = 0;
};