#include "Glew\include\GL\glew.h"
#include "PanelConfig.h"
#include "Application.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleImgui.h"
#include "PanelAbout.h"

PanelConfig::PanelConfig() {

	resizable = fullscreen = borderless = false;
	vsync = App->renderer3D->GetVsync();

	x = 786;
	y = 30;
	w = 476;
	h = 554;

}

PanelConfig::~PanelConfig() {

}

void PanelConfig::Draw() {

	ImGui::SetNextWindowPos(ImVec2(x, y));
	ImGui::SetNextWindowContentSize(ImVec2(w, h));

	ImGui::Begin("Configuration", &active);

	if (ImGui::CollapsingHeader("Application"))
		DrawApplication();

	if (ImGui::CollapsingHeader("Window")) 
		DrawWindow();

	if (ImGui::CollapsingHeader("Hardware")) 
		DrawHardware();

	if (ImGui::CollapsingHeader("Renderer"))
		DrawRenderer();

	if (ImGui::CollapsingHeader("Input"))
		DrawInput();

	if (ImGui::CollapsingHeader("Texture"))
		DrawTexture();

	ImGui::End();
}

void PanelConfig::AddFps(float fps)
{
	if (fps_buffer.size() > FPS_LOG_NUM)
		fps_buffer.erase(fps_buffer.begin());

	fps_buffer.push_back(fps);
}

void PanelConfig::AddMs(float ms)
{
	if (ms_buffer.size() > MS_LOG_NUM)
		ms_buffer.erase(ms_buffer.begin());

	ms_buffer.push_back(ms);
}

int PanelConfig::GetFPSCap()
{
	if (fps_cap == 0) return 60;
	else return fps_cap;
}

void PanelConfig::DrawApplication()
{
	char title[25];

	// fps
	ImGui::SliderInt(" Max FPS", &fps_cap, 0, 100);

	ImGui::Text("Limit Framerate:");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "%i", fps_cap);

	if (ImGui::Checkbox("Vsync", &vsync))
		App->renderer3D->SetVsync(vsync);

	sprintf_s(title, 25, "Framerate %.1f", fps_buffer[fps_buffer.size() - 1]);
	ImGui::PlotHistogram("##Framerate", &fps_buffer[0], fps_buffer.size(), 0, title, 0.0f, 100.0f, ImVec2(300, 120));

	// dt
	sprintf_s(title, 25, "Miliseconds %.1f", ms_buffer[ms_buffer.size() - 1]);
	ImGui::PlotHistogram("##Dt", &ms_buffer[0], ms_buffer.size(), 0, title, 0.0f, 100.0f, ImVec2(300, 120));
}

void PanelConfig::DrawWindow()
{
	//SLIDERS
	brightness = App->window->GetBrightness();
	if (ImGui::SliderFloat("Brightness", &brightness, 0, 1))
		App->window->SetBrightness(brightness);

	int width = App->window->GetWidth();
	if (ImGui::SliderInt("Width", &width, 900, 1920))
		App->window->SetWidth(width);

	int height = App->window->GetHeight();
	if (ImGui::SliderInt("Height", &height, 600, 1024))
		App->window->SetHeight(height);

	//BUTTONS
	if (ImGui::Checkbox("Fullscreen", &fullscreen))
		App->window->SwitchFullScreen(fullscreen);

	ImGui::SameLine();
	if (ImGui::Checkbox("Resizable", &resizable));
		App->window->SwitchResizable();

	if (ImGui::Checkbox("Borderless", &borderless))
		App->window->SwitchBorderless(borderless);
}

void PanelConfig::DrawHardware()
{
	//SDL VERSION
	ImGui::Text("SDL version: ");
	ImGui::SameLine();
	SDL_version version;
	SDL_VERSION(&version);
	ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "%d.%d.%d", version.major, version.minor, version.patch);

	ImGui::Text("OpenGL version: ");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "%s", glewGetString(GL_VERSION));

	ImGui::Separator();

	//CPU 
	ImGui::Text("Number of logical CPU cores : ");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "%d (Cache: %dkb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());

	//RAM
	ImGui::Text("Number of system RAM: ");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "%dGb", SDL_GetCPUCount());

	//CAPS
	ImGui::Text("Caps: ");
	ImGui::SameLine();
	if (SDL_Has3DNow)ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "3DNow");
	ImGui::SameLine();
	if (SDL_HasAVX)ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "AVX");
	ImGui::SameLine();
	if (SDL_HasAltiVec)ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "AltiVec");
	ImGui::SameLine();
	if (SDL_HasMMX)ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "MMX");
	ImGui::SameLine();
	if (SDL_HasRDTSC)ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "RDTSC");

	if (SDL_HasSSE)ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "\t\tSSE");
	ImGui::SameLine();
	if (SDL_HasSSE2)ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "SSE2");
	ImGui::SameLine();
	if (SDL_HasSSE3)ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "SSE3");
	ImGui::SameLine();
	if (SDL_HasSSE41)ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "SSE41");
	ImGui::SameLine();
	if (SDL_HasSSE42)ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "SSE42");

	ImGui::Separator();

	//GPU
	ImGui::Text("GPU Information");
	ImGui::Text("Vendor: ");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "%s", glGetString(GL_VENDOR));
	ImGui::Text("Renderer: ");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "%s", glGetString(GL_RENDERER));
	ImGui::Text("Version: ");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "%s", glGetString(GL_VERSION));
}


void PanelConfig::DrawRenderer()
{
	if (ImGui::Checkbox("Grid", &App->renderer3D->grid));

	if (ImGui::Checkbox("Wireframe Mode", &App->renderer3D->wireframe));	

	if (ImGui::Checkbox("Points Mode", &App->renderer3D->points));

	if (ImGui::Checkbox("Backface Culling", &App->renderer3D->bf_culling))
		App->renderer3D->SwitchBFCulling();

	if (ImGui::Checkbox("Depth", &App->renderer3D->depth))
		App->renderer3D->SwitchDepthTest();
	
	if (App->renderer3D->depth) {
		ImGui::SameLine();
		float depth_range = App->renderer3D->GetDepthRange();
		if (ImGui::SliderFloat("Range", &depth_range, 0.00, 1.00))
			App->renderer3D->SetDepthRange(depth_range);
	}

	if(ImGui::Checkbox("Color", &App->renderer3D->color))
		App->renderer3D->SwitchColor();
	ImGui::ColorPicker3("Polygons color", App->renderer3D->p_color);

	if (ImGui::Checkbox("Texture 2D", &App->renderer3D->texture))
		App->renderer3D->SwitchTexture();

	if (ImGui::Checkbox("Lights", &App->renderer3D->light))
		App->renderer3D->SwitchLights();
	

}

void PanelConfig::DrawInput ()
{
	ImGui::Text("Mouse coords:\nX: %d\tY: %d", App->input->GetMouseX(), App->input->GetMouseY());

	ImGui::Separator();

	ImGui::Text("Camera configuration");
	ImGui::Checkbox("X axis", &App->camera->orb_x_inverted);
	ImGui::Checkbox("Y axis", &App->camera->orb_y_inverted);
	ImGui::Checkbox("Invert zoom", &App->camera->wheel_inverted);
	float sensitivity = App->camera->GetSensitivity();
	if (ImGui::SliderFloat("Camera sensitivity", &sensitivity, 0.10f, 1.00f))
		App->camera->SetSensitivity(sensitivity);
}

void PanelConfig::DrawTexture()
{

}