#ifndef SVE_H
#define SVE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define SVE_INIT_SUCCESS 0
#define SVE_INIT_FAILURE 1

typedef struct {
	uint8_t tick_rate;
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
