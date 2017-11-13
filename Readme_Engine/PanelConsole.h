#pragma once

#ifndef __PANEL_CONSOLE__
#define __PANEL_CONSOLE__

#include "Panel.h"
#include "Imgui/imgui.h"

class PanelConsole : public Panel {
public:
	PanelConsole(bool active);
	virtual ~PanelConsole();

	void Draw();
	void AdjustPanel();
	void ConsoleText(const char* log);
};

#endif // !__PANEL_CONSOLE_H__