#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Primitive.h"
#include "PhysBody3D.h"
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
	PPlane p(0, 1, 0, 0);
	p.axis = false;
	p.wire = false;
	p.color = Green;
	p.Render();

	// Just doin the render
	for (int i = 0; i < num_spheres; i++)
	{
		float3 p = positions[i];

		PSphere s(2); // Bullet
		s.color.Set(255, 0, 0);
		s.SetPos(p.x, p.y, p.z);
		s.Render();
	}		

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

