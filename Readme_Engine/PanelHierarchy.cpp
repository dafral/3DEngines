#include "Application.h"
#include "ModuleScene.h"
#include "PanelHierarchy.h"
#include "GameObject.h"
#include "Imgui/imgui.h"

PanelHierarchy::PanelHierarchy(bool active = true) : Panel(active)
{
	go_selected = nullptr;
}

void PanelHierarchy::Draw()
{
	if (active)
	{
		ImGui::SetNextWindowPos(ImVec2(x, y));
		ImGui::SetNextWindowSize(ImVec2(w, h));

		// Content
		ImGui::Begin("Hierarchy", &active);
		App->scene->root->CreateHierarchy();
		ImGui::End();
	}
}

// ---------------------------------------------------------------

void PanelHierarchy::AdjustPanel()
{
	x = MARGIN_X;
	y = MARGIN_Y;
	w = 300;
	h = App->window->GetHeight() / 2;
}