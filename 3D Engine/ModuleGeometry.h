#ifndef __ModuleGeometry_H__
#define __ModuleGeometry_H__

#include "Module.h"
#include "Globals.h"

struct texture_data
{
	uint id_texture = 0;
	uint height = 0;
	uint width = 0;
};

struct mesh_data 
{
	uint id_vertices = 0; 
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_indices = 0; 
	uint num_vertices = 0;
	float* vertices = nullptr;

	uint id_uvs = 0;
	uint num_uvs = 0;
	float* texture_coords = nullptr;
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
	void LoadTexture(const char* full_path);
	void Draw();

	void SetTextureDimensions(int w, int h);

	const int GetVertices();
	const int GetIndices();
	const int GetTextureWidth();
	const int GetTextureHeight();

private:
	std::vector<mesh_data*> meshes;
	texture_data tex;
};

#endif // __ModuleGeometry_H__