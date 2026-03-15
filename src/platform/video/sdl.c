#include "video.h"
#include <SDL3/SDL.h>
#include <stdio.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;
static int stride = 0;

uint8_t sve_video_init(sve_video_config_t config)
{
	SDL_WindowFlags flags = 0;
	SDL_DisplayID display = 0;
	int window_w;
	int window_h;

	stride = config.render_width * (int)sizeof(uint32_t);

	if (config.flags & SVE_VIDEO_FULLSCREEN)
		flags |= SDL_WINDOW_FULLSCREEN;
	if (config.flags & SVE_VIDEO_BORDERLESS)
		flags |= SDL_WINDOW_BORDERLESS;
	if (config.flags & SVE_VIDEO_RESIZABLE)
		flags |= SDL_WINDOW_RESIZABLE;
	if (config.flags & SVE_VIDEO_HIGHDPI)
		flags |= SDL_WINDOW_HIGH_PIXEL_DENSITY;

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "sve_video: SDL_Init failed: %s\n",
			SDL_GetError());
		return SVE_INIT_FAILURE;
	}

	if (config.window_width && config.window_height) {
		window_w = config.window_width;
		window_h = config.window_height;
	} else {
		SDL_Rect bounds;
		display = SDL_GetPrimaryDisplay();
		if (!SDL_GetDisplayUsableBounds(display, &bounds)) {
			fprintf(stderr,
				"sve_video: SDL_GetDisplayUsableBounds failed: "
				"%s\n",
				SDL_GetError());
			SDL_Quit();
			return SVE_INIT_FAILURE;
		}
		int scale;
		if (config.render_width * 3 <= bounds.w &&
		    config.render_height * 3 <= bounds.h)
			scale = 3;
		else if (config.render_width * 2 <= bounds.w &&
			 config.render_height * 2 <= bounds.h)
			scale = 2;
		else
			scale = 1;
		window_w = config.render_width * scale;
		window_h = config.render_height * scale;
	}

	window = SDL_CreateWindow("sve", window_w, window_h,
				  flags | SDL_WINDOW_HIDDEN);
	if (!window) {
		fprintf(stderr, "sve_video: SDL_CreateWindow failed: %s\n",
			SDL_GetError());
		SDL_Quit();
		return SVE_INIT_FAILURE;
	}

	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED_DISPLAY(display),
			      SDL_WINDOWPOS_CENTERED_DISPLAY(display));

	if (config.flags & SVE_VIDEO_GRAB_MOUSE)
		SDL_SetWindowMouseGrab(window, true);

	renderer = SDL_CreateRenderer(
	    window, (config.flags & SVE_VIDEO_SOFTWARE) ? "software" : NULL);
	if (!renderer) {
		fprintf(stderr, "sve_video: SDL_CreateRenderer failed: %s\n",
			SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return SVE_INIT_FAILURE;
	}

	if (config.flags & SVE_VIDEO_VSYNC)
		SDL_SetRenderVSync(renderer, 1);

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
				    SDL_TEXTUREACCESS_STREAMING,
				    config.render_width, config.render_height);
	if (!texture) {
		fprintf(stderr, "sve_video: SDL_CreateTexture failed: %s\n",
			SDL_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return SVE_INIT_FAILURE;
	}

	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

	return SVE_INIT_SUCCESS;
}

void sve_video_show(void) { SDL_ShowWindow(window); }

void sve_video_present(const uint32_t *pixels)
{
	SDL_UpdateTexture(texture, NULL, pixels, stride);
	SDL_RenderClear(renderer);
	SDL_RenderTexture(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void sve_video_shutdown(void)
{
	if (texture) {
		SDL_DestroyTexture(texture);
		texture = NULL;
	}
	if (renderer) {
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
	}
	if (window) {
		SDL_DestroyWindow(window);
		window = NULL;
	}
	SDL_Quit();
}
