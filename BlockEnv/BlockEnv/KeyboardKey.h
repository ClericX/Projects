#pragma once

class KeyboardKey
{
public:
	KeyboardKey(void);
	KeyboardKey(int nKeyCode, char cKey, char cCapitalKey);
	~KeyboardKey(void);

	int KeyCode;
	char Key;
	char CapitalKey;
	double LastPressed;
	double LastReleased;
	double LastRepeat;
	bool CurrentlyDown;
	bool PreviousDownState;

	void Init();
	bool HasChar();
	bool HasCapitalChar();
	bool IsBeingHeldDown(unsigned int HoldingDownThreshold);
	bool IsBeingHeldDown();
	double getTimeHeldDown();
	bool CheckForRepeat();
	void SetRepeatComplete();
	bool WasPressed();
	bool WasReleased();
	void Update();
};