#include "input.h"
#include <SDL3/SDL.h>

void sve_input_poll(int *running)
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_EVENT_QUIT)
			*running = 0;
	}
}
