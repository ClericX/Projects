#pragma once

#include <Windows.h>
#include "KeyboardKey.h"
#include "Point.h"
#include <vector>

using namespace std;

namespace Input
{
	// Keyboard variables.
	extern "C" vector<KeyboardKey *> MonitoredKeys;

	// Cursor variables.
	extern "C" POINT CursorPos;
	extern "C" POINT ClientCursorPos;
	extern "C" POINT OldCursorPos;
	extern "C" bool MouseLocked;

	void Initialize();
	bool RawIsKeyPressed(int KeyCode);
	KeyboardKey *GetKeyByKeyCode(int KeyCode);
	KeyboardKey *GetKeyByChar(char Key);
	bool IsShiftDown();
	void Update();
	Point GetCursorChange();
	bool IsForGame();
	bool IsForGUI();

	namespace Keys
	{
		enum TheKeys
		{
			LMouse = 0x01, // Left mouse button
			RMouse = 0x02, // Right mouse button
			MMouse = 0x04, // Middle mouse button
			Backspace = 0x08, // Backspace
			Tab = 0x09, // Tab
			Enter = 0x0D, // Enter
			Shift = 0x10, // Shift
			Control = 0x11, // Control
			Alt = 0x12, // Alt
			CapsLock = 0x14, // Caps Lock
			Esc = 0x1B, // Esc
			Spacebar = 0x20, // Spacebar
			PageUp = 0x21, // Page Up
			PageDown = 0x22, // Page Down
			End = 0x23, // End
			Home = 0x24, // Home
			Left = 0x25, // Left arrow
			Up = 0x26, // Up arrow
			Right = 0x27, // Right arrow
			Down = 0x28, // Down arrow
			PrintScreen = 0x2C, // Print Screen
			Insert = 0x2D, // Insert
			Delete = 0x2E, // Delete
			_0 = 0x30, // 0
			_1 = 0x31, // 1
			_2 = 0x32, // 2
			_3 = 0x33, // 3
			_4 = 0x34, // 4
			_5 = 0x35, // 5
			_6 = 0x36, // 6
			_7 = 0x37, // 7
			_8 = 0x38, // 8
			_9 = 0x39, // 9
			A = 0x41, // A
			B = 0x42, // B
			C = 0x43, // C
			D = 0x44, // D
			E = 0x45, // E
			F = 0x46, // F
			G = 0x47, // G
			H = 0x48, // H
			I = 0x49, // I
			J = 0x4A, // J
			K = 0x4B, // K
			L = 0x4C, // L
			M = 0x4D, // M
			N = 0x4E, // N
			O = 0x4F, // O
			P = 0x50, // P
			Q = 0x51, // Q
			R = 0x52, // R
			S = 0x53, // S
			T = 0x54, // T
			U = 0x55, // U
			V = 0x56, // V
			W = 0x57, // W
			X = 0x58, // X
			Y = 0x59, // Y
			Z = 0x5A, // Z
			F1 = 0x70, // F1
			F2 = 0x71, // F2
			F3 = 0x72, // F3
			F4 = 0x73, // F4
			F5 = 0x74, // F5
			F6 = 0x75, // F6
			F7 = 0x76, // F7
			F8 = 0x77, // F8
			F9 = 0x78, // F9
			F10 = 0x79, // F10
			F11 = 0x7A, // F11
			F12 = 0x7B // F12
		};
	};
};