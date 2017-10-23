#pragma once
#include "Globals.h"

enum component_type {COMPONENT_MESH, COMPONENT_TRANSFORM, COMPONENT_MATERIAL};

class Component
{
public:
	Component(component_type type) : type(type) {};
	virtual ~Component() {};

	virtual void Enable() {};
	virtual void Disable() {};
	virtual void Update() {};

public:
	component_type type;
};

// Different types of components ----------------------------------------------

class Component_Mesh : public Component
{
public:
	Component_Mesh() : Component(COMPONENT_MESH) {};
	~Component_Mesh() {};

public:
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

class Component_Material : public Component
{
public:
	Component_Material() : Component(COMPONENT_MATERIAL) {};
	~Component_Material() {};

public:
	uint id_texture = 0;
	uint height = 0;
	uint width = 0;
};
