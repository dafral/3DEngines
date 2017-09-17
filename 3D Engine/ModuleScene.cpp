#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleScene::~ModuleScene()
{}

// Load assets
bool ModuleScene::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 90.0f, 100.0f));
	App->camera->LookAt(vec3(0, 0, 60));

	return ret;
}

// Load assets
bool ModuleScene::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleScene::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = false;
	p.wire = false;
	p.color = Green;
	p.Render();

	Cube cube(10, 10, 10);
	cube.SetPos(0, 0, 0);
	cube.Scale(1, 1, 1);
	cube.color = Blue;
	cube.Render();

	return UPDATE_CONTINUE;
}

void ModuleScene::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	
}

