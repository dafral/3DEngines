#include "PanelConsole.h"
#include "Application.h"

PanelConsole::PanelConsole() 
{
	w = 750;
	h = 300;
}

PanelConsole::~PanelConsole() 
{}

void PanelConsole::Draw() 
{
	ImGui::Begin("Console", &active, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	ImGui::Text(App->text.begin());
	ImGui::End();
}

void PanelConsole::ConsoleText(const char* log) 
{
	App->text.append(log);
	App->text.append("\n");
}