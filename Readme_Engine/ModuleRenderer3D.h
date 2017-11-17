#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#include <vector>
class GameObject;

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

	float GetDepthRange();
	void SetDepthRange(float new_range);

	void SwitchBFCulling();
	void SwitchDepthTest();
	void SwitchColor();
	void SwitchTexture();
	void SwitchLights();

	void AddObjectToDraw(GameObject* go) { objects_to_draw.push_back(go); };

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	bool grid = true;
	bool wireframe = false;
	bool points = false;
	bool bf_culling = false;
	bool depth = true;
	bool color = true;
	bool texture = true;
	bool light = true;

	float p_color[3];

private:
	bool vsync;
	float depth_range;
	std::vector<GameObject*> objects_to_draw;	
};