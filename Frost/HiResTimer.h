#pragma once

#include "DLL.h"

#include <Windows.h>

namespace HiResTimer
{
	extern "C" FROSTAPI LARGE_INTEGER Start;
	extern "C" FROSTAPI LARGE_INTEGER Stop;
	extern "C" FROSTAPI LARGE_INTEGER Frequency;

	bool FROSTAPI Initialize();
	void FROSTAPI Update();
	LONGLONG FROSTAPI GetElapsedMicroseconds();
	double FROSTAPI GetElapsedSeconds();
	double FROSTAPI GetElapsedMilliseconds();
	float FROSTAPI GetFPS();
	void FROSTAPI LockFPS(float TargetFPS);
};