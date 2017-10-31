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

	void SetGeometryName(char* reference);
	void SetTextureName(char* reference);
	const char* GetGeometryName();

private:
	std::string geometry_name;
	std::string texture_name;
};

#endif // !__PANEL_CONSOLE_H__