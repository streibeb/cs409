//
//  TimeSystem.h
//
//  A module to manager the current time and frame rate.
//
//  DO NOT MODIFY THIS FILE
//

#ifndef TIME_SYSTEM_H
#define TIME_SYSTEM_H

#include <cassert>

// for time representation types
#ifdef _WIN32
	#include <windows.h>
#elif __APPLE__
	#include <sys/time.h>
	// TODO: consider switching to absolute time as described here
	// https://developer.apple.com/library/mac/qa/qa1398/_index.html
#else	// POSIX
	// no includes needed
#endif



//
//  TimeSystem
//
//  A static class to keep track of the current time and frame
//    rate.  The TimeSystem can be queried to determine the
//    current fame duration, total elapsed time, and time until
//    the next frame should start.  The TimeSystem can also be
//    initialized and updated for a new frame or for the end of
//    a pause.
//
//  The current frame duration is smoothed by a factor set during
//    initialization.  This is to prevent the frame rate
//    fluctuating wildly whenever some proccessor power is
//    diverted to another task.  If the smoothing factor is set
//    to 0.2, the duration of the the most recent frame accounts
//    for 20% of the fram rate, and the durations of the
//    previous frames account for the other 80%.  Smoothing can
//    be disabled by using a smoothing factor of 1.0.
//
//  It is not possible to create a TimeSystem, and there is no
//    reason to do so.  The class functions and members are all
//    declared as static, and as such they can be accessed from
//    anywhere in the program.  The syntax is:
//
//    float time = TimeSystem::getFrameDuration();
//
//  It would be a bad idea to use TimeSystem as a base class for
//    inheritance.
//
//  This class uses system class to determine the current time,
//    and these vary by platform.  It does not require access to
//    OpenGL libraries.
//
//  Class Invariant:
//    <1> ms_frame_time_current     >= 0.0f
//    <2> ms_frame_duration_max     >  0.0f
//    <3> ms_frame_duration_desired <= ms_frame_duration_max
//    <4> ms_frame_duration_current <= ms_frame_duration_max
//    <5> ms_smoothing_factor >  0.0f
//    <6> ms_smoothing_factor <= 1.0f
//    <7> ms_smoothing_factor_inverse ==
//        1.0f - ms_smoothing_factor
//    <8> ms_pause_duration >= 0.0f
//    <9> (ms_ai_time_start == AI_TIME_NOT_INITIALIZED) ==
//        (ms_ai_time_max   == AI_TIME_NOT_INITIALIZED)
//  Windows Only:
//    <10>ms_time_counter_rate_inverse > 0.0f
//

class TimeSystem
{
public:
//
//  isInitialized
//
//  Purpose: To determine if the TimeSystem has been
//           initialized.
//  Paremeter(s): N/A
//  Precondition(s):
//    <1> isInitialized()
//  Returns: Whether the TimeSystem is initialized.
//  Side Effect: N/A
//

	static bool isInitialized ()
	{	return ms_is_initialized;	}


//
//  isAiInitialized
//
//  Purpose: To determine if the Ai time tecking system has been
//           initialized.  This can be done by calling
//           markAiStart.
//  Paremeter(s): N/A
//  Precondition(s):
//    <1> isInitialized()
//  Returns: Whether the Ai time tracking system is initialized.
//  Side Effect: N/A
//

	static bool isAiInitialized ()
	{
		assert(isInitialized());
		return (ms_ai_time_max != AI_TIME_NOT_INITIALIZED);
	}


//
//  getFrameNumber
//
//  Purpose: To determine how many frames have been completed
//           since init was called.  The modulus of this value
//           can be used to make something happen
//           periodically, such as every second frame.
//  Paremeter(s): N/A
//  Precondition(s):
//    <1> isInitialized()
//  Returns: The number for the frame.  The frame lasting from
//           when init is called to the first call of
//           markFrameEnd is numbered 0.  The nexct frame is
//           numbered 1, and so on.
//  Side Effect: N/A
//

	static unsigned int getFrameNumber ()
	{
		assert(isInitialized());
		return ms_frame_number;
	}

//
//  getFrameStartTime
//
//  Purpose: To determine the elapsed time in seconds from when
//           the TimeSystem was initialized to the current
//           frame.
//  Paremeter(s): N/A
//  Precondition(s):
//    <1> isInitialized()
//  Returns: The time for the current frame in seconds.
//  Side Effect: N/A
//

	static float getFrameStartTime ()
	{
		assert(isInitialized());
		return ms_frame_time_current;
	}

//
//  getFrameDuration
//
//  Purpose: To determine the smoothed current frame duration in
//           seconds.
//  Paremeter(s): N/A
//  Precondition(s):
//    <1> isInitialized()
//  Returns: The smoothed duration of the most recent frame in
//           seconds.
//  Side Effect: N/A
//

	static float getFrameDuration ()
	{
		assert(isInitialized());
		return ms_frame_duration_current;
	}

//
//  getTimeToNextFrame
//
//  Purpose: To determine the remaining time in seconds before
//           the next frame should start.
//  Paremeter(s): N/A
//  Precondition(s):
//    <1> isInitialized()
//  Returns: The time in seconds before the next frame should
//           start.  If the next frame should have already
//           started, 0.0 is returned.
//  Side Effect: N/A
//

	static float getTimeToNextFrame ();

//
//  getPauseDuration
//
//  Purpose: To determine the duration of the most recent pause
//           interval in seconds.
//  Paremeter(s): N/A
//  Precondition(s):
//    <1> isInitialized()
//  Returns: The duration in seconds of the most recent interval
//           that the TimeSystem was paused.  If the TimeSystem
//           has not been paused sice it was most recently
//           initialized, 0.0f is returned.
//  Side Effect: N/A
//

	static float getPauseDuration ()
	{
		assert(isInitialized());
		return ms_pause_duration;
	}

//
//  getAiTimeElapsed
//
//  Purpose: To determine the time in seconds used so far by the
//           AI that is currently running.
//  Paremeter(s): N/A
//  Precondition(s):
//    <1> isInitialized()
//    <2> isAiInitialized()
//  Returns: The elapsed time in seconds since markAiStart was
//           called.
//  Side Effect: N/A
//

	static float getAiTimeElapsed ()
	{
		assert(isInitialized());
		assert(isAiInitialized());
		return calculateCurrentTime() - ms_ai_time_start;
	}

//
//  getAiTimeRemaining
//
//  Purpose: To determine the remaining time in seconds allowed
//           for the current AI.
//  Paremeter(s): N/A
//  Precondition(s):
//    <1> isInitialized()
//    <2> isAiInitialized()
//  Returns: The time in seconds left for this AI to perform its
//           calculations.  If the AI has already run for more
//           than its permitted time, a negative value is
//           returned.
//  Side Effect: N/A
//

	static float getAiTimeRemaining ()
	{
		assert(isInitialized());
		assert(isAiInitialized());
		return ms_ai_time_max - calculateCurrentTime();
	}

//
//  getAiTimeOvershot
//
//  Purpose: To determine the how much more time than the
//           permitted maximum the current AI has used.
//  Paremeter(s): N/A
//  Precondition(s):
//    <1> isInitialized()
//    <2> isAiInitialized()
//  Returns: The amount of time in seconds beyond the permitted
//           maximum that the current AI has used.  If the
//           current AI has not yet used up the permitted time,
//           0.0 is returned.
//  Side Effect: N/A
//

	static float getAiTimeOvershot ();

//
//  init
//
//  Purpose: To initialize the TimeSystem to keep track of frame
//           times.
//  Paremeter(s):
//    <1> minimum_frames_per_second: The minimum frame rate
//    <2> desired_frames_per_second: The desired frame rate
//    <3> smoothing_factor: The smoothing fractor for frame
//                          durations
//  Precondition(s):
//    <1> minimum_frames_per_second >  0.0f
//    <2> minimum_frames_per_second <= desired_frames_per_second
//    <3> smoothing_factor >  0.0f
//    <4> smoothing_factor <= 1.0f
//  Returns: N/A
//  Side Effect: The TimeSystem is initialized.  The starting
//               time is set to the current time.  The desired
//               and current frame lengths are set to
//               1.0 / desired_frames_per_second, and the
//               current frame duration is set to be smoothed by
//               smoothing_factor.  The maximum frame duration
//               is set to 1.0 / minimum_frames_per_second.  The
//               AI time tracking system is marked as not
//               initialized.
//

	static void init (float minimum_frames_per_second,
	                  float desired_frames_per_second,
	                  float smoothing_factor);

//
//  markFrameEnd
//
//  Purpose: To alert the TimeSystem that the current frame has
//           just ended.
//  Paremeter(s): N/A
//  Precondition(s):
//    <1> isInitialized()
//  Returns: N/A
//  Side Effect: The time for the current frame is updated to
//               the current time.  The current frame duration
//               is updated based on the length of the most
//               recent frame and the smoothing factor.  The
//               current frame duration will never be less than
//               the minimum frame duration.
//

	static void markFrameEnd ();

//
//  markPauseEnd
//
//  Purpose: To alert the TimeSystem that the program was paused
//           for some reason and that the pause is now over.
//           This removes the elapsed time for this frame so far
//           from frame rate calculations.  Failure to call this
//           function when the program is unpaused will result
//           in very low reported frame rates.
//  Paremeter(s): N/A
//  Precondition(s):
//    <1> isInitialized()
//  Returns: N/A
//  Side Effect: The time for the current frame is updated to
//               the current time.  The current frame duration
//               is updated based on the length of the most
//               recent frame and the smoothing factor.
//

	static void markPauseEnd ();

//
//  markAiStart
//
//  Purpose: To alert the TimeSystem the an AI is about to begin
//           running.  This will allow the time used by the AI
//           to be tracked.
//  Paremeter(s):
//    <1> time_max: The maximum time in seconds for the AI
//  Precondition(s):
//    <1> isInitialized()
//    <2> time_max >= 0.0
//  Returns: N/A
//  Side Effect: The AI time tracking system is initalized.  The
//               start time for the AI is updated to the current
//               time and the maximum available time is set to
//               time_max seconds.
//

	static void markAiStart (float time_max);

private:
//
//  Default Constructor
//  Copy Constructor
//  Destructor
//  Assignment Operator
//
//  These functions have intentionally not been implemented.  It
//    should not be possible to create a TimeSystem, and, as
//    none will ever be created, it also should not be possible
//    to destroy or copy one.
//

	TimeSystem ();
	TimeSystem (const TimeSystem& original);
	~TimeSystem ();
	TimeSystem& operator= (const TimeSystem& original);

//
//  calculateCurrentTime
//
//  Purpose: To determine the current elapsed time in seconds.
//  Paremeter(s): N/A
//  Precondition(s): N/A
//  Returns: The current elapsed time in seconds.
//  Side Effect: N/A
//

	static float calculateCurrentTime ();

//
//  invariant
//
//  Purpose: To determine if the class invariant is true.  Note
//           the the class invariant checks the validity of the
//           static member variables.
//  Paremeter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether the class invariant is true.
//  Side Effect: N/A
//

	static bool invariant ();

private:
//
//  AI_TIME_NOT_INITIALIZED
//
//  A constant indicating that the Ai time tracking system has
//    not yet been initialized.
//

	static const float AI_TIME_NOT_INITIALIZED;

private:
	static bool ms_is_initialized;

	static unsigned int ms_frame_number;
	static float ms_frame_time_current;
	static float ms_frame_duration_max;
	static float ms_frame_duration_desired;
	static float ms_frame_duration_current;
	static float ms_smoothing_factor;
	static float ms_smoothing_factor_inverse;

	static float ms_pause_duration;

	static float ms_ai_time_start;
	static float ms_ai_time_max;

#ifdef _WIN32
	static float ms_time_counter_rate_inverse;
	static LARGE_INTEGER ms_start_time_win;
#elif __APPLE__
	static timeval ms_start_time_apple;
#else	// POSIX
	static double ms_start_time_posix;
#endif
};



#endif
