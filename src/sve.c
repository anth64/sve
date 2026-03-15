#include "sve.h"
#include "platform.h"
#include <stdio.h>
#include <stk/stk.h>

uint64_t s_tick_ns = 0;

sve_config_t sve_config_default(void)
{
	sve_config_t config;
	config.tick_rate = 35;
	return config;
}

uint8_t sve_init(sve_config_t config)
{
	s_tick_ns = 1000000000ULL / config.tick_rate;
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
