#ifndef __ModuleGeometry_H__
#define __ModuleGeometry_H__

#include "Module.h"
#include "Globals.h"

struct vertex_data 
{
	uint id_vertices = 0; 
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_indices = 0; 
	uint num_vertices = 0;
	float* vertices = nullptr;
};

class ModuleGeometry : public Module
{
public:
	ModuleGeometry(Application* app, bool start_enabled = true);
	~ModuleGeometry();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();

	void LoadGeometry(const char* full_path);
	void Draw();

	/*void SetFileName(const char* full_path);
	const char* GetFileName();*/
	const int GetVertices();
	const int GetIndices();

private:
	std::vector<vertex_data> meshes;
	//std::vector<const char> filename;
};

#endif // __ModuleGeometry_H__