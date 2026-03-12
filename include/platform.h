#ifndef SVE_PLATFORM_H
#define SVE_PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

uint64_t sve_time_ms(void);
void sve_sleep_ms(uint64_t ms);

#ifdef __cplusplus
}
#endif

#endif /* SVE_PLATFORM_H */
