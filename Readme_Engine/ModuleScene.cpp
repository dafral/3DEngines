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
	octree = new Octree();

	Component_Camera* main_camera = new Component_Camera;
	root->AddComponent(main_camera);
	SetActiveCam(main_camera);

	return ret;
}

update_status ModuleScene::Update(float dt)
{
	root->Update();
	octree->CollectFrustumIntersections(GetActiveCam());
	octree->Draw();

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

// --------------------------------------------------------------

Component_Camera* ModuleScene::GetActiveCam()
{
	return play_camera;
}

void ModuleScene::SetActiveCam(Component_Camera* cam)
{
	if (cam->active_camera == false)
	{
		Component_Camera* main_camera = (Component_Camera*)root->FindComponent(COMPONENT_CAMERA);
		main_camera->SwitchActive(true);
		play_camera = main_camera;
	}
	else
	{
		// Setting to active = false the older camera
		if (play_camera != nullptr) play_camera->SwitchActive(false);

		// New camera
		play_camera = cam;
	}
}


