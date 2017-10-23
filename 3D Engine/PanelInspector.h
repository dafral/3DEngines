#pragma once

#include "Panel.h"
#include "GameObject.h"

class PanelInspector : public Panel
{
public:
	PanelInspector(bool active) : Panel(active)
	{ 
		x = 0; 
	    y = 50; 
	    w = 200; 
	    h = 500;
		selected = nullptr;
	};

	void Draw();

public:
	GameObject* selected;
	uint num_GO = 0;
};
