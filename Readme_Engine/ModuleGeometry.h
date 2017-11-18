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
	bool CleanUp();

	void LoadScene(char* full_path);
};

#endif // __ModuleGeometry_H__