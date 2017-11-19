#include "Application.h"
#include "PanelConfig.h"
#include "mmgr/mmgr.h"

Application::Application()
{
	debug = new DebugDraw();

	PlayClock.fps_counter = 0;
	PlayClock.last_frame_fps = 0;

	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	scene = new ModuleScene(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	physics = new ModulePhysics3D(this);
	imgui = new ModuleImgui(this);
	octree = new ModuleOctree(this);
	picker = new ModulePicker(this);
	json = new ModuleJSON(this);
	filesystem = new ModuleFileSystem(this);
	material_imp = new MaterialImporter(this);
	mesh_imp = new MeshImporter(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	AddModule(physics);

	AddModule(scene);
	AddModule(octree);
	AddModule(picker);
	AddModule(json);
	AddModule(filesystem);
	AddModule(material_imp);
	AddModule(imgui);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL)
	{
		delete item->data;
		item = item->prev;
	}

	delete debug;
}

bool Application::Init()
{
	bool ret = true;
	PlayClock.fps_timer.Start();

	// Call Init() in all modules
	p2List_item<Module*>* item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		if (item->data->IsEnabled()) ret = item->data->Init();
		item = item->next;
	}

	// After all Init calls we call Start() in all modules
	CONSOLELOG("Application Start --------------");
	item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		if (item->data->IsEnabled()) ret = item->data->Start();
		item = item->next;
	}
	
	PlayClock.ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	PlayClock.dt = (float)PlayClock.ms_timer.Read() / 1000.0f;

	PlayClock.ms_timer.Start();
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	p2List_item<Module*>* item = list_modules.getFirst();
	
	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		if (item->data->IsEnabled()) ret = item->data->PreUpdate(PlayClock.dt);
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		if (item->data->IsEnabled()) ret = item->data->Update(PlayClock.dt);
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		if (item->data->IsEnabled()) ret = item->data->PostUpdate(PlayClock.dt);
		item = item->next;
	}

	FinishUpdate();
	return ret;
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	PlayClock.fps_counter++;

	if (PlayClock.fps_timer.Read() >= 1000) // 1 sec
	{
		PlayClock.last_frame_fps = PlayClock.fps_counter;
		PlayClock.fps_counter = 0;
		PlayClock.fps_timer.Start();
	}

	// FPS cap
	int frame_ms = (1000 / App->imgui->config->GetFPSCap()) - PlayClock.ms_timer.Read();
	if (frame_ms > 0) SDL_Delay(frame_ms);

	// Add fps and ms to the vector
	App->imgui->config->AddMs(PlayClock.ms_timer.Read());
	App->imgui->config->AddFps(PlayClock.last_frame_fps);
	App->imgui->config->AddMemory(m_getMemoryStatistics().totalActualMemory);
}

bool Application::CleanUp()
{
	bool ret = true;
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.add(mod);
}

void Application::SetState(State state)
{
	if (state == State::PLAY)
	{
		if (EngineState == State::PLAY)
		{
			EngineState = State::STOP;
			PlayClock.ms_timer.Reset();
			PlayClock.fps_timer.Reset();
			//ChangeCamera("Scene");
			//WantToLoad();
		}
		else
		{
			EngineState = State::PLAY;
			//ChangeCamera("Game");
			//WantToSave();
		}
	}
	else
	{
		EngineState = state;
	}

	CONSOLELOG("Engine State is Now: %i", EngineState);
}