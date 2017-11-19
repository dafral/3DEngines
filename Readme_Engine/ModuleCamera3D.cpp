#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "Component.h"
#include "GameObject.h"
#include "glmath.h"
#include "MathGeoLib\MathGeoLib.h"
#include "MathGeoLib\Geometry\Frustum.h"

ModuleCamera3D::ModuleCamera3D(Application * app, bool start_enabled) : Module(app, start_enabled)
{
	orb_x_inverted = orb_y_inverted = wheel_inverted = false;
	sensitivity = 0.25f;

	GameObject* empty = App->scene->CreateGameObject("Main camera", App->scene->root);
	main_camera = new Component_Camera();
	SetActiveCam(main_camera);
	empty->AddComponent(main_camera);
}

ModuleCamera3D::~ModuleCamera3D()
{}

bool ModuleCamera3D::Start()
{
	CONSOLELOG("Setting up the camera");
	App->scene->root->AddComponent(main_camera);
	bool ret = true;
	return ret;
}

bool ModuleCamera3D::CleanUp()
{
	CONSOLELOG("Cleaning camera");

	delete main_camera;
	curr_camera = nullptr;

	return true;
}

update_status ModuleCamera3D::Update(float dt)
{
	if (!ImGui::IsAnyWindowHovered())
	{
		// Mouse motion ----------------

		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			MoveCamera(dt);
			RotateCamera(dt);
		}

		if (ImGui::IsMouseDoubleClicked(0))
		{
			App->picker->RayCast();
		}
	}
	return UPDATE_CONTINUE;
}

float* ModuleCamera3D::GetViewMatrix()
{
	return curr_camera->GetViewMatrix();
}

float* ModuleCamera3D::GetProjectionMatrix()
{
	return curr_camera->GetProjectionMatrix();
}

void ModuleCamera3D::LookAt(const float3 & spot)
{
	float3 direction = spot - curr_camera->GetFrustum().pos;

	float3x3 matrix = float3x3::LookAt(curr_camera->GetFrustum().front, direction.Normalized(), curr_camera->GetFrustum().up, float3::unitY);

	curr_camera->GetFrustum().front = matrix.MulDir(curr_camera->GetFrustum().front).Normalized();
	curr_camera->GetFrustum().up = matrix.MulDir(curr_camera->GetFrustum().up).Normalized();
}

void ModuleCamera3D::RotateCamera(float dt)
{
	float dx = 0.0f, dy = 0.0f;

	orb_x_inverted ? dx = App->input->GetMouseXMotion() * sensitivity * dt : dx = -App->input->GetMouseXMotion() * sensitivity * dt;
	orb_y_inverted ? dy = App->input->GetMouseYMotion() * sensitivity * dt : dy = -App->input->GetMouseYMotion() * sensitivity * dt;

	curr_camera->Rotate(dx, dy);
}

void ModuleCamera3D::MoveCamera(float dt)
{
	float speed = 5.0f * dt;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 10.0f * dt;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) curr_camera->MoveForwards(speed);
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) curr_camera->MoveBackwards(speed);

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) curr_camera->MoveLeft(speed);
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) curr_camera->MoveRight(speed);

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) curr_camera->MoveUp(speed);
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) curr_camera->MoveDown(speed);
}

void ModuleCamera3D::CameraZoom(int mouse_z, float dt)
{
	float speed = 40.0f;

	if (mouse_z == 1)
		wheel_inverted ? main_camera->MoveForwards(speed * -sensitivity * dt) : main_camera->MoveForwards(speed *sensitivity * dt);
	else if (mouse_z == -1)
		wheel_inverted ? main_camera->MoveForwards(speed *sensitivity * dt) : main_camera->MoveForwards(speed *-sensitivity * dt);
}

Component_Camera* ModuleCamera3D::GetActiveCam()
{
	return curr_camera;
}

void ModuleCamera3D::SetActiveCam(Component_Camera* cam)
{
	if (cam->active_camera == false)
	{
		main_camera->SwitchActive(true);
		curr_camera = main_camera;
	}
	else
	{
		// Setting to active = false the older camera
		if (curr_camera != nullptr) curr_camera->SwitchActive(false);

		// New camera
		curr_camera = cam;
	}
}