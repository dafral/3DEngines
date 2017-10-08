#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);
	void SetVsync(bool vsync);
	bool GetVsync();

	void SwitchBFCulling();
	void SwitchDepthTest();
	void SwitchColor();
	void SwitchTexture();
	void SwitchLights();

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	bool wireframe = false;
	bool points = false;
	bool bf_culling = false;
	bool depth = true;
	bool color = true;
	bool texture = true;
	bool light = true;

	float depth_range;
	float p_color[3];

private:
	bool vsync;
	
};