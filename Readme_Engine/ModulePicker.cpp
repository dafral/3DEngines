#include "Application.h"
#include "ModulePicker.h"
#include "ModuleImgui.h"
#include "PanelHierarchy.h"

bool ModulePicker::Start()
{
	return true;
}

update_status ModulePicker::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModulePicker::CleanUp()
{
	return true;
}

void ModulePicker::RayCast()
{
	// Create ray --------------------------------------

	// Check collisions --------------------------------

	GameObject* selected;
	vector<GameObject*> possible_collisions;
	//App->octree->RayIntersections(ray, possible_collisions);

	for (uint i = 0; i < possible_collisions.size(); ++i)
	{
		Component_Mesh* mesh = (Component_Mesh*)possible_collisions[i]->FindComponent(COMPONENT_MESH);

		if (mesh != nullptr)
		{
			for (uint i = 0; i < mesh->num_indices; i += 3)
			{
				Triangle triangle;

				triangle.a.Set(mesh->vertices[mesh->indices[i]], mesh->vertices[mesh->indices[i + 1]], mesh->vertices[mesh->indices[i + 2]]);
				triangle.b.Set(mesh->vertices[mesh->indices[i + 3]], mesh->vertices[mesh->indices[i + 4]], mesh->vertices[mesh->indices[i + 5]]);
				triangle.c.Set(mesh->vertices[mesh->indices[i + 6]], mesh->vertices[mesh->indices[i + 7]], mesh->vertices[mesh->indices[i + 8]]);

				float dist;
				float3 hit_point;

				/*if (ray.Intersects(triangle, &dist, &hit_point))
				{
					if (dist < distance)
					{
						distance = dist;
						selected = possible_collisions[i];
					}
				}*/
			}
		}
	}

	App->imgui->hierarchy->go_selected = selected;
	distance = FLOAT_INF;
}