#include "sve.h"
#include "platform.h"
#include <stdio.h>
#include <stk/stk.h>

uint64_t s_tick_ns = 0;

sve_config_t sve_config_default(void)
{
	sve_config_t config;
	config.engine.tick_rate = 35;
	config.video.window_width = 0;
	config.video.window_height = 0;
	config.video.render_width = 320;
	config.video.render_height = 200;
	config.video.max_fps = 0;
	config.video.flags = SVE_VIDEO_VSYNC;
	return config;
}

uint8_t sve_init(sve_config_t config)
{
	s_tick_ns = 1000000000ULL / config.engine.tick_rate;
	sve_platform_init();
	if (stk_init() != STK_INIT_SUCCESS) {
		fprintf(stderr, "sve: failed to initialize stk\n");
		return SVE_INIT_FAILURE;
	}
	return SVE_INIT_SUCCESS;
}

void sve_tick(void) { stk_poll(); }

uint64_t sve_tick_ns(void) { return s_tick_ns; }

void sve_shutdown(void) { stk_shutdown(); }
