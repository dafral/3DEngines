#include "Globals.h"
#include "Application.h"
#include "ModuleImgui.h"
#include "ModuleWindow.h"

#include "Imgui/imgui.h"
#include "imgui_impl_sdl.h"

ModuleImgui::ModuleImgui(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleImgui::~ModuleImgui()
{}

bool ModuleImgui::Init()
{
	ImGui_ImplSdl_Init(App->window->window);

	return true;
}

update_status ModuleImgui::PreUpdate(float dt)
{
	ImGui_ImplSdl_NewFrame(App->window->window);

	return UPDATE_CONTINUE;
}

update_status ModuleImgui::Update(float dt)
{
	// Creating windows
	ImGui::Begin("Close Window");

	if (ImGui::Button("Exit", ImVec2(100, 100))) 
		return UPDATE_STOP;

	ImGui::End();

	// Render last
	ImGui::Render();

	return UPDATE_CONTINUE;
}

bool ModuleImgui::CleanUp()
{
	return true;
}


