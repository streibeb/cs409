//
//  TimeSystem.cpp
//
//  DO NOT MODIFY THIS FILE
//

#include <cassert>
#include <cstdlib>  // for NULL

// for time representation types
#ifdef _WIN32
	#include <windows.h>
#elif __APPLE__
	#include <sys/time.h>
#else	// POSIX
	#include <time.h>
#endif

#include "TimeSystem.h"

namespace
{
	const float FRAME_DURATION_DESURED_DEFAULT = 1.0f;
	const float FRAME_DURATION_MAX_DEFAULT     = 1.0f;
	const float FRAME_SMOOTHING_FACTOR_DEFAULT = 1.0f;

	const float TIME_COUNTER_RATE_INVERSE_DEFAULT = 0.001f;
}



const float TimeSystem :: AI_TIME_NOT_INITIALIZED = -1.0e20f;

bool TimeSystem :: ms_is_initialized = false;

unsigned int TimeSystem :: ms_frame_number      = 0;
float TimeSystem :: ms_frame_time_current       = 0.0f;
float TimeSystem :: ms_frame_duration_max       = FRAME_DURATION_MAX_DEFAULT;
float TimeSystem :: ms_frame_duration_desired   = FRAME_DURATION_DESURED_DEFAULT;
float TimeSystem :: ms_smoothing_factor         =        FRAME_SMOOTHING_FACTOR_DEFAULT;
float TimeSystem :: ms_smoothing_factor_inverse = 1.0f - FRAME_SMOOTHING_FACTOR_DEFAULT;
float TimeSystem :: ms_frame_duration_current   = FRAME_DURATION_DESURED_DEFAULT;

float TimeSystem :: ms_pause_duration = 0.0f;

float TimeSystem :: ms_ai_time_start = AI_TIME_NOT_INITIALIZED;
float TimeSystem :: ms_ai_time_max   = AI_TIME_NOT_INITIALIZED;

#ifdef _WIN32
	float TimeSystem :: ms_time_counter_rate_inverse = TIME_COUNTER_RATE_INVERSE_DEFAULT;
	LARGE_INTEGER TimeSystem :: ms_start_time_win;
#elif __APPLE__
	timeval TimeSystem :: ms_start_time_apple;
#else	// POSIX
	double TimeSystem :: ms_start_time_posix;
#endif



float TimeSystem :: getTimeToNextFrame ()
{
	assert(isInitialized());

	float time_current = calculateCurrentTime();
	float time_frame_next = ms_frame_time_current + ms_frame_duration_desired;

	if(time_frame_next <= time_current)
		return 0.0f;
	else
	{
		assert(time_current < time_frame_next);
		return time_frame_next - time_current;
	}
}

float TimeSystem :: getAiTimeOvershot ()
{
	assert(isInitialized());
	assert(isAiInitialized());

	float current = calculateCurrentTime();
	if(current <= ms_ai_time_max)
		return 0.0f;
	else
	{
		assert(ms_ai_time_max < current);
		return current - ms_ai_time_max;
	}
}




void TimeSystem :: init (float minimum_frames_per_second,
                         float desired_frames_per_second,
                         float smoothing_factor)
{
	assert(minimum_frames_per_second >  0.0f);
	assert(minimum_frames_per_second <= desired_frames_per_second);
	assert(smoothing_factor >  0.0f);
	assert(smoothing_factor <= 1.0f);

#ifdef _WIN32
	LARGE_INTEGER time_counter_rate;
	QueryPerformanceFrequency(&time_counter_rate);
	ms_time_counter_rate_inverse = 1.0f / time_counter_rate.QuadPart;

	QueryPerformanceCounter(&ms_start_time_win);
#elif __APPLE__
	gettimeofday(&ms_start_time_apple, NULL);
#else	// POSIX
	timespec spec;
	clock_gettime(CLOCK_REALTIME, &spec);
	ms_start_time_posix = spec.tv_sec + spec.tv_nsec * 0.000000001;
#endif

	ms_frame_number       = 0;
	ms_frame_time_current = 0.0f;

	ms_frame_duration_max     = 1.0f / minimum_frames_per_second;
	ms_frame_duration_desired = 1.0f / desired_frames_per_second;
	ms_frame_duration_current = ms_frame_duration_desired;

	ms_smoothing_factor         =        smoothing_factor;
	ms_smoothing_factor_inverse = 1.0f - smoothing_factor;

	ms_pause_duration = 0.0f;

	ms_ai_time_max = AI_TIME_NOT_INITIALIZED;

	ms_is_initialized = true;

	assert(invariant());
}

void TimeSystem :: markFrameEnd ()
{
	assert(isInitialized());

	ms_frame_number++;

	float frame_time_previous = ms_frame_time_current;
	ms_frame_time_current = calculateCurrentTime();

	float frame_duration_last = ms_frame_time_current - frame_time_previous;
	ms_frame_duration_current = frame_duration_last       * ms_smoothing_factor +
	                            ms_frame_duration_current * ms_smoothing_factor_inverse;
	if(ms_frame_duration_current > ms_frame_duration_max)
		ms_frame_duration_current = ms_frame_duration_max;

	assert(invariant());
}

void TimeSystem :: markPauseEnd ()
{
	float time_current = calculateCurrentTime();
	assert(ms_frame_time_current <= time_current);

	ms_pause_duration     = time_current - ms_frame_time_current;
	ms_frame_time_current = time_current;

	assert(invariant());
}

void TimeSystem :: markAiStart (float time_max)
{
	assert(isInitialized());
	assert(time_max >= 0.0);

	ms_ai_time_start = calculateCurrentTime();
	ms_ai_time_max   = ms_ai_time_start + time_max;
}



//
//  Default Constructor
//  Copy Constructor
//  Destructor
//  Assignment Operator
//
//  These functions have intentionally not been implemented.
//
//  TimeSystem :: TimeSystem ();
//  TimeSystem :: TimeSystem (const TimeSystem& original);
//  TimeSystem :: ~TimeSystem ();
//  TimeSystem& TimeSystem :: operator= (const TimeSystem& original);
//

float TimeSystem :: calculateCurrentTime ()
{
#ifdef _WIN32
	static LARGE_INTEGER current_time_win;
	QueryPerformanceCounter(&current_time_win);
	return (current_time_win.QuadPart -
	        ms_start_time_win.QuadPart) * ms_time_counter_rate_inverse;
#elif __APPLE__
	timeval current_time;
	gettimeofday(&current_time, NULL);
	int seconds_diff = current_time.tv_sec  - ms_start_time_apple.tv_sec;
	assert(seconds_diff >= 0);
	int micro_diff   = current_time.tv_usec - ms_start_time_apple.tv_usec;
	assert(micro_diff > -1000000);
	assert(micro_diff <  1000000);
	return (float)(seconds_diff + micro_diff * 0.000001f);
#else	// POSIX
	timespec spec;
	clock_gettime(CLOCK_REALTIME, &spec);
	double current_time_seconds = spec.tv_sec + spec.tv_nsec * 0.000000001;
	return (float)(current_time_seconds - ms_start_time_posix);
#endif
}

bool TimeSystem :: invariant ()
{
	if(ms_frame_time_current     < 0.0f) return false;
	if(ms_frame_duration_max     <= 0.0f) return false;
	if(ms_frame_duration_desired >  ms_frame_duration_max) return false;
	if(ms_frame_duration_current >  ms_frame_duration_max) return false;
	if(ms_smoothing_factor <= 0.0f) return false;
	if(ms_smoothing_factor >  1.0f) return false;
	if(ms_smoothing_factor_inverse != (float)(1.0f - ms_smoothing_factor)) return false;
	if(ms_pause_duration < 0.0f) return false;
	if((ms_ai_time_start == AI_TIME_NOT_INITIALIZED) != (ms_ai_time_max == AI_TIME_NOT_INITIALIZED)) return false;
#ifdef _WIN32
	if(ms_time_counter_rate_inverse <= 0.0f) return false;
#endif

	return true;
}
