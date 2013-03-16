#include "HiResTimer.h"

namespace HiResTimer
{
	LARGE_INTEGER Start;
	LARGE_INTEGER Stop;
	LARGE_INTEGER Frequency;

	bool Initialize()
	{
		Start.QuadPart = Stop.QuadPart = Frequency.QuadPart = 0;
		if (!QueryPerformanceFrequency(&Frequency))
		{
			return false;
		}
		else
		{
			QueryPerformanceCounter(&Start);
			Stop = Start;
			return true;
		}
	}

	void Update()
	{
		Start = Stop;
		QueryPerformanceCounter(&Stop);
	}

	LONGLONG GetElapsedMicroseconds()
	{
		return (Stop.QuadPart - Start.QuadPart);
	}

	double GetElapsedSeconds()
	{
		return ((double)GetElapsedMicroseconds() / (double)Frequency.QuadPart);
	}

	double GetElapsedMilliseconds()
	{
		return (GetElapsedSeconds() * 1000);
	}

	float GetFPS()
	{
		/*
		static float ReturnFPS = 0;
		static float FPSCounter = 0;
		static float ElapsedSecTime = 0;
		ElapsedSecTime += GetElapsedMilliseconds();
		FPSCounter++;
		if (ElapsedSecTime >= 1000)
		{
			ElapsedSecTime = 0;
			ReturnFPS = FPSCounter;
			FPSCounter = 0;
		}
		return ReturnFPS;
		*/

		static const int NUM_FPS_SAMPLES = 100;
		static float fpsSamples[NUM_FPS_SAMPLES] = { 0 };
		static int currentSample = 0;

		fpsSamples[currentSample] = 1.0f / (float)GetElapsedSeconds();

		float fps = 0;

		for (int i = 0; i < NUM_FPS_SAMPLES; i++)
			fps += fpsSamples[i];

		fps /= NUM_FPS_SAMPLES;

		currentSample++;

		if (currentSample > NUM_FPS_SAMPLES)
			currentSample = 0;

		return fps;
	}

	void LockFPS(float TargetFPS)
	{
		LARGE_INTEGER OldLastTime = Stop;
		while (GetFPS() > TargetFPS)
		{
			QueryPerformanceCounter(&Stop);
		}
		Stop = OldLastTime;
	}
};

/*

-Unix implementations of the resolution timer Windows functions-

#include <sys/time.h>

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

bool QueryPerformanceFrequency(LARGE_INTEGER* frequency)
{
	frequency->QuadPart = 10000000;
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

*/