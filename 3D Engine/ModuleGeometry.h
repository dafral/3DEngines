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
};

#endif // __ModuleGeometry_H__