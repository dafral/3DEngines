#ifndef __MATERIAL_IMPORTER_H__
#define __MATERIAL_IMPORTER_H__

#include "Application.h"
#include "Globals.h"
#include "Module.h"

class Component_Material;

class MaterialImporter : public Module
{
public: 
	MaterialImporter(Application * app, bool start_enabled = true);
	~MaterialImporter();
	
	bool Init();
	bool CleanUp();

	Component_Material* Import(const char* path);
	//bool LoadCheckers(Texture* resource);

	void SaveAsDDS();

	void RemoveMaterial(Component_Material* mat);
	
private:
	list<Component_Material*> materials;
	uint save_id = 0;

};

#endif // !__MATERIAL_IMPORTER_H__

