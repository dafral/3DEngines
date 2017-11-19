#include "Application.h"
#include "Globals.h"
#include "ModuleScene.h"
#include "ModuleImgui.h"
#include "GameObject.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	root = CreateGameObject("Root");
}

ModuleScene::~ModuleScene()
{}

bool ModuleScene::Start()
{
	CONSOLELOG("Loading Intro assets");
	bool ret = true;

	scene_file = App->json->CreateJSON("Library/config.json");
	scene_file->Save();

	return ret;
}

void ModuleScene::SaveScene(JSON_Doc* file)
{
	//file->SetArray("GameObjects");
	//file->SetArray("Components");

	App->scene->OnSave(file);
}

update_status ModuleScene::Update(float dt)
{
	root->Update();

	// Provisional button for octree
	if (ImGui::Button("Play", ImVec2(0, 0)) && App->EngineState == STOP)
	{
		App->SetState(State::PLAY);
	}

	ImGui::SameLine();

	if (ImGui::Button("Stop", ImVec2(0, 0)) && App->EngineState == PLAY)
	{
		App->SetState(State::STOP);
	}

	return UPDATE_CONTINUE;
}


bool ModuleScene::CleanUp()
{
	CONSOLELOG("Unloading Intro scene");

	return true;
}

// --------------------------------------------------------------

GameObject* ModuleScene::CreateGameObject(std::string name, GameObject* parent)
{
	GameObject* aux;
	aux = new GameObject(name, parent);

	if (parent != nullptr)
		parent->AddChildren(aux);

	// Add component trans by default
	Component_Transform* new_component = new Component_Transform;
	aux->AddComponent(new_component);

	return aux;
}

void ModuleScene::OnSave(JSON_Doc * config)
{
	//Travel go
	if (root != nullptr) {
		root->OnSave(config);
	}
}


