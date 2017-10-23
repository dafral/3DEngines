#include "Application.h"
#include "ModuleScene.h"
#include "PanelInspector.h"
#include "Imgui/imgui.h"

void PanelInspector::Draw()
{
	// Setting dimensions
	ImGui::SetNextWindowPos(ImVec2(x, y));
	ImGui::SetNextWindowContentSize(ImVec2(w, h));

	// Content
	ImGui::Begin("Inspector", &active);

	if (ImGui::Button("+ Create Game Object", { (float)w, 20 }))
	{
		if (selected != nullptr) App->scene->CreateGameObject("New Game Object", selected);
		else App->scene->CreateGameObject("New Game Object", App->scene->root);
	}

	App->scene->root->CreateHierarchy();
	ImGui::End();
}