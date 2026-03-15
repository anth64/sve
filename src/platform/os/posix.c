#define _POSIX_C_SOURCE 199309L
#include "platform.h"
#include <signal.h>
#include <time.h>

static void (*sve_handler)(void) = NULL;

static void sigint_handler(int sig)
{
	(void)sig;
	if (sve_handler)
		sve_handler();
}

void sve_platform_init(void (*signal_handler)(void))
{
	struct sigaction sa;
	sve_handler = signal_handler;
	sa.sa_handler = sigint_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
}

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
