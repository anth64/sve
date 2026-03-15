#include "platform.h"
#include "sve.h"

extern uint64_t s_tick_ns;

int main(int argc, char *argv[])
{
	int running = 1;
	uint64_t last;
	uint64_t now;
	uint64_t elapsed;
	sve_config_t config = sve_config_default();

	if (sve_init(config) != SVE_INIT_SUCCESS)
		return 1;

	last = sve_time_ns();

	while (running) {
		now = sve_time_ns();
		elapsed = now - last;

		if (elapsed < s_tick_ns) {
			sve_sleep_ns(s_tick_ns - elapsed);
			continue;
		}

		sve_tick();
		/* TODO: input, render */
		last = now;
	}

	sve_shutdown();
	return 0;
}
