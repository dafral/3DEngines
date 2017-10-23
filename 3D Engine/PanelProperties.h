#pragma once

#ifndef __PANEL_PROPERTIES__
#define __PANEL_PROPERTIES__

#include "Panel.h"
#include "Imgui\imgui.h"
#include <string>

class PanelProperties : public Panel {
public:
	PanelProperties(bool active);
	virtual ~PanelProperties();

	void Draw();

	void SetPosition(vec3 pos);
	void SetGeometryName(char* reference);
	void SetTextureName(char* reference);
	const char* GetGeometryName();
	const char* GetTextureName();

private:
	std::string geometry_name;
	std::string texture_name;
	uint mod_x, mod_y, mod_z;
};

#endif // !__PANEL_CONSOLE_H__