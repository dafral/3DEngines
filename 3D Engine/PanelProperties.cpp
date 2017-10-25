#include "Application.h"
#include "PanelProperties.h"
#include "ModuleGeometry.h"
#include "ModuleImgui.h"
#include "PanelInspector.h"

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

	GameObject* go = App->imgui->inspector->selected;

	if (go != nullptr && go != App->scene->root)
	{
		// Name
		ImGui::Begin("Properties", &active, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
		ImGui::Text("3D Model: %s", GetGeometryName());

		ImGui::Separator();

		// Transformation
		if (go->FindComponent(COMPONENT_TRANSFORM) != nullptr)
		{
			ImGui::Text("Transformation\n");
			ImGui::Text("Position X: 0 Y: 0 Z: 0");
			ImGui::Text("Rotation X: 0 Y: 0 Z: 0");
			ImGui::Text("Scale\tX: 1 Y: 1 Z: 1");
			ImGui::Separator();
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
			ImGui::Text("Texture name: %s", GetTextureName());
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

void PanelProperties::SetTextureName(char* reference)
{
	std::string str = reference;

	int dot_pos = str.find_last_of(".");
	int bar_pos = str.find_last_of("\\");
	int name_length = dot_pos - bar_pos - 1;

	texture_name = str.substr(bar_pos + 1, name_length);
}

const char* PanelProperties::GetGeometryName()
{
	return geometry_name.c_str();
}

const char* PanelProperties::GetTextureName()
{
	return texture_name.c_str();
}
