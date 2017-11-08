#include "MathGeoLib\MathGeoLib.h"
#include "Color.h"

class DebugDraw 
{
public:
	DebugDraw();
	~DebugDraw();

private:
	void Draw(float3 * line_points, int size) const;

private:
	Color color = Red;
	float line_width = 5.0f;

public:
	void DrawFrustum(float3* vertices);
	void DrawBoundingBox(float3 center, float3 size);
};
