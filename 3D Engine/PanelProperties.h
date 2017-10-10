#pragma once

#ifndef __PANEL_PROPERTIES__
#define __PANEL_PROPERTIES__

#include "Panel.h"
#include "Imgui\imgui.h"

class PanelProperties : public Panel {
public:
	PanelProperties();
	virtual ~PanelProperties();

	void Draw();

	
};

#endif // !__PANEL_CONSOLE_H__