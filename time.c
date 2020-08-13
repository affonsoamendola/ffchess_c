#include "SDL2/SDL.h"
#include "time.h"

static uint64_t frame_timer_start = 0;
static uint64_t frame_timer_end = 0;

static double frame_time = 0.0f;

//Sets the start of the frame timer
void set_frame_timer_start()
{
	frame_timer_start = SDL_GetPerformanceCounter();
}

//Sets the end of the frame timer
void set_frame_timer_end()
{
	frame_timer_end = SDL_GetPerformanceCounter();
	frame_time = (frame_timer_end - frame_timer_start) / (double)SDL_GetPerformanceFrequency();
}

//Gets how long lasted the last frame, in seconds
double get_frame_time()
{
	return frame_time;
}