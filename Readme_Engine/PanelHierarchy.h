#pragma once

#include "Panel.h"
#include "GameObject.h"

class PanelHierarchy : public Panel
{
public:
	PanelHierarchy(bool active) : Panel(active)
	{ 
		x = 15; 
	    y = 30; 
	    w = 185; 
	    h = 300;
		selected = nullptr;
	};

	void Draw();

public:
	GameObject* selected;
};
