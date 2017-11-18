#include "Application.h"
#include "MeshImporter.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "ModuleGeometry.h"
#include "MaterialImporter.h"
#include "ModuleImGui.h"
#include <direct.h>

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")



MeshImporter::MeshImporter(Application * app, bool start_enabled) : Module(app, start_enabled)
{}

MeshImporter::~MeshImporter()
{}

bool MeshImporter::Start()
{
	return true;
}

void MeshImporter::SaveToLibrary(Component_Mesh * mesh)
{
	// Save Mesh to file
	// file structure has: 
	// amount of indices / vertices / texture_coords
	// indices / vertices / texture_coords data

	uint elements_num[3] = { mesh->num_indices, mesh->num_vertices * 3, mesh->num_uvs };
	uint total_size = sizeof(elements_num) + sizeof(uint)*mesh->num_indices + sizeof(float)*(mesh->num_vertices * 3 + mesh->num_uvs * 3);

	char* data = new char[total_size];
	char* cursor = data;

	//store the number of elements of each type
	uint size = sizeof(elements_num);
	memcpy(cursor, elements_num, size);
	cursor += size;

	//store indices
	size = sizeof(uint)*elements_num[0];
	memcpy(cursor, mesh->indices, size);
	cursor += size;

	//store vertices
	size = sizeof(float)*elements_num[1];
	memcpy(cursor, mesh->vertices, size);
	cursor += size;

	//store texture_coords
	size = sizeof(float)*elements_num[2];
	memcpy(cursor, mesh->texture_coords, size);
	cursor += size;

	//Serialize data to file
	char file[69];
	sprintf_s(file, "Library//Meshes//mesh_%d.ownmesh", save_id++);

	FILE* f = fopen(file, "wb");
	fwrite(data, sizeof(char), total_size, f);
	fclose(f);

	RELEASE_ARRAY(data);

	CONSOLELOG("Created %s.", file);
}

void MeshImporter::LoadFile(const char * path)
{
	//Open file path and get size
	FILE* file = fopen(path, "rb");
	fseek(file, 0L, SEEK_END);
	uint total_size = ftell(file);
	rewind(file); //go back to file begining

	// Copy file to a data buffer
	char* data = new char[total_size];
	char* cursor = data;
	fread(data, sizeof(char), total_size, file);
	fclose(file);

	//Start reading num_elements for indices/vertices/texture_coords
	uint elements_num[3];
	uint size = sizeof(elements_num);
	memcpy(elements_num, cursor, size);
	cursor += size;

	//Read indices
	uint* ind = new uint[elements_num[0]];
	size = sizeof(uint)*elements_num[0];
	memcpy(ind, cursor, size);
	cursor += size;

	//Read vertices
	float* vert = new float[elements_num[1]];
	size = sizeof(float)*elements_num[1];
	memcpy(vert, cursor, size);
	cursor += size;

	//Read texture coords
	float* tex_coords = new float[elements_num[2]];
	size = sizeof(float)*elements_num[2];
	memcpy(tex_coords, cursor, size);
	cursor += size;

	//Create a mesh with this info
	Component_Mesh* geo = new Component_Mesh(elements_num[1] / 3, vert, elements_num[0], ind, elements_num[2] / 3, tex_coords);
	meshes.push_back(geo);

	RELEASE_ARRAY(data);
}
