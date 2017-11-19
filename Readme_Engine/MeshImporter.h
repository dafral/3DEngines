#pragma once
#ifndef __MESH_IMPORTER_H__
#define __MESH_IMPORTER_H__

#include "Component.h"
#include "Globals.h"
#include "Module.h"
#include <list>
#include <vector>

class Component;
class Component_Mesh;
class Component_Material;
class GameObject;

struct aiNode;
struct aiScene;
struct aiMesh;

class MeshImporter : public Module
{
public:
	MeshImporter(Application * app, bool start_enabled = true);
	~MeshImporter();

	bool Start();

	void Import(Component_Mesh* mesh);
	void LoadFile(const char* path);
	void LoadScene(char* full_path);
	void LoadMesh(GameObject* parent, const aiScene* scene, const aiNode* node);
	Component_Mesh* IsMeshLoaded(const Component_Mesh* new_component);
	void LoadBuffers(Component_Mesh* new_component, aiMesh* new_mesh);

private:
	vector<Component_Mesh*> loaded_meshes;

	uint save_id = 0;
};

#endif // !__MESH_IMPORTER_H__