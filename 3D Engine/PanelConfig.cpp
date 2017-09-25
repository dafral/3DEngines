#include "PanelConfig.h"
#include "Application.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleImgui.h"
#include "PanelAbout.h"

PanelConfig::PanelConfig() {

	resizable = fullscreen = borderless = full_desktop = false;
}

PanelConfig::~PanelConfig() {

}

void PanelConfig::Draw() {

	ImGui::Begin("Configuration", &active, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	if (ImGui::CollapsingHeader("Application")) {
		//FPS
	}

	if (ImGui::CollapsingHeader("Window")) {

		//SLIDERS
		brightness = App->window->GetBrightness();
		if (ImGui::SliderFloat("Brightness", &brightness, 0, 1))
			App->window->SetBrightness(brightness);



		//BUTTONS
		if (ImGui::Checkbox("Fullscreen", &fullscreen))
			App->window->SwitchFullScreen(fullscreen);

		ImGui::SameLine();
		if (ImGui::Checkbox("Resizable", &resizable));
			//App->window->SetResizable(resizable);

		if (ImGui::Checkbox("Borderless", &borderless))
			App->window->SwitchBorderless(borderless);

		ImGui::SameLine();
		if (ImGui::Checkbox("Full Desktop", &full_desktop));
		//App->window->SetFullscreen(fullscreen);

	}

	if (ImGui::CollapsingHeader("Hardware")) {

		//CPU 
		ImGui::Text("Number of logical CPU cores : ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.00f,0.40f, 0.00f, 1.00f),"%d (Cache: %dkb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());

		//RAM
		ImGui::Text("Number of system RAM: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "%dGb", SDL_GetCPUCount());
	}


	ImGui::End();
}
