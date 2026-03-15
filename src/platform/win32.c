#include "platform.h"
#include <windows.h>

static LARGE_INTEGER s_freq;
static HANDLE s_timer;

void sve_platform_init(void)
{
	QueryPerformanceFrequency(&s_freq);
	s_timer = CreateWaitableTimer(NULL, TRUE, NULL);
}

uint64_t sve_time_ns(void)
{
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	return (uint64_t)(counter.QuadPart * 1000000000ULL / s_freq.QuadPart);
}

void sve_sleep_ns(uint64_t ns)
{
	LARGE_INTEGER due;
	due.QuadPart = -(LONGLONG)(ns / 100);
	SetWaitableTimer(s_timer, &due, 0, NULL, NULL, 0);
	WaitForSingleObject(s_timer, INFINITE);
}
