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
	void OnSave(JSON_Doc * config);
	void SaveScene(JSON_Doc* config);


	JSON_Doc* scene_file;

public:
	GameObject* root = nullptr;
};
