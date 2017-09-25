#pragma once

#ifndef __PANEL_CONFIG_H__
#define __PANEL_CONFIG_H__

#include "Panel.h"
#include "Globals.h"
#include "Imgui\imgui.h"

class PanelConfig : public Panel {
public:
	PanelConfig();
	virtual ~PanelConfig();

	void Draw();

private:
	bool resizable, fullscreen, borderless, full_desktop;
	float brightness;
};

#endif