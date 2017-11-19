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
	bool IsMeshLoaded(const aiMesh* mesh);
private:
	list<Component_Mesh*> meshes;
	vector<aiMesh*> loaded_meshes;

	int id = 0;
	uint save_id = 0;
};

#endif // !__MESH_IMPORTER_H__