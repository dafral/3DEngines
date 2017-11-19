#include "Application.h"
#include "ModuleFileSystem.h"

ModuleFileSystem::ModuleFileSystem(Application * app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleFileSystem::~ModuleFileSystem()
{

}

bool ModuleFileSystem::Init()
{
	//Libraries
	CreateFolder("", "Library");
	CreateFolder("Library", "Meshes");
	CreateFolder("Library", "Materials");

	return true;
}

bool ModuleFileSystem::CleanUp()
{
	return true;
}

string ModuleFileSystem::CreateFolder(const char * path, const char * name)
{
	string ret;

	string strPath = path;
	if (strPath.length() > 0 && strPath[strPath.length() - 1] != '\\') {
		strPath += '\\';
	}
	strPath += name;

	if (CreateDirectory(strPath.c_str(), NULL) == 0) {
		CONSOLELOG("ERROR Creating Directory %s [%s]", name, strerror(errno));
		return ret;
	}

	ret = strPath + '\\';

	return ret;
}


bool ModuleFileSystem::SaveFile(const char * path, const char * file_content, const char * name, const char * extension, int size)
{
	bool ret = false;

	string file = path;
	file += name;
	file += ".";
	file += extension;

	std::ofstream;
	FILE* new_file = fopen(file.c_str(), "wb");

	if (new_file)
	{
		fwrite(file_content, sizeof(char), size, new_file);
		ret = true;
	}
	else
	{
		CONSOLELOG("ERROR saving file %s: ", name);
	}

	fclose(new_file);
	return ret;
}

bool ModuleFileSystem::SaveUnique(const char * path, const char * file_content, const char * name, const char * extension, int size, std::string& output_file, bool gen_uid)
{
	bool ret = false;

	uint uniqueID = 0;
	string file;
	struct stat buffer;

	if (gen_uid == true) {
		do {
			file = path;
			file += name;
			file += "_";
			file += to_string(uniqueID++);
			file += ".";
			file += extension;
		} while (exists(file));

	}
	else {
		file = path;
		file += name;
		file += ".";
		file += extension;
	}

	FILE* new_file = fopen(file.c_str(), "wb");

	if (new_file != nullptr)
	{
		fwrite(file_content, sizeof(char), size, new_file);
		output_file = file;
		ret = true;
		fclose(new_file);
	}
	else
	{
		CONSOLELOG("ERROR unique saving file:\n\t%s\n\tERROR: %s ", name, strerror(errno));
	}

	return ret;
}

string ModuleFileSystem::GetFileFromPath(const char * path, bool take_extension)
{
	string ret;

	for (int i = 0; path[i] != '\0'; i++) {
		if (take_extension == false) {
			if (path[i] == '.')
				break;
		}
		if (path[i] == '\\' || path[i] == '/') {
			ret.clear();
			continue;
		}
		ret += path[i];
	}

	return ret;
}

string ModuleFileSystem::GetExtension(const char * path)
{
	string extension;

	for (int i = 0; path[i] != '\0'; i++) {
		if (path[i] == '.') {
			extension.clear();
			continue;
		}
		extension += path[i];
	}

	return extension;
}

void ModuleFileSystem::SaveScene(JSON_Doc& config)
{

}

void ModuleFileSystem::LoadScene(JSON_Doc * config)
{

}

