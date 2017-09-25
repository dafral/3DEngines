#pragma once

#include "Globals.h"

class Panel {
public:
	Panel();
	virtual ~Panel();

	virtual void Draw() = 0;

	void SwitchActive();

public:
	bool active;
	int x, y, h, w;

};