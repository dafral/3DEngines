#include "Application.h"
#include "ModuleGeometry.h"
#include "ModuleScene.h"
#include "PanelProperties.h"
#include "Component.h"
#include "GameObject.h"

#include "glew/include/GL/glew.h"
#include "MathGeoLib/MathGeoLib.h"

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")

ModuleGeometry::ModuleGeometry(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleGeometry::~ModuleGeometry()
{}

bool ModuleGeometry::Init()
{
	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	// DeviL Initialization
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	return true;
}                                                                        

bool ModuleGeometry::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();
	return true;
}

void ModuleGeometry::LoadScene(char* full_path)
{
	// Creating the parent (empty game object) 
	App->imgui->properties->SetGeometryName(full_path);
	const char* name = App->imgui->properties->GetGeometryName();
	GameObject* empty_go = App->scene->CreateGameObject(name, App->scene->root);

	// Loading scene
	const aiScene* scene = aiImportFile(full_path, aiProcessPreset_TargetRealtime_MaxQuality);
	aiNode* node = scene->mRootNode;

	if (scene != nullptr && scene->HasMeshes())
	{
		App->mesh_imp->LoadMesh(empty_go, scene, node);

		CONSOLELOG("FBX file loaded!")
	}
	else 
	{
		CONSOLELOG("Error loading scene %s", full_path);
	}

	// Releasing scene
	aiReleaseImport(scene);
}

