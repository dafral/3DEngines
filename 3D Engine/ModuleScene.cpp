#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "PanelConsole.h"

#include "Glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>


ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleScene::~ModuleScene()
{}

// Load assets
bool ModuleScene::Start()
{
	CONSOLELOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(10.0f, 4.0f, -4.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	// Random numbers
	LCG random;
	num_spheres = random.Int(2, 5);

	// Random positions
	for (int i = num_spheres; i > 0; i--)
	{
		float3 p = { random.Float(0, 10), random.Float(0, 10), random.Float(0, 10) };
		positions.push_back(p);
	}

	// Check collisions
	for (int i = 0; i < num_spheres; i++)
	{
		float3 p = positions[i];
		Sphere sph1(p, 2); // MathGeoLib

		for (int i2 = i; i2 < num_spheres; i2++)
		{
			if (i != i2)
			{
				float3 p2 = positions[i2];
				Sphere sph2(p2, 2); // MathGeoLib

				if (sph1.Intersects(sph2))
					App->imgui->console->ConsoleText("Spheres are colliding");
			}
		}
	}

	return ret;
}

// Update
update_status ModuleScene::Update(float dt)
{
	return UPDATE_CONTINUE;
}


// Load assets
bool ModuleScene::CleanUp()
{
	CONSOLELOG("Unloading Intro scene");

	return true;
}

void ModuleScene::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	
}

