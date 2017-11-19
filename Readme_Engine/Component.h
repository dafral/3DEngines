#pragma once
#include "Globals.h"
#include "MathGeoLib/MathGeoLib.h"
#include <string>

#include "Glew\include\GL\glew.h"
#include "SDL\include\SDL_opengl.h"

#include <string>
#include <gl/GL.h>
#include <gl/GLU.h>

enum component_type {COMPONENT_MESH, COMPONENT_TRANSFORM, COMPONENT_MATERIAL, COMPONENT_CAMERA };

class GameObject;
class Component_Transform;
class json_file;

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

//0...1
class Component_Mesh : public Component
{
public:
	Component_Mesh() : Component(COMPONENT_MESH) {};
	Component_Mesh(uint num_ver, float* ver, uint num_ind, uint* ind, uint num_uv = 0, float* uv = nullptr, uint num_norm = 0, float* norm = nullptr);
	~Component_Mesh() 
	{
		delete[] indices;
		delete[] vertices;
		delete[] texture_coords;
	};

	void Update();
	void AdaptBoundingBox(GameObject* go, Component_Transform* trans);

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

	AABB bounding_box;
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
	std::string path;
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
		return transform;
	};

private:
	float3 position;
	float3 scale;
	float3 euler_rotation;

	Quat rotation;
	float4x4 transform;
};

//0...1
class Component_Camera : public Component
{
public:
	Component_Camera();
	~Component_Camera() {};	

	void Update();

	const float GetFOV() { return fov; };
	const bool IsActive() { return active_camera; };
	void SwitchActive(bool new_active) { active_camera = new_active; };
	void SetPos(float3 pos) { frustum.pos = pos; };
	void SetZDir(const float3 & front) { frustum.front = front.Normalized(); };
	void SetYDir(const float3 & up) { frustum.up = up.Normalized(); };
	void SetFOV(float new_fov);
	void SetAspectRatio(const float & set);
	bool AABBInside(AABB &aabb);
	Frustum GetFrustum() { return frustum; };

	void MoveUp(const float movement);
	void MoveDown(const float movement);
	void MoveLeft(const float movement);
	void MoveRight(const float movement);
	void MoveForwards(const float movement);
	void MoveBackwards(const float movement);
	void Rotate(const float dx, const float dy);

	float* GetViewMatrix() const;
	float* GetProjectionMatrix() const;
	const float3 GetPosition();

public:
	bool active_camera = false;
	float fov;
	float aspect_ratio;

private:
	Frustum frustum;
};
