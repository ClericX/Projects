/****************************************************************************
 HiResTimer.h

 Cross-platform implementation of the HiResTimer originally written
 by Dave Astle for the book OpenGL Game Programming.

 Author		: Luke Benstead

 Wrapper for the high-resolution timer. Can't be used (on Windows) if the
 hi-res timer doesn't exist.

*****************************************************************************/


#ifndef __TIMER_H_INCLUDED__
#define __TIMER_H_INCLUDED__

#ifdef WIN32
#include <windows.h>
#endif

#ifdef unix
	#include <sys/time.h>

	/*
		This is the definition of LARGE_INTEGER
		for unix platforms only, Windows already has
		this defined.
	*/
	typedef union _LARGE_INTEGER
	{
	  struct
	  {
		unsigned long LowPart;
		long HighPart;
	  };
	  struct
	  {
		unsigned long LowPart;
		long HighPart;
	  } u;
	  long long QuadPart;
	} LARGE_INTEGER;

	//This is the number of ticks per second for
	//unix platforms
	const long TicksPerSecond = 10000000;
#endif

class HiResTimer
{
public:

/*****************************************************************************
 Init()

 If the hi-res timer is present, the tick rate is stored and the function
 returns true. Otherwise, the function returns false, and the timer should
 not be used.
*****************************************************************************/
	bool Init()
	{
		if (!QueryPerformanceFrequency(&m_ticksPerSecond))
		{
			// system doesn't support hi-res timer
			return false;
		}
		else
		{
			QueryPerformanceCounter(&m_startTime);
			return true;
		}
	} // end Init()

	float GetElapsedSeconds(unsigned long elapsedFrames = 1)
	{
		static LARGE_INTEGER s_lastTime = m_startTime;
		LARGE_INTEGER currentTime;

		QueryPerformanceCounter(&currentTime);

		float seconds =  ((float)currentTime.QuadPart - (float)s_lastTime.QuadPart) / (float)m_ticksPerSecond.QuadPart;

		// reset the timer
		s_lastTime = currentTime;

		return seconds;
	} // end GetElapsedSeconds()

/***************************************************************************
 GetFPS()

 Returns the average frames per second over elapsedFrames, which defaults to
 one. If this is not called every frame, the client should track the number
 of frames itself, and reset the value after this is called.
***************************************************************************/
	float GetFPS(unsigned long elapsedFrames = 1)
	{
		static LARGE_INTEGER s_lastTime = m_startTime;
		LARGE_INTEGER currentTime;

		QueryPerformanceCounter(&currentTime);

		float fps = (float)elapsedFrames * (float)m_ticksPerSecond.QuadPart / ((float)currentTime.QuadPart - (float)s_lastTime.QuadPart);

		// reset the timer
		s_lastTime = currentTime;

		return fps;
	} // end GetFPS

/***************************************************************************
 LockFPS()

 Used to lock the frame rate to a set amount. This will block until enough
 time has passed to ensure that the fps won't go over the requested amount.
 Note that this can only keep the fps from going above the specified level;
 it can still drop below it. It is assumed that if used, this function will
 be called every frame. The value returned is the instantaneous fps, which
 will be <= targetFPS.
***************************************************************************/
	float LockFPS(unsigned char targetFPS)
	{
		if (targetFPS == 0)
		targetFPS = 1;

		static LARGE_INTEGER s_lastTime = m_startTime;
		LARGE_INTEGER currentTime;
		float   fps;

		// delay to maintain a constant frame rate
		do {
		QueryPerformanceCounter(&currentTime);
		fps = (float)m_ticksPerSecond.QuadPart/((float)(currentTime.QuadPart - s_lastTime.QuadPart));
		} while (fps > (float)targetFPS);

		// reset the timer
		s_lastTime = m_startTime;

		return fps;
	} // end LockFPS()

private:
#ifdef unix
	/*
		These next 2 functions are reimplementations of the Windows
		functions of the same name for unix. If we are compiling
		on Windows these 2 methods will not be compiled and the
		Windows built-in ones will be used instead.
	*/
	bool QueryPerformanceFrequency(LARGE_INTEGER* frequency)
	{
		frequency->QuadPart = TicksPerSecond;
		return true;
	}

	bool QueryPerformanceCounter(LARGE_INTEGER* currentTime)
	{
		unsigned long long Ticks1601To1970 = (369 * 365 + 89) * 86400 * (unsigned long long) TicksPerSecond;
		struct timeval now;
		gettimeofday( &now, 0 );
		currentTime->QuadPart = now.tv_sec * TicksPerSecond + Ticks1601To1970;
		currentTime->QuadPart += now.tv_usec * 10;

		return true;
	}
#endif

	LARGE_INTEGER   m_startTime;
	LARGE_INTEGER   m_ticksPerSecond;
};

#endif