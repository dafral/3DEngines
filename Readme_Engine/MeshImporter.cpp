#include "Application.h"
#include "MeshImporter.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "ModuleGeometry.h"
#include "MaterialImporter.h"
#include "ModuleImgui.h"
#include "PanelProperties.h"
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

void MeshImporter::Import(Component_Mesh* mesh)
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

void MeshImporter::LoadFile(const char* path)
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

	// Create game object with component mesh like this:
	/*Component_Mesh* geo = new Component_Mesh(elements_num[1] / 3, vert, elements_num[0], ind, elements_num[2] / 3, tex_coords);*/

	RELEASE_ARRAY(data);
}

void MeshImporter::LoadScene(char* full_path)
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
		LoadMesh(empty_go, scene, node);

		CONSOLELOG("FBX file loaded!")
	}
	else
	{
		CONSOLELOG("Error loading scene %s", full_path);
	}

	// Releasing scene
	aiReleaseImport(scene);
}

void MeshImporter::LoadMesh(GameObject* parent, const aiScene* scene, const aiNode* node)
{
	if (node->mNumMeshes <= 0)
	{
		// Recursion
		for (int i = 0; i < node->mNumChildren; i++)
			LoadMesh(parent, scene, node->mChildren[i]);
	}
	else
	{
		for (int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* new_mesh = scene->mMeshes[node->mMeshes[i]];

			// Creating a Game Object (child of parent) for each mesh.
			GameObject* go = App->scene->CreateGameObject(node->mName.C_Str(), parent);

			// Adding component mesh
			Component_Mesh* new_component = new Component_Mesh;

			// Vertices
			new_component->num_vertices = new_mesh->mNumVertices;
			new_component->vertices = new float[new_component->num_vertices * 3];
			memcpy(new_component->vertices, new_mesh->mVertices, sizeof(float) * new_component->num_vertices * 3);

			// Indices
			if (new_mesh->HasFaces())
			{
				new_component->num_indices = new_mesh->mNumFaces * 3;
				new_component->indices = new uint[new_component->num_indices];
				for (uint i = 0; i < new_mesh->mNumFaces; i++)
				{
					if (new_mesh->mFaces[i].mNumIndices != 3) {
						CONSOLELOG("WARNING, geometry face with != 3 indices!");
					}
					else {
						memcpy(&new_component->indices[i * 3], new_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
					}
				}
			}

			// UVs
			if (new_mesh->HasTextureCoords(0))
			{
				new_component->num_uvs = new_mesh->mNumVertices;
				new_component->texture_coords = new float[new_component->num_uvs * 3];
				memcpy(new_component->texture_coords, new_mesh->mTextureCoords[0], sizeof(float) * new_component->num_uvs * 3);
			}

			// Changing transform
			Component_Transform* trans = (Component_Transform*)go->FindComponent(COMPONENT_TRANSFORM);

			if (node != nullptr)
			{
				aiVector3D translation;
				aiVector3D scaling;
				aiQuaternion rotation;

				node->mTransformation.Decompose(scaling, rotation, translation);

				float3 pos(translation.x, translation.y, translation.z);
				float3 scale(scaling.x, scaling.y, scaling.z);
				Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);

				trans->SetPosition(pos);
				trans->SetRotation(rot);
				trans->SetScale(scale);
			}

			// Creating bounding box
			new_component->bounding_box.SetNegativeInfinity();
			new_component->bounding_box.Enclose((float3*)new_mesh->mVertices, new_mesh->mNumVertices);
			new_component->bounding_box.TransformAsAABB(trans->GetTransform());

			// Check if we already loaded this mesh in memory
			Component_Mesh* aux = IsMeshLoaded(new_component);
			if (aux == nullptr)
			{
				LoadBuffers(new_component, new_mesh);
				loaded_meshes.push_back(new_component);
			}
			else
			{
				new_component->id_indices = aux->id_indices;
				new_component->id_vertices = aux->id_vertices;
				new_component->id_uvs = aux->id_uvs;
			}

			// Import mesh
			App->mesh_imp->Import(new_component);
			go->AddComponent(new_component);

			// Recursion
			for (int i = 0; i < node->mNumChildren; i++)
				LoadMesh(go, scene, node->mChildren[i]);
		}	
	}
}

Component_Mesh* MeshImporter::IsMeshLoaded(const Component_Mesh* new_component)
{
	Component_Mesh* ret = nullptr;

	for (int i = 0; i < loaded_meshes.size(); i++)
	{
		bool loaded = true;

		// vertices
		if (loaded_meshes[i]->num_vertices == new_component->num_vertices)
		{
			for (unsigned int j = 0; j < loaded_meshes[i]->num_vertices * 3; j++)
				if (loaded_meshes[i]->vertices[j] != new_component->vertices[j]) 
					loaded = false;
		}
		else loaded = false;

		// indices
		if (loaded_meshes[i]->num_indices == new_component->num_indices)
		{
			for (unsigned int j = 0; j < loaded_meshes[i]->num_indices; j++)
				if (loaded_meshes[i]->indices[j] != new_component->indices[j]) 
					loaded = false;
		}
		else loaded = false;

		if (loaded == true)
		{
			ret = loaded_meshes[i];
			break;
		}
	}

	return ret;
}

void MeshImporter::LoadBuffers(Component_Mesh* new_component, aiMesh* new_mesh)
{
	// Load buffer for vertices
	glGenBuffers(1, (GLuint*) &(new_component->id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, new_component->id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*new_component->num_vertices * 3, new_component->vertices, GL_STATIC_DRAW);

	// Load buffer for indices
	if (new_mesh->HasFaces())
	{
		glGenBuffers(1, (GLuint*) &(new_component->id_indices));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_component->id_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * new_component->num_indices, new_component->indices, GL_STATIC_DRAW);
	}

	// Load buffer for UVs
	if (new_mesh->HasTextureCoords(0))
	{
		glGenBuffers(1, (GLuint*) &(new_component->id_uvs));
		glBindBuffer(GL_ARRAY_BUFFER, (GLuint)new_component->id_uvs);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * new_component->num_uvs * 3, new_component->texture_coords, GL_STATIC_DRAW);
	}
}