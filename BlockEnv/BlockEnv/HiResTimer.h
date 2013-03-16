#pragma once

#include <Windows.h>

namespace HiResTimer
{
	extern "C" LARGE_INTEGER Start;
	extern "C" LARGE_INTEGER Stop;
	extern "C" LARGE_INTEGER Frequency;

	bool Initialize();
	void Update();
	LONGLONG GetElapsedMicroseconds();
	double GetElapsedSeconds();
	double GetElapsedMilliseconds();
	float GetFPS();
	void LockFPS(float TargetFPS);
};