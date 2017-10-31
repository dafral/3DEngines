#include <stdlib.h>
#include "Application.h"
#include "Globals.h"

#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )

#include "Brofiler\Brofiler.h"
#pragma comment(lib, "Brofiler/ProfilerCore32.lib")

enum main_states
{
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

Application* App = NULL;

int main(int argc, char ** argv)
{
	CONSOLELOG("Starting game '%s'...", TITLE);

	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;

	while (state != MAIN_EXIT)
	{
		BROFILER_FRAME("MAIN");

		switch (state)
		{
		case MAIN_CREATION:

			CONSOLELOG("-------------- Application Creation --------------");
			App = new Application();
			state = MAIN_START;
			break;

		case MAIN_START:

			CONSOLELOG("-------------- Application Init --------------");
			if (App->Init() == false)
			{
				CONSOLELOG("Application Init exits with ERROR");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				CONSOLELOG("-------------- Application Update --------------");
			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				CONSOLELOG("Application Update exits with ERROR");
				state = MAIN_EXIT;
			}

			if (update_return == UPDATE_STOP)
				state = MAIN_FINISH;
		}
			break;

		case MAIN_FINISH:

			CONSOLELOG("-------------- Application CleanUp --------------");
			if (App->CleanUp() == false)
			{
				CONSOLELOG("Application CleanUp exits with ERROR");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}
	}

	delete App;
	App = NULL;
	CONSOLELOG("Exiting game '%s'...\n", TITLE);
	return main_return;
}