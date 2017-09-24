#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "src/MathGeoLib.h"
#include "PanelConsole.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleScene::~ModuleScene()
{}

// Load assets
bool ModuleScene::Start()
{
	CONSOLELOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 90.0f, 100.0f));
	App->camera->LookAt(vec3(0, 0, 60));

	return ret;
}

// Load assets
bool ModuleScene::CleanUp()
{
	CONSOLELOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleScene::Update(float dt)
{
	PPlane p(0, 1, 0, 0);
	p.axis = false;
	p.wire = false;
	p.color = Green;
	p.Render();

	// Creating circles

	PSphere s1(2);
	s1.color.Set(255, 0, 0);
	s1.SetPos(0, 10, 0);
	s1.Render();

	Sphere sph1(float3(0, 10, 0), 2);

	PSphere s2(2);
	s2.color.Set(0, 255, 0);
	s2.SetPos(0, 10, 0);
	s2.Render();

	Sphere sph2(float3(0, 10, 0), 2);

	if (sph1.Intersects(sph2))
		App->imgui->console->ConsoleText("Spheres are colliding");
	else
		App->imgui->console->ConsoleText("Spheres are not colliding");



	return UPDATE_CONTINUE;
}

void ModuleScene::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	
}

