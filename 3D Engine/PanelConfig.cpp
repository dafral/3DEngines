#include "PanelConfig.h"
#include "Application.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleImgui.h"

PanelConfig::PanelConfig() {

}

PanelConfig::~PanelConfig() {

}

void PanelConfig::Draw() {

	ImGui::Begin("Configuration", &active, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	if (ImGui::CollapsingHeader("Application")) {
		//FPS
	}

	if (ImGui::CollapsingHeader("Window")) {
		//if (ImGui::SliderFloat("Brightness", brightness, 0, 1));

		if (ImGui::Checkbox("Fullscreen", &fullscreen));
			//App->window->SetFullscreen(fullscreen);

		ImGui::SameLine();
		if (ImGui::Checkbox("Resizable", &resizable))
			//App->window->SetResizable(resizable);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Restart to apply");
	}

	if (ImGui::CollapsingHeader("Hardware")) {

	}

	if (ImGui::BeginMenu("Help")) {
		
		if (ImGui::MenuItem("Documentation"));

		if (ImGui::MenuItem("Download latest version"));

		if (ImGui::MenuItem("Report a bug"));
		
		if (ImGui::MenuItem("About"));
		

		ImGui::EndMenu();

	}

	ImGui::End();
}