#include "PanelConsole.h"
#include "Application.h"

PanelConsole::PanelConsole(bool active = true) : Panel(active)
{
	x = 300;
	y = 700;
	w = 750;
	h = 200;
}

PanelConsole::~PanelConsole() 
{}

void PanelConsole::Draw() 
{
	if (first) {
		ImGui::SetNextWindowPos(ImVec2(x, y));
		ImGui::SetNextWindowContentSize(ImVec2(w, h));
		first = false;
	}

	ImGui::Begin("Console", &active);
	ImGui::Text(App->text.begin());
	ImGui::End();
}

void PanelConsole::ConsoleText(const char* log) 
{
	App->text.append(log);
	App->text.append("\n");
}