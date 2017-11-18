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
class aiNode;
class aiScene;
class aiMesh;

class MeshImporter : public Module
{
public:
	MeshImporter(Application * app, bool start_enabled = true);
	~MeshImporter();

	bool Start();

	void Import(Component_Mesh* mesh);
	void LoadFile(const char* path);

	void LoadMesh(GameObject* parent, const aiScene* scene, const aiNode* node);


private:
	list<Component_Mesh*> meshes;
	int id = 0;
	uint save_id = 0;
	
	Component_Mesh* cube = nullptr;

};

#endif // !__MESH_IMPORTER_H__