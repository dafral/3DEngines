#include "PanelAbout.h"
#include "Application.h"
#include "Glew/include/GL/glew.h"
#include "SDL/include/SDL.h"

PanelAbout::PanelAbout(bool active = true) : Panel(active)
{
	x = 786;
	y = 660;
	w = 476;
	h = 300;
}

PanelAbout::~PanelAbout() 
{}

void PanelAbout::Draw() 
{
	ImGui::SetNextWindowPos(ImVec2(x, y));
	ImGui::SetNextWindowSize(ImVec2(w, h));

	if (active) 
	{
		ImGui::Begin("About", &active, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

		ImGui::TextColored(ImVec4(1.00f, 0.00f, 0.50f, 1.00f), "3D ENGINE\n");
		ImGui::Text("Engine created from scratch in C++ for 3D Engines subject.\n\nMade by Marc de Pedro & David Franco.");
		
		ImGui::Separator();

		ImGui::Text("Libraries used: ");

		ImGui::Text("SDL version: ");
		ImGui::SameLine();
		SDL_version version;
		SDL_VERSION(&version);
		ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "%d.%d.%d", version.major, version.minor, version.patch);

		ImGui::Text("OpenGL version: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "4.5.0");

		ImGui::Text("Glew version: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "%s", glewGetString(GLEW_VERSION));

		ImGui::Text("Assimp version: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "3.1.1");

		ImGui::Text("Evil version: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "1.7.8");

		ImGui::Text("ImGui version: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "%s", ImGui::GetVersion());

		ImGui::Separator();

		ImGui::Text("Protected by the TIM license.\nCheck the documentation for more information.");

		ImGui::End();
	}
}