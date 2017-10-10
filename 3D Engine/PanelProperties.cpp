#include "PanelProperties.h"
#include "Application.h"

PanelProperties::PanelProperties()
{}

PanelProperties::~PanelProperties()
{}

void PanelProperties::Draw()
{
	ImGui::Begin("Properties", &active);

	//Transformation
	ImGui::Text("Transformation");

	ImGui::Separator();

	//Geometry
	ImGui::Text("Geometry");

	ImGui::Separator();

	//Texture
	ImGui::Text("Texture");

	ImGui::End();
}