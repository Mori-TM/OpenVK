#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define ARRAY_SIZE(x)  (sizeof(x) / sizeof((x)[0]))
#define SWAP(T, a, b) do { T tmp = a; a = b; b = tmp; } while (0)

#define true 1
#define false 0
#ifndef __cplusplus
typedef uint32_t bool;
#endif

#define E 2.71828182846
#define PI 3.141592653589793238
#define PI2 6.28318530718

SDL_Window* Window;
SDL_Event Event;

uint32_t WindowWidth = 1280;
uint32_t WindowHeight = 720;

float DeltaTime = 0.0;
float LastTime = 0.0;

float GetDeltaTime()
{
	float CurrentTime = (float)SDL_GetTicks() / 1000.0;
	DeltaTime = CurrentTime - LastTime;
	LastTime = CurrentTime;

	return DeltaTime;
}

double GetExecutionTime(void (*Func)(void))
{
	struct timespec TimerStartTime;
	struct timespec TimerEndTime;

	timespec_get(&TimerStartTime, TIME_UTC);
	Func();
	timespec_get(&TimerEndTime, TIME_UTC);

	double Time = (double)((TimerEndTime.tv_sec) * 1000.0 + (TimerEndTime.tv_nsec) / 1000000.0) - ((TimerStartTime.tv_sec) * 1000.0 + (TimerStartTime.tv_nsec) / 1000000.0);

	return Time;
}