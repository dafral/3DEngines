#include "Application.h"
#include "ModuleTexture.h"

ModuleTexture::ModuleTexture(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleTexture::~ModuleTexture()
{}

bool ModuleTexture::Init()
{
	return true;
}

update_status ModuleTexture::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleTexture::CleanUp()
{
	return true;
}

void ModuleTexture::LoadTexture(const char* full_path)
{
	
}