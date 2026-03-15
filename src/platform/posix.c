#define _POSIX_C_SOURCE 199309L
#include "platform.h"
#include <time.h>

void sve_platform_init(void) {}

uint64_t sve_time_ns(void)
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (uint64_t)(ts.tv_sec * 1000000000ULL + ts.tv_nsec);
}

void sve_sleep_ns(uint64_t ns)
{
	struct timespec ts;
	ts.tv_sec = (time_t)(ns / 1000000000ULL);
	ts.tv_nsec = (long)(ns % 1000000000ULL);
	nanosleep(&ts, NULL);
}
