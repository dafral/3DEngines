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
	float x = -(1.0f - (float(App->input->GetMouseX() * 2.0f)) / App->window->GetWidth());
	float y = 1.0f - (float(App->input->GetMouseY() * 2.0f)) / App->window->GetHeight();

	LineSegment ray = App->camera->curr_camera->GetFrustum().UnProjectLineSegment(x, y);

	// Check collisions --------------------------------

	GameObject* selected = nullptr;
	vector<GameObject*> possible_collisions;
	App->octree->RayIntersections(ray, possible_collisions);

	for (uint i = 0; i < possible_collisions.size(); i++)
	{
		Component_Mesh* mesh = (Component_Mesh*)possible_collisions[i]->FindComponent(COMPONENT_MESH);

		if (mesh != nullptr)
		{
			Component_Transform* trans = (Component_Transform*)possible_collisions[i]->FindComponent(COMPONENT_TRANSFORM);

			LineSegment local_ray(ray);
			local_ray.Transform(trans->GetTransform().Inverted());

			for (uint j = 0; j < mesh->num_indices - 9; j += 3)
			{
				Triangle triangle;

				triangle.a.Set(mesh->vertices[mesh->indices[j]], mesh->vertices[mesh->indices[j + 1]], mesh->vertices[mesh->indices[j + 2]]);
				triangle.b.Set(mesh->vertices[mesh->indices[j + 3]], mesh->vertices[mesh->indices[j + 4]], mesh->vertices[mesh->indices[j + 5]]);
				triangle.c.Set(mesh->vertices[mesh->indices[j + 6]], mesh->vertices[mesh->indices[j + 7]], mesh->vertices[mesh->indices[j + 8]]);

				float dist;
				float3 hit_point;

				if (local_ray.Intersects(triangle, &dist, &hit_point))
				{
					if (dist < distance)
					{
						distance = dist;
						selected = possible_collisions[i];
					}
				}
			}
		}
	}

	App->imgui->hierarchy->go_selected = selected;
	distance = FLOAT_INF;
}