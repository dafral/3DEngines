#include "PanelConsole.h"
#include "Application.h"

PanelConsole::PanelConsole(bool active = true) : Panel(active)
{}

PanelConsole::~PanelConsole() 
{}

void PanelConsole::Draw() 
{
	ImGui::SetNextWindowPos(ImVec2(x, y));
	ImGui::SetNextWindowSize(ImVec2(w, h));

	ImGui::Begin("Console", &active);
	ImGui::Text(App->text.begin());
	ImGui::End();
}

void PanelConsole::ConsoleText(const char* log) 
{
	App->text.append(log);
	App->text.append("\n");
}

void PanelConsole::AdjustPanel()
{
	h = 300;
	w = App->window->GetWidth() / 2;
	x = (App->window->GetWidth() / 2) - (w / 2);
	y = App->window->GetHeight() - (MARGIN_Y + h);
}