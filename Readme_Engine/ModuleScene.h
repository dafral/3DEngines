#pragma once
#include "Module.h"
#include "Globals.h"
#include "Component.h"

#include <string>

class GameObject;

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	GameObject* CreateGameObject(std::string name, GameObject* parent = nullptr);

public:
	GameObject* root = nullptr;
};
