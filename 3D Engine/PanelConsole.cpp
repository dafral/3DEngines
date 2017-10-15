#include "PanelConsole.h"
#include "Application.h"

PanelConsole::PanelConsole() 
{
	x = 16;
	y = 30;
	w = 750;
	h = 326;

}

PanelConsole::~PanelConsole() 
{}

void PanelConsole::Draw() 
{
	ImGui::SetNextWindowPos(ImVec2(x, y));
	ImGui::SetNextWindowContentSize(ImVec2(w, h));

	ImGui::Begin("Console", &active);
	ImGui::Text(App->text.begin());
	ImGui::End();
}

void PanelConsole::ConsoleText(const char* log) 
{
	App->text.append(log);
	App->text.append("\n");
}