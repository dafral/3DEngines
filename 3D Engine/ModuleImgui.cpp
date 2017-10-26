#include "Globals.h"
#include "Application.h"
#include "ModuleImgui.h"
#include "ModuleWindow.h"

#include "Panel.h"
#include "PanelConsole.h"
#include "PanelConfig.h"
#include "PanelAbout.h"
#include "PanelProperties.h"
#include "PanelHierarchy.h"

#include "Imgui\imgui.h"
#include "imgui_impl_sdl.h"
#include "Glew\include\GL\glew.h"

ModuleImgui::ModuleImgui(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleImgui::~ModuleImgui()
{}

bool ModuleImgui::Init()
{
	ImGui_ImplSdl_Init(App->window->window);

	// Filling the vector with panels
	panels.push_back(console = new PanelConsole(true));
	panels.push_back(config = new PanelConfig(true));
	panels.push_back(about = new PanelAbout(false));
	panels.push_back(properties = new PanelProperties(true));
	panels.push_back(hierarchy = new PanelHierarchy(true));

	return true;
}

update_status ModuleImgui::PreUpdate(float dt)
{
	ImGui_ImplSdl_NewFrame(App->window->window);

	return UPDATE_CONTINUE;
}

update_status ModuleImgui::Update(float dt)
{
	if (!App->renderer3D->color)
		glEnable(GL_COLOR_MATERIAL);

	if (ImGui::BeginMainMenuBar()) 
	{
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Console window"))
				console->SwitchActive();

			if (ImGui::MenuItem("Configuration window"))
				config->SwitchActive();

			if (ImGui::MenuItem("Properties window"))
				properties->SwitchActive();

			if (ImGui::MenuItem("About window"))
				about->SwitchActive();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help")) 
		{
			if (ImGui::MenuItem("Documentation"))
			    ShellExecute(NULL, "open", "https://github.com/dafral/3DEngines", NULL, NULL, SW_SHOWNORMAL);

			if (ImGui::MenuItem("Download latest version"))
				ShellExecute(NULL, "open", "https://github.com/dafral/3DEngines/releases", NULL, NULL, SW_SHOWNORMAL);

			if (ImGui::MenuItem("Report a bug"))
				ShellExecute(NULL, "open", "https://github.com/dafral/3DEngines/issues", NULL, NULL, SW_SHOWNORMAL);

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Quit"))
			return update_status::UPDATE_STOP;

		ImGui::EndMainMenuBar();
	}

	if (!App->renderer3D->color)
		glDisable(GL_COLOR_MATERIAL);

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
	if (!App->renderer3D->color)
		glEnable(GL_COLOR_MATERIAL);

	for (std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it)
	{
		if ((*it)->active == true)
		{
			Panel* panel = (*it);
			panel->Draw();
		}
	}

	ImGui::Render();
}


