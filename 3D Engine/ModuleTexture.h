#ifndef __ModuleTexture_H__
#define __ModuleTexture_H__

#include "Module.h"
#include "Globals.h"

class ModuleTexture : public Module
{
public:
	ModuleTexture(Application* app, bool start_enabled = true);
	~ModuleTexture();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();

	void LoadTexture(const char* full_path);
};

#endif // __ModuleTexture_H__
