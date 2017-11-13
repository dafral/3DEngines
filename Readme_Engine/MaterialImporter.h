#ifndef __MATERIAL_IMPORTER_H__
#define __MATERIAL_IMPORTER_H__

#include "Application.h"
#include "Globals.h"
#include "Module.h"

class MaterialImporter : public Module
{
public: 
	MaterialImporter(Application * app, bool start_enabled = true);
	~MaterialImporter();
	
	bool Init();

	bool Import(const char* file, const char* path, std::string& output_file);
	//bool Import(const void* buffer, uint size, std::string& output_file);
	void Load(const char* full_path, GameObject* go);
	//bool LoadCheckers(Texture* resource);

};

#endif // !__MATERIAL_IMPORTER_H__

