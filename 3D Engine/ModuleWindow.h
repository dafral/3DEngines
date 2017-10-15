#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);

	void SwitchResizable();
	void SwitchBorderless(bool borderless);
	void SwitchFullScreen(bool full_screen);

	float GetBrightness();
	void SetBrightness(float brightness);

	int GetMinWidth();
	int GetMinHeight();
	int GetMaxWidth();
	int GetMaxHeight();

	int GetWidth();
	int GetHeight();
	void SetWidth(int width);
	void SetHeight(int height);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

	bool resizable = false;

};

#endif // __ModuleWindow_H__