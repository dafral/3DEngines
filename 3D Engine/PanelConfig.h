#pragma once

#ifndef __PANEL_CONFIG_H__
#define __PANEL_CONFIG_H__

#include "Panel.h"
#include "Globals.h"
#include "Imgui\imgui.h"
#include <vector>

#define FPS_LOG_NUM 50
#define MS_LOG_NUM 50
#define MEMORY_LOG_NUM 50

class PanelConfig : public Panel 
{
public:
	PanelConfig();
	virtual ~PanelConfig();

	void Draw();
	void AddFps(float fps);
	void AddMs(float ms);
	void AddMemory(uint mem);
	int GetFPSCap();

private:
	bool resizable, fullscreen, borderless, vsync;
	float brightness;
	int fps_cap = 0;
	std::vector<float> fps_buffer;
	std::vector<float> ms_buffer;
	std::vector<float> memory_buffer;

	void DrawApplication();
	void DrawWindow();
	void DrawHardware();
	void DrawRenderer();
	void DrawInput();
<<<<<<< HEAD
	void DrawTexture();
=======
>>>>>>> origin/master
};

#endif