#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class Component_Camera;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application * app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	float* GetViewMatrix();
	float* GetProjectionMatrix();

	void LookAt(const float3 &spot);
	Component_Camera* GetActiveCam();
	void SetActiveCam(Component_Camera* cam);
	const float GetSensitivity() const { return sensitivity; };
	void SetSensitivity(float new_sensitivity) { sensitivity = new_sensitivity; };
	void CameraZoom(int mouse_z, float dt);

private:
	void RotateCamera(float dt);
	void MoveCamera(float dt);

public:
	Component_Camera* curr_camera;
	Component_Camera* main_camera;

	float3 orbitate_around = float3::zero;
	bool orb_x_inverted, orb_y_inverted, wheel_inverted;
	float sensitivity;
};