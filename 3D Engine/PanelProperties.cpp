#include "PanelProperties.h"
#include "Application.h"

PanelProperties::PanelProperties()
{}

PanelProperties::~PanelProperties()
{}

void PanelProperties::Draw()
{
	ImGui::Begin("Properties", &active);
	ImGui::Text("3D Model: %s", GetGeometryName());

	ImGui::Separator();

	//Transformation
	ImGui::Text("Transformation\n");

	ImGui::Text("Position X: 0 Y: 0 Z: 0");
	ImGui::Text("Rotation X: 0 Y: 0 Z: 0");
	ImGui::Text("Scale\tX: 1 Y: 1 Z: 1");

	ImGui::Separator();

	//Geometry
	ImGui::Text("Geometry\n");

	ImGui::Text("Vertices: %d", App->geometry->GetVertices());
	ImGui::Text("Indices: %d", App->geometry->GetIndices());

	ImGui::Separator();

	//Texture
	ImGui::Text("Texture\n");
	ImGui::Text("Texture name: %s", GetTextureName());

	ImGui::End();
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