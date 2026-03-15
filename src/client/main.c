#include "input.h"
#include "platform.h"
#include "sve.h"
#include "video.h"
#include <string.h>

extern uint64_t tick_ns;

static int running = 1;

static void on_signal(void) { running = 0; }

int main(int argc, char *argv[])
{
	uint64_t last;
	uint64_t now;
	uint64_t elapsed;
	sve_config_t config = sve_config_default();
	uint32_t pixels[config.video.render_width * config.video.render_height];

	memset(pixels, 0,
	       config.video.render_width * config.video.render_height *
		   sizeof(uint32_t));

	if (sve_init(config) != SVE_INIT_SUCCESS)
		return 1;

	sve_platform_init(on_signal);

	if (sve_video_init(config.video) != SVE_INIT_SUCCESS) {
		sve_shutdown();
		return 1;
	}

	last = sve_time_ns();
	sve_video_show();

	while (running) {
		sve_input_poll(&running);

		now = sve_time_ns();
		elapsed = now - last;

		if (elapsed < tick_ns) {
			sve_sleep_ns(tick_ns - elapsed);
			continue;
		}

		sve_tick();
		sve_video_present(pixels);
		last = now;
	}

	sve_video_shutdown();
	sve_shutdown();
	return 0;
}
