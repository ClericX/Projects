#include "Input.h"
#include "GUIWindow.h"
#include "Application.h"

namespace Input
{
	vector<KeyboardKey *> MonitoredKeys;

	POINT CursorPos;
	POINT ClientCursorPos;
	POINT OldCursorPos;
	bool MouseLocked = false;

	void Initialize()
	{
		MonitoredKeys.push_back(new KeyboardKey(0x01, 0, 0)); // Left mouse button
		MonitoredKeys.push_back(new KeyboardKey(0x02, 0, 0)); // Right mouse button
		MonitoredKeys.push_back(new KeyboardKey(0x04, 0, 0)); // Middle mouse button
		MonitoredKeys.push_back(new KeyboardKey(0x08, 0, 0)); // Backspace
		MonitoredKeys.push_back(new KeyboardKey(0x09, 0, 0)); // Tab
		MonitoredKeys.push_back(new KeyboardKey(0x0D, 0, 0)); // Enter
		MonitoredKeys.push_back(new KeyboardKey(0x10, 0, 0)); // Shift
		MonitoredKeys.push_back(new KeyboardKey(0x11, 0, 0)); // Control
		MonitoredKeys.push_back(new KeyboardKey(0x12, 0, 0)); // Alt
		MonitoredKeys.push_back(new KeyboardKey(0x14, 0, 0)); // Caps Lock
		MonitoredKeys.push_back(new KeyboardKey(0x1B, 0, 0)); // Esc
		MonitoredKeys.push_back(new KeyboardKey(0x20, ' ', 0)); // Spacebar
		MonitoredKeys.push_back(new KeyboardKey(0x21, 0, 0)); // Page Up
		MonitoredKeys.push_back(new KeyboardKey(0x22, 0, 0)); // Page Down
		MonitoredKeys.push_back(new KeyboardKey(0x23, 0, 0)); // End
		MonitoredKeys.push_back(new KeyboardKey(0x24, 0, 0)); // Home
		MonitoredKeys.push_back(new KeyboardKey(0x25, 0, 0)); // Left arrow
		MonitoredKeys.push_back(new KeyboardKey(0x26, 0, 0)); // Up arrow
		MonitoredKeys.push_back(new KeyboardKey(0x27, 0, 0)); // Right arrow
		MonitoredKeys.push_back(new KeyboardKey(0x28, 0, 0)); // Down arrow
		MonitoredKeys.push_back(new KeyboardKey(0x2C, 0, 0)); // Print Screen
		MonitoredKeys.push_back(new KeyboardKey(0x2D, 0, 0)); // Insert
		MonitoredKeys.push_back(new KeyboardKey(0x2E, 0, 0)); // Delete
		MonitoredKeys.push_back(new KeyboardKey(0x30, '0', ')')); // 0
		MonitoredKeys.push_back(new KeyboardKey(0x31, '1', '!')); // 1
		MonitoredKeys.push_back(new KeyboardKey(0x32, '2', '@')); // 2
		MonitoredKeys.push_back(new KeyboardKey(0x33, '3', '#')); // 3
		MonitoredKeys.push_back(new KeyboardKey(0x34, '4', '$')); // 4
		MonitoredKeys.push_back(new KeyboardKey(0x35, '5', '%')); // 5
		MonitoredKeys.push_back(new KeyboardKey(0x36, '6', '^')); // 6
		MonitoredKeys.push_back(new KeyboardKey(0x37, '7', '&')); // 7
		MonitoredKeys.push_back(new KeyboardKey(0x38, '8', '*')); // 8
		MonitoredKeys.push_back(new KeyboardKey(0x39, '9', '(')); // 9
		MonitoredKeys.push_back(new KeyboardKey(0x41, 'a', 'A')); // A
		MonitoredKeys.push_back(new KeyboardKey(0x42, 'b', 'B')); // B
		MonitoredKeys.push_back(new KeyboardKey(0x43, 'c', 'C')); // C
		MonitoredKeys.push_back(new KeyboardKey(0x44, 'd', 'D')); // D
		MonitoredKeys.push_back(new KeyboardKey(0x45, 'e', 'E')); // E
		MonitoredKeys.push_back(new KeyboardKey(0x46, 'f', 'F')); // F
		MonitoredKeys.push_back(new KeyboardKey(0x47, 'g', 'G')); // G
		MonitoredKeys.push_back(new KeyboardKey(0x48, 'h', 'H')); // H
		MonitoredKeys.push_back(new KeyboardKey(0x49, 'i', 'I')); // I
		MonitoredKeys.push_back(new KeyboardKey(0x4A, 'j', 'J')); // J
		MonitoredKeys.push_back(new KeyboardKey(0x4B, 'k', 'K')); // K
		MonitoredKeys.push_back(new KeyboardKey(0x4C, 'l', 'L')); // L
		MonitoredKeys.push_back(new KeyboardKey(0x4D, 'm', 'M')); // M
		MonitoredKeys.push_back(new KeyboardKey(0x4E, 'n', 'N')); // N
		MonitoredKeys.push_back(new KeyboardKey(0x4F, 'o', 'O')); // O
		MonitoredKeys.push_back(new KeyboardKey(0x50, 'p', 'P')); // P
		MonitoredKeys.push_back(new KeyboardKey(0x51, 'q', 'Q')); // Q
		MonitoredKeys.push_back(new KeyboardKey(0x52, 'r', 'R')); // R
		MonitoredKeys.push_back(new KeyboardKey(0x53, 's', 'S')); // S
		MonitoredKeys.push_back(new KeyboardKey(0x54, 't', 'T')); // T
		MonitoredKeys.push_back(new KeyboardKey(0x55, 'u', 'U')); // U
		MonitoredKeys.push_back(new KeyboardKey(0x56, 'v', 'V')); // V
		MonitoredKeys.push_back(new KeyboardKey(0x57, 'w', 'W')); // W
		MonitoredKeys.push_back(new KeyboardKey(0x58, 'x', 'X')); // X
		MonitoredKeys.push_back(new KeyboardKey(0x59, 'y', 'Y')); // Y
		MonitoredKeys.push_back(new KeyboardKey(0x5A, 'z', 'Z')); // Z
		MonitoredKeys.push_back(new KeyboardKey(0x70, 0, 0)); // F1
		MonitoredKeys.push_back(new KeyboardKey(0x71, 0, 0)); // F2
		MonitoredKeys.push_back(new KeyboardKey(0x72, 0, 0)); // F3
		MonitoredKeys.push_back(new KeyboardKey(0x73, 0, 0)); // F4
		MonitoredKeys.push_back(new KeyboardKey(0x74, 0, 0)); // F5
		MonitoredKeys.push_back(new KeyboardKey(0x75, 0, 0)); // F6
		MonitoredKeys.push_back(new KeyboardKey(0x76, 0, 0)); // F7
		MonitoredKeys.push_back(new KeyboardKey(0x77, 0, 0)); // F8
		MonitoredKeys.push_back(new KeyboardKey(0x78, 0, 0)); // F9
		MonitoredKeys.push_back(new KeyboardKey(0x79, 0, 0)); // F10
		MonitoredKeys.push_back(new KeyboardKey(0x7A, 0, 0)); // F11
		MonitoredKeys.push_back(new KeyboardKey(0x7B, 0, 0)); // F12
		MonitoredKeys.push_back(new KeyboardKey(0xBA, ';', ':')); // ;:
		MonitoredKeys.push_back(new KeyboardKey(0xBB, '=', '+')); // +=
		MonitoredKeys.push_back(new KeyboardKey(0xBC, ',', '<')); // ,<
		MonitoredKeys.push_back(new KeyboardKey(0xBD, '-', '_')); // -_
		MonitoredKeys.push_back(new KeyboardKey(0xBE, '.', '>')); // .>
		MonitoredKeys.push_back(new KeyboardKey(0xBF, '/', '?')); // /?
		MonitoredKeys.push_back(new KeyboardKey(0xC0, '`', '~')); // `~
		MonitoredKeys.push_back(new KeyboardKey(0xDB, '[', '{')); // [{
		MonitoredKeys.push_back(new KeyboardKey(0xDC, '\\', '|')); // \|
		MonitoredKeys.push_back(new KeyboardKey(0xDD, ']', '}')); // ]}
		MonitoredKeys.push_back(new KeyboardKey(0xDE, '\'', '\"')); // '"

		Update();
		// Set the initial old mouse position to the current one so there is no initial massive shoot in difference.
		OldCursorPos = CursorPos;
	}

	bool RawIsKeyPressed(int KeyCode)
	{
		return ((HIWORD(GetAsyncKeyState(KeyCode)) & 0x80) == 0x80);
	}

	KeyboardKey *GetKeyByKeyCode(int KeyCode)
	{
		for (unsigned int i = 0; i < MonitoredKeys.size(); i++)
		{
			if (MonitoredKeys[i]->KeyCode == KeyCode)
			{
				return MonitoredKeys[i];
			}
		}
		return (KeyboardKey *)0;
	}

	KeyboardKey *GetKeyByChar(char Key)
	{
		for (unsigned int i = 0; i < MonitoredKeys.size(); i++)
		{
			if (MonitoredKeys[i]->Key == Key || MonitoredKeys[i]->CapitalKey == Key)
			{
				return MonitoredKeys[i];
			}
		}
		return (KeyboardKey *)0;
	}

	bool IsShiftDown()
	{
		static KeyboardKey *pShift = 0;
		if (pShift == 0)
		{
			pShift = GetKeyByKeyCode(0x10);
		}
		if (pShift != 0)
		{
			return pShift->CurrentlyDown;
		}
		return false;
	}

	void Update()
	{
		// Update some cursor position variables.
		OldCursorPos = CursorPos;
		GetCursorPos(&CursorPos);
		//memcpy(&Application::ClientCursorPos, &Application::CursorPos, sizeof(POINT));
		ClientCursorPos = CursorPos;
		ScreenToClient(Application::Window, &ClientCursorPos);

		// Update the state of all keyboard keys and the mouse left/right buttons.
		for (unsigned int i = 0; i < MonitoredKeys.size(); i++)
		{
			MonitoredKeys[i]->Update();

			if ((MonitoredKeys[i]->WasPressed() || MonitoredKeys[i]->CheckForRepeat()) && IsForGUI())
			{
				GUI::RecieveKeyboardKey(MonitoredKeys[i]);
			}
		}
	}

	Point GetCursorChange()
	{
		return Point(CursorPos.x - OldCursorPos.x, CursorPos.y - OldCursorPos.y);
	}

	bool IsForGame()
	{
		return !GUI::GetDesktop()->IsVisible();
	}

	bool IsForGUI()
	{
		return GUI::GetDesktop()->IsVisible();
	}
};