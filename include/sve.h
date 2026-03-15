#ifndef SVE_H
#define SVE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define SVE_INIT_SUCCESS 0
#define SVE_INIT_FAILURE 1

typedef enum {
	SVE_VIDEO_FULLSCREEN = 0x01,
	SVE_VIDEO_VSYNC = 0x02,
	SVE_VIDEO_BORDERLESS = 0x04,
	SVE_VIDEO_SOFTWARE = 0x08,
	SVE_VIDEO_RESIZABLE = 0x10,
	SVE_VIDEO_HIGHDPI = 0x20,
	SVE_VIDEO_GRAB_MOUSE = 0x40,
} sve_video_flags_t;

typedef struct {
	uint16_t window_width;
	uint16_t window_height;
	uint16_t render_width;
	uint16_t render_height;
	uint16_t max_fps;
	sve_video_flags_t flags;
} sve_video_config_t;

typedef struct {
	uint8_t tick_rate;
} sve_engine_config_t;

typedef struct {
	sve_engine_config_t engine;
	sve_video_config_t video;
} sve_config_t;

sve_config_t sve_config_default(void);
uint8_t sve_init(sve_config_t config);
void sve_tick(void);
uint64_t sve_tick_ns(void);
void sve_shutdown(void);

#ifdef __cplusplus
}
#endif

#endif /* SVE_H */
