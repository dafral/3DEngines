#ifndef __MODULE_FILESYSTEM_H__
#define __MODULE_FILESYSTEM_H__

#include "Module.h"
#include "Globals.h"
#include <string>

using namespace std;

class ModuleFileSystem : public Module
{
public:

	ModuleFileSystem(Application * app, bool start_enabled = true);
	~ModuleFileSystem();

	bool Init();

	bool CleanUp();

	//Folder Functions
	string CreateFolder(const char* path, const char* name);
	bool SaveFile(const char * path, const char * file_content, const char * name, const char * extension, int size);
	bool SaveUnique(const char * path, const char * file_content, const char * name, const char * extension, int size, string& output_file, bool gen_uid);

	string GetFileFromPath(const char* path, bool extension);
	string GetExtension(const char * path);

	void SaveScene(JSON_Doc& config);
	void LoadScene(JSON_Doc * config);

	inline bool exists(const string &path)
	{
		{
			struct stat buffer;
			return (stat(path.c_str(), &buffer) == 0);
		}
	}


private:

	
};

#endif