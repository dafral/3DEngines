#pragma once

#ifndef __PANEL_ABOUT_H__
#define __PANEL_ABOUT_H__

#include "Panel.h"
#include "Imgui/imgui.h"

class PanelAbout : public Panel 
{
public:
	PanelAbout(bool active);
	virtual ~PanelAbout();

	void Draw();
};

#endif