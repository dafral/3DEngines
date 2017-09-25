#include "PanelConfig.h"

PanelConfig::PanelConfig() {

}

PanelConfig::~PanelConfig() {

}

void PanelConfig::Draw() {

	ImGui::Begin("Configuration", &active, ImGuiWindowFlags_NoResize);
	ImGui::End();
}