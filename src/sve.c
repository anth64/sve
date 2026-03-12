#include "sve.h"
#include <stdio.h>
#include <stk/stk.h>

uint8_t sve_init(void)
{
	if (stk_init() != STK_INIT_SUCCESS) {
		fprintf(stderr, "sve: failed to initialize stk\n");
		return SVE_INIT_FAILURE;
	}
	return SVE_INIT_SUCCESS;
}

void sve_tick(void) { stk_poll(); }

void sve_shutdown(void) { stk_shutdown(); }
