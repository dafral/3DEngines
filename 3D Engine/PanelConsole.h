#pragma once

#ifndef __PANEL_CONSOLE__
#define __PANEL_CONSOLE__

#include "Panel.h"
#include "Imgui\imgui.h"

class PanelConsole : public Panel {
public:
	PanelConsole();
	virtual ~PanelConsole();

	void Draw();

	void ConsoleText(const char* log);

public:
	ImGuiTextBuffer text;

};

#endif // !__PANEL_CONSOLE_H__