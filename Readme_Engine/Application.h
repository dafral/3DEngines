#pragma once

#include "p2List.h"
#include "Globals.h"
#include "Timer.h"
#include "Imgui\imgui.h"

#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModuleImgui.h"
#include "ModuleGeometry.h"

enum State { PLAY = 0, PAUSE, STOP };

struct EngineClock
{
	Timer	ms_timer;
	Timer   fps_timer;
	float	dt;
	float   fps_counter;
	int     last_frame_fps;
};

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleScene* scene;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModulePhysics3D* physics;
	ModuleImgui* imgui;
	ModuleGeometry* geometry;

	ImGuiTextBuffer text;

private:

	p2List<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	void SetState(State state);

	State EngineState;
	EngineClock PlayClock;
	EngineClock EditorClock;

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

// Give App pointer access everywhere
extern Application* App;