#include "Application.h"
#include "PanelProperties.h"
#include "ModuleGeometry.h"
#include "ModuleImgui.h"
#include "PanelHierarchy.h"
#include "GameObject.h"
#include "MathGeoLib/MathGeoLib.h"

PanelProperties::PanelProperties(bool active = true) : Panel(active)
{
	x = 15;
	y = 660;
	w = 223;
	h = 349;
}

PanelProperties::~PanelProperties()
{}

void PanelProperties::Draw()
{
	ImGui::SetNextWindowPos(ImVec2(x, y));
	ImGui::SetNextWindowContentSize(ImVec2(w, h));

	GameObject* go = App->imgui->hierarchy->selected;

	if (go != nullptr && go != App->scene->root)
	{
		ImGui::Begin("Properties", &active, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

		// Name
		ImGui::Text("Name: %s", go->name.c_str());

		// Transformation
		if (go->FindComponent(COMPONENT_TRANSFORM) != nullptr)
		{
			if (ImGui::CollapsingHeader("Transform"))
			{
				Component_Transform* trans = (Component_Transform*)go->FindComponent(COMPONENT_TRANSFORM);
				Component_Mesh* mesh = (Component_Mesh*)go->FindComponent(COMPONENT_MESH);
				Component_Camera* cam = (Component_Camera*)go->FindComponent(COMPONENT_CAMERA);

				float3 position = trans->GetPosition();
				float3 scale = trans->GetScale();
				Quat rotation = trans->GetRotation();

				float3 euler_rotation = RadToDeg(rotation.ToEulerXYZ());

				ImGui::Text("  X       Y      Z");

				if (ImGui::DragFloat3("Position", (float*)&position, 0.1f))
				{
					trans->SetPosition(position);
					if (mesh != nullptr)
					{
						mesh->bounding_box.SetNegativeInfinity();
						mesh->bounding_box.Enclose((float3*)mesh->vertices, mesh->num_vertices);
						mesh->bounding_box.TransformAsAABB(trans->GetTransform());
					}
				}

				if (ImGui::DragFloat3("Rotation", (float*)&euler_rotation, 0.1f))
				{
					trans->SetRotation(DegToRad(euler_rotation));
					if (mesh != nullptr)
					{
						mesh->bounding_box.SetNegativeInfinity();
						mesh->bounding_box.Enclose((float3*)mesh->vertices, mesh->num_vertices);
						mesh->bounding_box.TransformAsAABB(trans->GetTransform());
					}
				}
					
				if (ImGui::DragFloat3("Scale", (float*)&scale, 1.0f))
				{
					trans->SetScale(scale);
					if (mesh != nullptr)
					{
						mesh->bounding_box.SetNegativeInfinity();
						mesh->bounding_box.Enclose((float3*)mesh->vertices, mesh->num_vertices);
						mesh->bounding_box.TransformAsAABB(trans->GetTransform());
					}
				}		

				if (cam != nullptr) 
				{
					cam->SetPos(position);
					cam->SetZDir(trans->GetTransform().WorldZ());
					cam->SetYDir(trans->GetTransform().WorldY());
				}
			}
		}

		// Meshes
		if (go->FindComponent(COMPONENT_MESH) != nullptr)
		{
			if (ImGui::CollapsingHeader("Meshes"))
			{
				ImGui::Text("Vertices: %d", go->GetVertices());
				ImGui::Text("Triangles: %d", go->GetIndices() / 3);
			}
		}

		// Texture
		if (go->FindComponent(COMPONENT_MATERIAL) != nullptr)
		{
			if (ImGui::CollapsingHeader("Texture"))
			{
				ImGui::Text("Texture name: %s", texture_name.c_str());
				ImGui::Text("Texture dimensions: %d x %d", go->GetTextureWidth(), go->GetTextureHeight());
			}
		}

		ImGui::End();
	}
}

void PanelProperties::SetGeometryName(char* reference)
{
	std::string str = reference;

	int dot_pos = str.find_last_of(".");
	int bar_pos = str.find_last_of("\\");
	int name_length = dot_pos - bar_pos - 1;

	geometry_name = str.substr(bar_pos + 1, name_length);
}

const char* PanelProperties::GetGeometryName()
{
	return geometry_name.c_str();
}

void PanelProperties::SetTextureName(char* reference)
{
	std::string str = reference;

	int dot_pos = str.find_last_of(".");
	int bar_pos = str.find_last_of("\\");
	int name_length = dot_pos - bar_pos - 1;

	texture_name = str.substr(bar_pos + 1, name_length);
}
