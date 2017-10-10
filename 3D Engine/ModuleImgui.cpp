#include "Globals.h"
#include "Application.h"
#include "ModuleImgui.h"
#include "ModuleWindow.h"
#include "Panel.h"
#include "PanelConsole.h"
#include "PanelConfig.h"
#include "PanelAbout.h"
#include "PanelProperties.h"

#include "Imgui\imgui.h"
#include "imgui_impl_sdl.h"

#include "Brofiler\Brofiler.h"

ModuleImgui::ModuleImgui(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleImgui::~ModuleImgui()
{}

bool ModuleImgui::Init()
{
	ImGui_ImplSdl_Init(App->window->window);

	// Filling the vector with panels
	panels.push_back(console = new PanelConsole());
	panels.push_back(config = new PanelConfig());
	panels.push_back(about = new PanelAbout());
	panels.push_back(properties = new PanelProperties());

	return true;
}

update_status ModuleImgui::PreUpdate(float dt)
{
	ImGui_ImplSdl_NewFrame(App->window->window);

	return UPDATE_CONTINUE;
}

update_status ModuleImgui::Update(float dt)
{
	if (ImGui::BeginMainMenuBar()) 
	{
		if (ImGui::BeginMenu("Help")) 
		{
			if (ImGui::MenuItem("Documentation"));

			if (ImGui::MenuItem("Download latest version"));

			if (ImGui::MenuItem("Report a bug"));

			if (ImGui::MenuItem("About"))
				App->imgui->about->SwitchActive();

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	return UPDATE_CONTINUE;
}

bool ModuleImgui::CleanUp()
{
	for (std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it)
	{
		Panel* panel = (*it);

		if (panel != nullptr)
		{
			delete panel;
			panel = nullptr;
		}
	}

	panels.clear();

	return true;
}

void ModuleImgui::Draw()
{
	for (std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it)
	{
		Panel* panel = (*it);
		panel->Draw();
	}

	ImGui::Render();
}


