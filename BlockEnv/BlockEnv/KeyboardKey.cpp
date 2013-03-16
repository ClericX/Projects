#include "KeyboardKey.h"
#include "Input.h"
#include "Application.h"
#include <math.h>

KeyboardKey::KeyboardKey(void)
{
	Init();
	KeyCode = 0;
	Key = 0;
	CapitalKey = 0;
}

KeyboardKey::KeyboardKey(int nKeyCode, char cKey, char cCapitalKey)
{
	Init();
	KeyCode = nKeyCode;
	Key = cKey;
	CapitalKey = cCapitalKey;
}

KeyboardKey::~KeyboardKey(void)
{
}

void KeyboardKey::Init()
{
	LastPressed = 0;
	LastReleased = 0;
	LastRepeat = 0;
	CurrentlyDown = false;
	PreviousDownState = false;
}

bool KeyboardKey::HasChar()
{
	return Key != 0;
}

bool KeyboardKey::HasCapitalChar()
{
	return CapitalKey != 0;
}

bool KeyboardKey::IsBeingHeldDown(unsigned int HoldingDownThreshold)
{
	return (CurrentlyDown && getTimeHeldDown() > HoldingDownThreshold);
}

bool KeyboardKey::IsBeingHeldDown()
{
	return IsBeingHeldDown(1000);
}

double KeyboardKey::getTimeHeldDown()
{
	if (CurrentlyDown)
	{
		return Application::RunTime - LastPressed;
	}
	else
	{
		return 0;
	}
}

bool KeyboardKey::CheckForRepeat()
{
	// Assuming the repeat rate we want is every 25 ms.
	// Also, delay before counting the repeats is 500 ms.
	if (this->CurrentlyDown && getTimeHeldDown() > 500)
	{
		return (Application::RunTime - LastRepeat) > 25;
	}
	else
	{
		return false;
	}
}

void KeyboardKey::SetRepeatComplete()
{
	LastRepeat = Application::RunTime;
}

bool KeyboardKey::WasPressed()
{
	return LastPressed == Application::RunTime;
}

bool KeyboardKey::WasReleased()
{
	return LastReleased == Application::RunTime;
}

void KeyboardKey::Update()
{
	PreviousDownState = CurrentlyDown;
	CurrentlyDown = Input::RawIsKeyPressed(KeyCode);
	if (!PreviousDownState && CurrentlyDown)
	{
		LastPressed = Application::RunTime;
	}
	if (PreviousDownState && !CurrentlyDown)
	{
		LastReleased = Application::RunTime;
	}
}