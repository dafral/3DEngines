#ifndef __ModuleImgui_H__
#define __ModuleImgui_H__

#include "Module.h"
#include <vector>

class Panel;
class PanelConsole;

class ModuleImgui : public Module
{
public:
	ModuleImgui(Application* app, bool start_enabled = true);
	~ModuleImgui();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	bool CleanUp();

public:
	PanelConsole* console =  nullptr;

private:
	std::vector<Panel*> panels;

};

#endif // __ModuleImgui_H__
