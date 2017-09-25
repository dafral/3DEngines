#include "PanelAbout.h"

PanelAbout::PanelAbout() {

	active = false;

}

PanelAbout::~PanelAbout() {

}

void PanelAbout::Draw() {

	if (active) {
		ImGui::Begin("About", &active, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

		ImGui::TextColored(ImVec4(1.00f, 0.00f, 0.50f, 1.00f), "3D ENGINE\n");
		ImGui::Text("Engine created from scratch in C++ for 3D Engines subject.\n\nMade by Marc de Pedro & David Franco.");
		
		ImGui::Separator();

		ImGui::Text("\nUsing MathGeoLib & ImGui");

		ImGui::End();
	}
}