#include "Application.h"
#include "ModuleJSON.h"
#include "ModuleInput.h"
#include "Component.h"
#include "GameObject.h"
#include "Color.h"

//COMPONENT CAMERA============================================

Component_Camera::Component_Camera() : Component(COMPONENT_CAMERA) 
{
	active_camera = false;

	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.nearPlaneDistance = 1.0f;
	frustum.farPlaneDistance = 100.0f;

	frustum.verticalFov = DEGTORAD * 60.0f;
	frustum.horizontalFov = SCREEN_WIDTH / SCREEN_HEIGHT;
	aspect_ratio = SCREEN_WIDTH / SCREEN_HEIGHT;

	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	
	SetAspectRatio(1.3f);
	SetFOV(60.0f);
};

void Component_Camera::Update()
{
	//Getting frustum coords
	float3 corners[8];
	frustum.GetCornerPoints(corners);

	App->debug->DrawFrustum(corners, 3, Yellow);
}

void Component_Camera::SetFOV(float FOV)
{
	fov = FOV;

	if (FOV > 0) {
		frustum.verticalFov = DEGTORAD * FOV;
		frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov / 2.0f) * aspect_ratio);
	}

	if (aspect_ratio > 0)
		frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect_ratio);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLoadMatrixf((GLfloat*)frustum.ProjectionMatrix().Transposed().v);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Component_Camera::SetAspectRatio(const float& ar)
{
	aspect_ratio = ar;

	if(frustum.verticalFov > 0)
		frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect_ratio);

	SetFOV(fov);
}

void Component_Camera::SetAspectRatio(const float & w, const float & h)
{
	aspect_ratio = w / h;

	if (frustum.verticalFov > 0)
		frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect_ratio);

	SetFOV(fov);
}

bool Component_Camera::AABBInside(AABB &aabb)
{
	int vertex_num = aabb.NumVertices();
	for (int i = 0; i < 6; i++)
	{
		int points_out = 0;
		for (int j = 0; j < vertex_num; j++)
		{
			Plane plane = frustum.GetPlane(i);
			if (plane.IsOnPositiveSide(aabb.CornerPoint(j)))
			{
				points_out++;
			}
		}

		if (points_out == 8) return false;
	}

	return true;
}

void Component_Camera::MoveUp(const float movement)
{
	float3 move = float3::zero;
	move += frustum.up * movement;
	frustum.Translate(move);
}

void Component_Camera::MoveDown(const float movement)
{
	float3 move = float3::zero;
	move -= frustum.up * movement;
	frustum.Translate(move);
}

void Component_Camera::MoveLeft(const float movement)
{
	float3 move = float3::zero;
	move -= frustum.WorldRight() * movement;
	frustum.Translate(move);
}

void Component_Camera::MoveRight(const float movement)
{
	float3 move = float3::zero;
	move += frustum.WorldRight() * movement;
	frustum.Translate(move);
}

void Component_Camera::MoveForwards(const float movement)
{
	float3 move = float3::zero;
	move += frustum.front * movement;
	frustum.Translate(move);
}

void Component_Camera::MoveBackwards(const float movement)
{
	float3 move = float3::zero;
	move -= frustum.front * movement;
	frustum.Translate(move);
}

void Component_Camera::Rotate(const float dx, const float dy)
{
	if (dx != 0)
	{
		Quat rotation_x = Quat::RotateY(dx);
		frustum.front = rotation_x.Mul(frustum.front).Normalized();
		frustum.up = rotation_x.Mul(frustum.up).Normalized();
	}

	if (dy != 0)
	{
		Quat rotation_y = Quat::RotateAxisAngle(frustum.WorldRight(), dy);

		float3 new_up = rotation_y.Mul(frustum.up).Normalized();

		if (new_up.y > 0.0f)
		{
			frustum.up = new_up;
			frustum.front = rotation_y.Mul(frustum.front).Normalized();
		}
	}
}

float* Component_Camera::GetViewMatrix() const 
{
	float4x4 matrix = frustum.ViewMatrix();
	return (float*)matrix.Transposed().v;
}

float* Component_Camera::GetProjectionMatrix() const
{
	return (float*)frustum.ProjectionMatrix().Transposed().v;
}

const float3 Component_Camera::GetPosition()
{
	return frustum.pos;
}

//void Component_Camera::OnSave(JSON_Doc& config)
//{
//	
//}

void Component_Camera::OnLoad(JSON_Doc * config)
{
	active_camera = config->GetBool("Active");
	fov = (float)config->GetNumber("FOV");
	aspect_ratio = (float)config->GetNumber("Aspect Ratio");
}

// COMPONENT MESH ==================================================

Component_Mesh::Component_Mesh(uint num_ver, float * ver, uint num_ind, uint * ind, uint num_uv, float * uv, uint num_norm, float * norm) : Component(COMPONENT_MESH)
{
	//Load vertices to vram
	glGenBuffers(1, (GLuint*)&id_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices * 3, ver, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Load indices to vram
	glGenBuffers(1, (GLuint*)&id_indices);
	glBindBuffer(GL_ARRAY_BUFFER, id_indices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * num_indices, ind, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Load uv to vram
	if (uv != nullptr)
	{
		glGenBuffers(1, (GLuint*)&id_uvs);
		glBindBuffer(GL_ARRAY_BUFFER, id_uvs);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_uv * 3, uv, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	bounding_box.SetNegativeInfinity();
	bounding_box.Enclose((float3*)ver, num_ver);
}

void Component_Mesh::Update()
{
	App->debug->DrawBoundingBox(bounding_box.CenterPoint(), bounding_box.Size(), 3, Red);
}

void Component_Mesh::AdaptBoundingBox(GameObject* go, Component_Transform* trans)
{
	bounding_box.SetNegativeInfinity();
	bounding_box.Enclose((float3*)vertices, num_vertices);
	bounding_box.TransformAsAABB(go->GetGlobalTransform(trans).Transposed());
}

void Component_Mesh::OnSave(JSON_Doc& config)
{

}

void Component_Mesh::OnLoad(JSON_Doc * config)
{

}

//COMPONENT TRANSFORM===================================

/*void Component_Transform::OnSave(JSON_Doc& config)
{
	config->SetEntry("Components");
	config->MoveToSectionFromArray("Components", config->GetArraySize("Components") - 1);

	config->SetInt("type", GetComponentType());
	config->SetInt("owner", GetComponentParent()->unique_id);
	config->SetFloat3("position", GetTransform());
	config->SetFloat3("scale", GetScale());
	config->SetQuaternion("rotation", GetRotation());
	int a;
}*/

void Component_Transform::OnLoad(JSON_Doc * config)
{
	SetPosition(float3(config->GetNumber("pos_x"), config->GetNumber("pos_y"), config->GetNumber("pos_z")));
	SetScale(float3(config->GetNumber("scale_x"), config->GetNumber("scale_y"), config->GetNumber("scale_z")));
}

//COMPONENT MATERIAL====================================

void Component_Material::OnSave(JSON_Doc& config)
{
	config.SetNumber("Texture ID", id_texture);
	config.SetNumber("Height", height);
	config.SetNumber("Width", width);
	config.SetString("Path", path.c_str());
}

void Component_Material::OnLoad(JSON_Doc * config)
{
	id_texture = config->GetNumber("Texture ID");
	height = config->GetNumber("Height");
	width = config->GetNumber("Width");
	path = config->GetNumber("Path");
}