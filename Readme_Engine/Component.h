#pragma once
#include "Globals.h"
#include "MathGeoLib/MathGeoLib.h"

enum component_type {COMPONENT_MESH, COMPONENT_TRANSFORM, COMPONENT_MATERIAL, COMPONENT_CAMERA };

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

//0...*
class Component_Mesh : public Component
{
public:
	Component_Mesh() : Component(COMPONENT_MESH) {};
	~Component_Mesh() 
	{
		delete[] indices;
		delete[] vertices;
		delete[] texture_coords;
	};

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

//0...1
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

//0...1
class Component_Transform : public Component
{
public:
	Component_Transform() : Component(COMPONENT_TRANSFORM) 
	{
		position = { 0.0f, 0.0f, 0.0f };
		scale = { 1.0f, 1.0f, 1.0f };
		rotation = { 0.0f, 0.0f, 0.0f, 0.0f};
	};
	~Component_Transform() {};

	const float3 GetPosition() { return position; };
	const float3 GetScale() { return scale; };
	const Quat GetRotation() { return rotation; };

	void SetPosition(float3 pos) { position = pos; };
	void SetScale(float3 scl) { scale = scl; };
	void SetRotation(Quat rot) { rotation = rot; };
	void SetRotation(float3 rot) { rotation = Quat::FromEulerXYZ(rot.x, rot.y, rot.z); };

	float4x4 GetTransform() 
	{ 
		transform = transform.FromTRS(position, rotation, scale);
		return transform.Transposed();
	};

private:
	float3 position;
	float3 scale;
	float3 euler_rotation;

	Quat rotation;
	float4x4 transform;
};

class Component_Camera : public Component
{
public:
	Component_Camera();
	~Component_Camera() {};	

	virtual void Update();

	const float GetFOV() { return fov; };
	const float GetDepth() { return depth; };
	void SetFOV(float new_fov);
	void SetDepth(float new_depth) { depth = new_depth; };

public:

	Frustum frustum;
	float fov;
	float depth;
	float aspect_ratio;

};
