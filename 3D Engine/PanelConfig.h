#pragma once

#ifndef __PANEL_CONFIG_H__
#define __PANEL_CONFIG_H__

#include "Panel.h"
#include "Globals.h"
#include "Imgui\imgui.h"
#include <vector>

#define FPS_LOG_NUM 25

class PanelConfig : public Panel 
{
public:
	PanelConfig();
	virtual ~PanelConfig();

	void Draw();
	void AddFps(float fps);

private:
	bool resizable, fullscreen, borderless, full_desktop;
	float brightness;
	std::vector<float> framerate;
};

#endif