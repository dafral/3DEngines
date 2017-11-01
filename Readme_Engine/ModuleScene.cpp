#include "Application.h"
#include "Globals.h"
#include "ModuleScene.h"
#include "ModuleImgui.h"
#include "GameObject.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleScene::~ModuleScene()
{}

bool ModuleScene::Start()
{
	CONSOLELOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(10.0f, 4.0f, -4.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	root = CreateGameObject("Root");

	return ret;
}

update_status ModuleScene::Update(float dt)
{
	root->Update();

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

