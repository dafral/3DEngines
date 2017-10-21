#pragma once
#include "Module.h"
#include "Globals.h"
#include "GameObject.h"
#include <string>

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	GameObject* CreateGameObject();

public:
	GameObject* root;
};
