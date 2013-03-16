#pragma once

#include "Includes.h"

class DirectInput
{
public:
	DirectInput(void);
	~DirectInput(void);

	static const int NUMMOUSEBUTTONS = 3;

	LPDIRECTINPUT8 InputObject;
	LPDIRECTINPUTDEVICE8 KeyboardDevice;
	LPDIRECTINPUTDEVICE8 MouseDevice;
	DIMOUSESTATE dims;
	POINT AbsPos;
	POINT RelPos;
	char MouseButtonState[NUMMOUSEBUTTONS];

	void Initialize();
	void UpdateInputState();
	void ProcessInput();
	char KeyboardState[256];
	bool KeyPressed(int Key);
	void UpdateKey(int Key, bool ConstantFunctionality, void KeyPressedFunction(), void KeyHeldDownFunction() = NULL);
	int KeyHeldBuffer[256];
	bool KeyHeldUsed[256];
	bool KeyHeldDown[256];
	int IsMouseButtonDown(int index);
};