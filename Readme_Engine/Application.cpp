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
	
	EditorClock.fps_timer.Start();
	EditorClock.ms_timer.Start();
	RealClock.ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	EditorClock.dt = (float)EditorClock.ms_timer.Read() / 1000.0f;
	EditorClock.ms_timer.Start();
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	p2List_item<Module*>* item = list_modules.getFirst();
	
	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		if (item->data->IsEnabled()) ret = item->data->PreUpdate(EditorClock.dt);
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		if (item->data->IsEnabled()) ret = item->data->Update(EditorClock.dt);
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		if (item->data->IsEnabled()) ret = item->data->PostUpdate(EditorClock.dt);
		item = item->next;
	}

	FinishUpdate();
	return ret;
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	EditorClock.fps_counter++;

	if (EditorClock.fps_timer.Read() >= 1000) // 1 sec
	{
		EditorClock.last_frame_fps = EditorClock.fps_counter;
		EditorClock.fps_counter = 0;
		EditorClock.fps_timer.Start();
	}

	// FPS cap
	int frame_ms = (1000 / App->imgui->config->GetFPSCap()) - EditorClock.ms_timer.Read();
	if (frame_ms > 0) SDL_Delay(frame_ms);

	// Add fps and ms to the vector
	App->imgui->config->AddMs(EditorClock.ms_timer.Read());
	App->imgui->config->AddFps(EditorClock.last_frame_fps);
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
			PlayClock.ms_timer.Stop();
		}
		else
		{
			EngineState = State::PLAY;
			PlayClock.ms_timer.Start();
		}
	}
	else
	{
		PlayClock.ms_timer.Reset();
		PlayClock.ms_timer.Stop();
		EngineState = state;
	}

	CONSOLELOG("Engine State is Now: %i", EngineState);
}