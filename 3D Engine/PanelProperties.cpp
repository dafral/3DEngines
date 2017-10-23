#include "Application.h"
#include "PanelProperties.h"
#include "ModuleGeometry.h"

PanelProperties::PanelProperties(bool active = true) : Panel(active)
{
	x = 16;
	y = 660;
	w = 223;
	h = 349;

	mod_x = 0;
	mod_y = 0;
	mod_z = 0;
}

PanelProperties::~PanelProperties()
{}

void PanelProperties::Draw()
{

	ImGui::SetNextWindowPos(ImVec2(x, y));
	ImGui::SetNextWindowContentSize(ImVec2(w, h));

	ImGui::Begin("Properties", &active, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
	ImGui::Text("3D Model: %s", GetGeometryName());

	ImGui::Separator();

	//Transformation
	ImGui::Text("Transformation\n");

	ImGui::Text("Position X: %d Y: %d Z: %d", mod_x, mod_y, mod_z);
	ImGui::Text("Rotation X: 0 Y: 0 Z: 0");
	ImGui::Text("Scale\tX: 1 Y: 1 Z: 1");

	ImGui::Separator();

	//Geometry
	ImGui::Text("Geometry\n");

	ImGui::Text("Vertices: %d", App->geometry->GetVertices());
	ImGui::Text("Triangles: %d", App->geometry->GetIndices()/3);

	ImGui::Separator();

	//Texture
	ImGui::Text("Texture\n");
	ImGui::Text("Texture name: %s", GetTextureName());
	ImGui::Text("Texture dimensions: %d x %d", App->geometry->GetTextureWidth(), App->geometry->GetTextureHeight());

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

void PanelProperties::SetPosition(vec3 pos)
{
	mod_x = pos.x;
	mod_y = pos.y;
	mod_z = pos.z;
}