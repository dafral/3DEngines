#pragma once
#include "Module.h"
#include "Globals.h"
#include <string>
#include "Component.h"

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
	void SetActiveCam(Component_Camera* cam);
	Component_Camera* GetActiveCam();

public:
	GameObject* root = nullptr;
	Component_Camera* play_camera = nullptr;
};
