#define _POSIX_C_SOURCE 199309L
#include <sve/platform.h>

#if defined(_WIN32)
#include <windows.h>

uint64_t sve_time_ms(void) { return (uint64_t)GetTickCount64(); }

void sve_sleep_ms(uint64_t ms) { Sleep((DWORD)ms); }

#else
#include <time.h>

uint64_t sve_time_ms(void)
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (uint64_t)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

void sve_sleep_ms(uint64_t ms)
{
	struct timespec ts;
	ts.tv_sec = (time_t)(ms / 1000);
	ts.tv_nsec = (long)((ms % 1000) * 1000000);
	nanosleep(&ts, NULL);
}

#endif
