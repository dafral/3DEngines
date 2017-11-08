#include "Application.h"
#include "ModuleInput.h"
#include "Component.h"

Component_Camera::Component_Camera() : Component(COMPONENT_CAMERA) 
{
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.nearPlaneDistance = 0.5f;
	frustum.farPlaneDistance = 500.f;

	frustum.verticalFov = DEGTORAD * 60.0f;
	frustum.horizontalFov = SCREEN_WIDTH / SCREEN_HEIGHT;
	aspect_ratio = SCREEN_WIDTH / SCREEN_HEIGHT;


	//frustum.Type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
};

void Component_Camera::Update()
{
	//Getting frustum coords
	float3 corners[8];
	frustum.GetCornerPoints(corners);

	App->debug->DrawFrustum(corners);
	
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		SetFOV(179);

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		SetFOV(10);
}

void Component_Camera::SetFOV(float FOV)
{
	//if (FOV < 1) {
		fov = FOV;
		frustum.verticalFov = DEGTORAD * FOV;
		frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov / 2.0f) * aspect_ratio);
	//}
}

// COMPONENT MESH ==================================================

void Component_Mesh::Update()
{
	App->debug->DrawBoundingBox(bounding_box.CenterPoint(), bounding_box.Size());
}

