#pragma once

#ifndef __PANEL_CONFIG_H__
#define __PANEL_CONFIG_H__

#include "Panel.h"
#include "Imgui\imgui.h"

class PanelConfig : public Panel {
public:
	PanelConfig();
	virtual ~PanelConfig();

	void Draw();
};

#endif