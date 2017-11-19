#pragma once

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "GameObject.h"

class ModulePicker : public Module
{
public:
	ModulePicker(Application * app, bool start_enabled = true) : Module(app, start_enabled) {};
	~ModulePicker() {};

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void RayCast();

private:
	float distance = FLOAT_INF;
	LineSegment picker;
};
