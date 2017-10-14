#include "Application.h"
#include "ModuleGeometry.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "Glew\include\GL\glew.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

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

	return true;
}

update_status ModuleGeometry::Update(float dt)
{
	return UPDATE_CONTINUE;
}                                                                           

bool ModuleGeometry::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();
	return true;
}

void ModuleGeometry::LoadGeometry(const char* full_path)
{
	const aiScene* scene = aiImportFile(full_path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			vertex_data data;

			// Vertices
			aiMesh* new_mesh = scene->mMeshes[i];
			data.num_vertices = new_mesh->mNumVertices;
			data.vertices = new float[data.num_vertices * 3];
			memcpy(data.vertices, new_mesh->mVertices, sizeof(float) * data.num_vertices * 3);
			CONSOLELOG("New mesh with %d vertices.", data.num_vertices);

			// Load buffer for vertices
			glGenBuffers(1, (GLuint*) &(data.id_vertices));
			glBindBuffer(GL_ARRAY_BUFFER, data.id_vertices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*data.num_vertices * 3, data.vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			// Indices
			if (new_mesh->HasFaces())
			{
				data.num_indices = new_mesh->mNumFaces * 3;
				data.indices = new uint[data.num_indices];
				for (uint i = 0; i < new_mesh->mNumFaces; i++)
				{
					if (new_mesh->mFaces[i].mNumIndices != 3) {
						CONSOLELOG("WARNING, geometry face with != 3 indices!");
					}
					else {
						memcpy(&data.indices[i * 3], new_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
					}
				}
			}

			// Load buffer for indices
			glGenBuffers(1, (GLuint*) &(data.id_indices));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.id_indices);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*data.num_indices, data.indices, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			meshes.push_back(data);
		}
		aiReleaseImport(scene);
	}
	else
		CONSOLELOG("Error loading scene %s", full_path);
}

void ModuleGeometry::Draw()
{
	for (int i = 0; i < meshes.size(); i++)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, meshes[i].id_vertices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[i].id_indices);
		glDrawElements(GL_TRIANGLES, meshes[i].num_indices, GL_UNSIGNED_INT, NULL);
		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

//void ModuleGeometry::SetFileName(const char* full_path)
//{
//	int length = strlen(full_path) - 1;
//
//	for (int i = length; i >= length && full_path[i] != (const char)"\\"; i--)
//		filename.push_back(full_path[i]);
//}
//
//const char* ModuleGeometry::GetFileName()
//{
//	return dropped;
//}

const int ModuleGeometry::GetVertices()
{
	int t_vertices = 0;

	for (int i = 0; i < meshes.size(); i++)
		t_vertices += meshes[i].num_vertices;

	return t_vertices;
}

const int ModuleGeometry::GetIndices()
{
	int t_indices = 0;

	for (int i = 0; i < meshes.size(); i++)
		t_indices += meshes[i].num_indices;

	return t_indices;
}
