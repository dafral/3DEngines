#include "DebugDraw.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

DebugDraw::DebugDraw()
{}

DebugDraw::~DebugDraw()
{}

void DebugDraw::Draw(float3* vertices, int num_vertices) const
{
	glLineWidth(line_width);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertexPointer(3, GL_FLOAT, 0, (float*)vertices->ptr());
	glDrawArrays(GL_LINES, 0, num_vertices);
	glDisableClientState(GL_COLOR);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void DebugDraw::DrawFrustum(float3* corners)
{
	int num_vertices = 24;

	float3* vertices = new float3[num_vertices];

	vertices[0] = float3(corners[0].x, corners[0].y, corners[0].z);
	vertices[1] = float3(corners[2].x, corners[2].y, corners[2].z);

	vertices[2] = float3(corners[2].x, corners[2].y, corners[2].z);
	vertices[3] = float3(corners[6].x, corners[6].y, corners[6].z);

	vertices[4] = float3(corners[4].x, corners[4].y, corners[4].z);
	vertices[5] = float3(corners[6].x, corners[6].y, corners[6].z);

	vertices[6] = float3(corners[4].x, corners[4].y, corners[4].z);
	vertices[7] = float3(corners[0].x, corners[0].y, corners[0].z);

	vertices[8] = float3(corners[1].x, corners[1].y, corners[1].z);
	vertices[9] = float3(corners[3].x, corners[3].y, corners[3].z);

	vertices[10] = float3(corners[3].x, corners[3].y, corners[3].z);
	vertices[11] = float3(corners[7].x, corners[7].y, corners[7].z);

	vertices[12] = float3(corners[5].x, corners[5].y, corners[5].z);
	vertices[13] = float3(corners[7].x, corners[7].y, corners[7].z);

	vertices[14] = float3(corners[5].x, corners[5].y, corners[5].z);
	vertices[15] = float3(corners[1].x, corners[1].y, corners[1].z);

	vertices[16] = float3(corners[0].x, corners[0].y, corners[0].z);
	vertices[17] = float3(corners[1].x, corners[1].y, corners[1].z);

	vertices[18] = float3(corners[2].x, corners[2].y, corners[2].z);
	vertices[19] = float3(corners[3].x, corners[3].y, corners[3].z);

	vertices[20] = float3(corners[4].x, corners[4].y, corners[4].z);
	vertices[21] = float3(corners[5].x, corners[5].y, corners[5].z);

	vertices[22] = float3(corners[6].x, corners[6].y, corners[6].z);
	vertices[23] = float3(corners[7].x, corners[7].y, corners[7].z);

	Draw(vertices, num_vertices);

	delete[] vertices;
}

void DebugDraw::DrawBoundingBox(float3 center, float3 size)
{
	int num_vertices = 24;

	float3* vertices = new float3[num_vertices];
	float3 half_size = size / 2;

	vertices[0] = float3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z);
	vertices[1] = float3(center.x - half_size.x, center.y + half_size.y, center.z - half_size.z);

	vertices[2] = float3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z);
	vertices[3] = float3(center.x + half_size.x, center.y - half_size.y, center.z - half_size.z);

	vertices[4] = float3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z);
	vertices[5] = float3(center.x - half_size.x, center.y - half_size.y, center.z + half_size.z);

	vertices[6] = float3(center.x - half_size.x, center.y - half_size.y, center.z + half_size.z);
	vertices[7] = float3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z);

	vertices[8] = float3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z);
	vertices[9] = float3(center.x + half_size.x, center.y - half_size.y, center.z - half_size.z);

	vertices[10] = float3(center.x - half_size.x, center.y + half_size.y, center.z - half_size.z);
	vertices[11] = float3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z);

	vertices[12] = float3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z);
	vertices[13] = float3(center.x + half_size.x, center.y + half_size.y, center.z + half_size.z);

	vertices[14] = float3(center.x + half_size.x, center.y + half_size.y, center.z + half_size.z);
	vertices[15] = float3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z);

	vertices[16] = float3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z);
	vertices[17] = float3(center.x - half_size.x, center.y + half_size.y, center.z - half_size.z);

	vertices[18] = float3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z);
	vertices[19] = float3(center.x - half_size.x, center.y - half_size.y, center.z + half_size.z);

	vertices[20] = float3(center.x + half_size.x, center.y + half_size.y, center.z + half_size.z);
	vertices[21] = float3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z);

	vertices[22] = float3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z);
	vertices[23] = float3(center.x + half_size.x, center.y - half_size.y, center.z - half_size.z);

	Draw(vertices, num_vertices);

	delete[] vertices;
}