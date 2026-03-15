#ifndef SVE_PLATFORM_H
#define SVE_PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void sve_platform_init(void (*signal_handler)(void));
uint64_t sve_time_ns(void);
void sve_sleep_ns(uint64_t ns);

#ifdef __cplusplus
}
#endif

#endif /* SVE_PLATFORM_H */
