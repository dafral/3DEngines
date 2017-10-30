#include "Application.h"
#include "PanelProperties.h"
#include "ModuleGeometry.h"
#include "ModuleImgui.h"
#include "PanelHierarchy.h"

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
		// Name
		ImGui::Begin("Properties", &active, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
		ImGui::Text("3D Model: %s", go->name.c_str());

		ImGui::Separator();

		// Transformation
		if (go->FindComponent(COMPONENT_TRANSFORM) != nullptr)
		{
			Component_Transform* trans = (Component_Transform*)go->FindComponent(COMPONENT_TRANSFORM);

			float3 position = trans->GetPosition();
			float3 scale = trans->GetScale();
			Quat rotation = trans->GetRotation();

			if (ImGui::DragFloat3("Position", (float*)&position, 0.25f))
				trans->SetPosition(position);
			if (ImGui::DragFloat3("Rotation", (float*)&rotation))
				trans->SetRotation(rotation);
			if (ImGui::DragFloat3("Scale", (float*)&scale, 0.05f))
				trans->SetScale(scale);
		}

		// Meshes
		if (go->FindComponent(COMPONENT_MESH) != nullptr)
		{
			ImGui::Text("Geometry\n");
			ImGui::Text("Vertices: %d", go->GetVertices());
			ImGui::Text("Triangles: %d", go->GetIndices() / 3);
			ImGui::Separator();
		}

		// Texture
		if (go->FindComponent(COMPONENT_MATERIAL) != nullptr)
		{
			ImGui::Text("Texture\n");
			ImGui::Text("Texture name: %s", texture_name.c_str());
			ImGui::Text("Texture dimensions: %d x %d", go->GetTextureWidth(), go->GetTextureHeight());
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
