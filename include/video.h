#ifndef SVE_VIDEO_H
#define SVE_VIDEO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sve.h"
#include <stdint.h>

uint8_t sve_video_init(sve_video_config_t config);
void sve_video_show(void);
void sve_video_present(const uint32_t *pixels);
void sve_video_shutdown(void);

#ifdef __cplusplus
}
#endif

#endif /* SVE_VIDEO_H */
