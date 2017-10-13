#include "PanelProperties.h"
#include "Application.h"

PanelProperties::PanelProperties()
{}

PanelProperties::~PanelProperties()
{}

void PanelProperties::Draw()
{
	ImGui::Begin("Properties", &active);
	ImGui::Text("3D Model: ");

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

	ImGui::End();
}