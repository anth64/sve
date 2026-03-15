#ifndef SVE_H
#define SVE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define SVE_INIT_SUCCESS 0
#define SVE_INIT_FAILURE 1

#define SVE_TICK_RATE 35
#define SVE_TICK_NS (1000000000ULL / SVE_TICK_RATE)

uint8_t sve_init(void);
void sve_tick(void);
void sve_shutdown(void);

#ifdef __cplusplus
}
#endif

#endif /* SVE_H */
