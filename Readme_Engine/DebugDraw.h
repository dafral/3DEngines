#include "MathGeoLib/MathGeoLib.h"

struct Color;

class DebugDraw 
{
public:
	DebugDraw();
	~DebugDraw();

private:
	void Draw(float3 * line_points, int size, float line_width, Color color) const;

public:
	void DrawFrustum(float3* vertices, float line_width, Color color);
	void DrawBoundingBox(float3 center, float3 size, float line_width, Color color);
};
