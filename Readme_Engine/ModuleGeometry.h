#ifndef __ModuleGeometry_H__
#define __ModuleGeometry_H__

#include "Module.h"
#include "Globals.h"

struct aiScene;
struct aiNode;

class ModuleGeometry : public Module
{
public:
	ModuleGeometry(Application* app, bool start_enabled = true);
	~ModuleGeometry();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();

	void LoadScene(char* full_path);
	void LoadGeometry(GameObject* parent, const aiScene* scene, const aiNode* node);

	void LoadMaterial(const char* full_path, GameObject* go);
};

#endif // __ModuleGeometry_H__