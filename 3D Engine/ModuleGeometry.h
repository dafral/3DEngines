#ifndef __ModuleGeometry_H__
#define __ModuleGeometry_H__

#include "Module.h"
#include "Globals.h"

class ModuleGeometry : public Module
{
public:
	ModuleGeometry(Application* app, bool start_enabled = true);
	~ModuleGeometry();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();

	void LoadMeshes(const char* full_path, GameObject* go);
	void LoadMaterial(const char* full_path, GameObject* go);

	void SetTextureDimensions(int w, int h);

	const int GetVertices();
	const int GetIndices();
	const int GetTextureWidth();
	const int GetTextureHeight();
};

#endif // __ModuleGeometry_H__