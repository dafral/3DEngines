#ifndef __MODULE_JSON_H__
#define __MODULE_JSON_H__

#include "Module.h"
#include "JSON/parson.h"
#include <list>

class JSON_Doc
{
public:
	JSON_Doc(JSON_Value* value, JSON_Object* object, const char* path);
	JSON_Doc(JSON_Object* Entry);
	~JSON_Doc();

	void SetString(const char* set, const char* str);
	void SetBool(const char* set, bool bo);
	void SetNumber(const char* set, double nu);
	void SetArray(const char * array_name);
	const char* GetString(const char* str);
	bool GetBool(const char* bo);
	double GetNumber(const char* nu);
	JSON_Doc GetArray(const char* field, int index) const;
	int GetArraySize(const char * field) const;

	JSON_Doc SetEntry(const char* set);
	JSON_Doc GetEntry(const char* set) const;
	const char* GetPath();
	bool MoveToSectionFromArray(const char * field, int index);
	
	void Save();
	void CleanUp();

private:
	JSON_Value*  value = nullptr;
	JSON_Object* object = nullptr;
	const char*  path;

};


class ModuleJSON : public Module {
public:
	ModuleJSON(Application* app, bool start_enabled = true);
	~ModuleJSON();

	bool Init();
	bool CleanUp();

	JSON_Doc* LoadJSON(const char* path);
	JSON_Doc* CreateJSON(const char* path);


private:
	std::list<JSON_Doc*> jsons;

public:
	JSON_Doc* config = nullptr;
};

#endif // !__MODULE_JSON_H__