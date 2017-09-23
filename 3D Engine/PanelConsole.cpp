#include "PanelConsole.h"

PanelConsole::PanelConsole() {

	w = 750;
	h = 300;
}

PanelConsole::~PanelConsole() {

}

void PanelConsole::Draw() {
	
	ImGui::Begin("Console", &active, ImGuiWindowFlags_NoResize);
	ImGui::Text(text.begin());
	ConsoleText("Hola Marc, que tal estas");
	ImGui::End();

}

void PanelConsole::ConsoleText(const char* log) {

	text.append(log);
	text.append("\n");

}