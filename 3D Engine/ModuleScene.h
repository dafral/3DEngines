#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include "src/MathGeoLib.h"
#include <vector>

struct PhysBody3D;
struct PhysMotor3D;

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

public:
	int num_spheres = 0;
	std::vector<float3> positions;
};
