#include "platform.h"
#include "sve.h"

int main(int argc, char *argv[])
{
	int running = 1;
	uint64_t last;
	uint64_t now;
	uint64_t elapsed;

	if (sve_init() != SVE_INIT_SUCCESS)
		return 1;

	last = sve_time_ns();

	while (running) {
		now = sve_time_ns();
		elapsed = now - last;

		if (elapsed >= SVE_TICK_NS) {
			sve_tick();
			/* TODO: networking, world update */
			last = now;
		} else {
			sve_sleep_ns(SVE_TICK_NS - elapsed);
		}
	}

	sve_shutdown();
	return 0;
}
