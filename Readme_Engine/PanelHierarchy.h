#pragma once

#include "Panel.h"

class GameObject;

class PanelHierarchy : public Panel
{
public:
	PanelHierarchy(bool active);
	virtual ~PanelHierarchy() {};

	void Draw();
	void AdjustPanel();

public:
	GameObject* go_selected;

};
