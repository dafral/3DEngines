#include "Application.h"
#include "ModuleInput.h"
#include "Color.h"
#include "Component.h"
#include "MathGeoLib\MathGeoLib.h"

#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

Component_Camera::Component_Camera() : Component(COMPONENT_CAMERA) 
{
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.nearPlaneDistance = 1.0f;
	frustum.farPlaneDistance = 50.0;

	frustum.verticalFov = DEGTORAD * 60.0f;
	frustum.horizontalFov = SCREEN_WIDTH / SCREEN_HEIGHT;
	aspect_ratio = SCREEN_WIDTH / SCREEN_HEIGHT;

	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;

	SetAspectRatio(1.3f);
	SetFOV(30);


};

void Component_Camera::Update()
{
	//Getting frustum coords
	float3 corners[8];
	frustum.GetCornerPoints(corners);

	int size = 24;
	float3* lines = new float3[size];

	lines[0] = float3(corners[0].x, corners[0].y, corners[0].z);
	lines[1] = float3(corners[2].x, corners[2].y, corners[2].z);

	lines[2] = float3(corners[2].x, corners[2].y, corners[2].z);
	lines[3] = float3(corners[6].x, corners[6].y, corners[6].z);

	lines[4] = float3(corners[4].x, corners[4].y, corners[4].z);
	lines[5] = float3(corners[6].x, corners[6].y, corners[6].z);

	lines[6] = float3(corners[4].x, corners[4].y, corners[4].z);
	lines[7] = float3(corners[0].x, corners[0].y, corners[0].z);

	//

	lines[8] = float3(corners[1].x, corners[1].y, corners[1].z);
	lines[9] = float3(corners[3].x, corners[3].y, corners[3].z);

	lines[10] = float3(corners[3].x, corners[3].y, corners[3].z);
	lines[11] = float3(corners[7].x, corners[7].y, corners[7].z);

	lines[12] = float3(corners[5].x, corners[5].y, corners[5].z);
	lines[13] = float3(corners[7].x, corners[7].y, corners[7].z);

	lines[14] = float3(corners[5].x, corners[5].y, corners[5].z);
	lines[15] = float3(corners[1].x, corners[1].y, corners[1].z);

	//

	lines[16] = float3(corners[0].x, corners[0].y, corners[0].z);
	lines[17] = float3(corners[1].x, corners[1].y, corners[1].z);

	lines[18] = float3(corners[2].x, corners[2].y, corners[2].z);
	lines[19] = float3(corners[3].x, corners[3].y, corners[3].z);

	lines[20] = float3(corners[4].x, corners[4].y, corners[4].z);
	lines[21] = float3(corners[5].x, corners[5].y, corners[5].z);

	lines[22] = float3(corners[6].x, corners[6].y, corners[6].z);
	lines[23] = float3(corners[7].x, corners[7].y, corners[7].z);	

	//OpenGL Drawing
	glLineWidth((float)5);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, (float*)lines->ptr());

	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(3, GL_FLOAT, 0, &White);


	glDrawArrays(GL_LINES, 0, size);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glLineWidth(1);

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		SetFOV(179);

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		SetFOV(10);

	delete[] lines;
}

void Component_Camera::SetFOV(float FOV)
{
	if (FOV > 0) {
		frustum.verticalFov = DEGTORAD * FOV;
		frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov / 2.0f) * aspect_ratio);
	}

	if (aspect_ratio > 0)
		frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect_ratio);
}

void Component_Camera::SetAspectRatio(const float& ar)
{
	aspect_ratio = ar;

	if(frustum.verticalFov > 0)
		frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect_ratio);
}

