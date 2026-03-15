#include "platform.h"
#include "sve.h"

extern uint64_t tick_ns;

static int running = 1;

static void on_signal(void) { running = 0; }

int main(int argc, char *argv[])
{
	uint64_t last;
	uint64_t now;
	uint64_t elapsed;
	sve_config_t config = sve_config_default();

	if (sve_init(config) != SVE_INIT_SUCCESS)
		return 1;

	sve_platform_init(on_signal);

	last = sve_time_ns();

	while (running) {
		now = sve_time_ns();
		elapsed = now - last;

		if (elapsed < tick_ns) {
			sve_sleep_ns(tick_ns - elapsed);
			continue;
		}

		sve_tick();
		/* TODO: networking, world update */
		last = now;
	}

	sve_shutdown();
	return 0;
}
