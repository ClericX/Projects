#pragma once

#include <Windows.h>

namespace Flags
{
	// Functions related to flag handling.
	void SetFlag(DWORD& Target, DWORD Flag);
	void UnsetFlag(DWORD& Target, DWORD Flag);
	bool CheckFlag(DWORD& Target, DWORD Flag);
	void InvertFlag(DWORD& Target);
};

static const DWORD WINDOW_TYPE_DESKTOP = (1 << 31);
static const DWORD WINDOW_TYPE_WINDOW = (1 << 30);
static const DWORD WINDOW_TYPE_TEXTBOX = (1 << 29);
static const DWORD WINDOW_TYPE_BUTTON = (1 << 28);

static const int PUSH_STATE_NORMAL = 0;
static const int PUSH_STATE_HOVER = 1;
static const int PUSH_STATE_PUSHED = 2;

static const int DRAWMODE_2D = 1;
static const int DRAWMODE_3D = 2;