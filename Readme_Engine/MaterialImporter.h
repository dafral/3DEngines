#ifndef __MATERIAL_IMPORTER_H__
#define __MATERIAL_IMPORTER_H__

#include "Application.h"
#include "Globals.h"
#include "Module.h"
#include <string>

class Component_Material;

class MaterialImporter : public Module
{
public: 
	MaterialImporter(Application * app, bool start_enabled = true);
	~MaterialImporter();
	
	bool Init();
	bool CleanUp();

	void Import(const char* full_path, GameObject* go);
	Component_Material* IsMaterialLoaded(string path);
	void SaveAsDDS();
	
private:
	vector<Component_Material*> paths_loaded;
	uint save_id = 0;

};

#endif // !__MATERIAL_IMPORTER_H__

