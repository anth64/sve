#include "platform.h"
#include <windows.h>

static LARGE_INTEGER freq;
static HANDLE timer;
static void (*sve_handler)(void) = NULL;

static BOOL WINAPI ctrl_handler(DWORD type)
{
	(void)type;
	if (sve_handler)
		sve_handler();
	return TRUE;
}

void sve_platform_init(void (*signal_handler)(void))
{
	sve_handler = signal_handler;
	QueryPerformanceFrequency(&freq);
	timer = CreateWaitableTimer(NULL, TRUE, NULL);
	SetConsoleCtrlHandler(ctrl_handler, TRUE);
}

uint64_t sve_time_ns(void)
{
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	return (uint64_t)(counter.QuadPart * 1000000000ULL / freq.QuadPart);
}

void sve_sleep_ns(uint64_t ns)
{
	LARGE_INTEGER due;
	due.QuadPart = -(LONGLONG)(ns / 100);
	SetWaitableTimer(timer, &due, 0, NULL, NULL, 0);
	WaitForSingleObject(timer, INFINITE);
}
