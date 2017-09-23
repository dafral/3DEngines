#pragma once

class Panel {
public:
	Panel();
	virtual ~Panel();

	virtual void Draw() = 0;

public:
	bool active;
	int x, y, h, w;

};