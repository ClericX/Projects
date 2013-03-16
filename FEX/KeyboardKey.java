/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package frostfe;

public class KeyboardKey
{
	public int KeyCode;
	public char Key;
	public char CapitalKey;
	public double LastPressed;
	public double LastReleased;
	public double LastRepeat;
	public boolean CurrentlyDown;
	public boolean PreviousDownState;
	
	public KeyboardKey()
	{
		Init();
		KeyCode = 0;
		Key = 0;
		CapitalKey = 0;
	}
	
	public KeyboardKey(int nKeyCode, char cKey, char cCapitalKey)
	{
		Init();
		KeyCode = nKeyCode;
		Key = cKey;
		CapitalKey = cCapitalKey;
	}

	public void Init()
	{
		LastPressed = 0;
		LastReleased = 0;
		LastRepeat = 0;
		CurrentlyDown = false;
		PreviousDownState = false;
	}
	
	public boolean HasChar()
	{
		return Key != 0;
	}
	
	public boolean HasCapitalChar()
	{
		return CapitalKey != 0;
	}
	
	public boolean IsBeingHeldDown(int HoldingDownThreshold)
	{
		return (CurrentlyDown && getTimeHeldDown() > HoldingDownThreshold);
	}
	
	public boolean IsBeingHeldDown()
	{
		return IsBeingHeldDown(1000);
	}
	
	public double getTimeHeldDown()
	{
		if (CurrentlyDown)
		{
			return Application.RunTime - LastPressed;
		}
		else
		{
			return 0;
		}
	}
	
	public boolean CheckForRepeat()
	{
		// Assuming the repeat rate we want is every 25 ms.
		// Also, delay before counting the repeats is 500 ms.
		if (this.CurrentlyDown && getTimeHeldDown() > 500)
		{
			return (Application.RunTime - LastRepeat) > 25;
		}
		else
		{
			return false;
		}
	}
	
	public void SetRepeatComplete()
	{
		LastRepeat = Application.RunTime;
	}
	
	public boolean WasPressed()
	{
		return LastPressed == Application.RunTime;
	}
	
	public boolean WasReleased()
	{
		return LastReleased == Application.RunTime;
	}
	
	public void Update()
	{
		PreviousDownState = CurrentlyDown;
		CurrentlyDown = Input.RawIsKeyPressed(KeyCode);
		if (!PreviousDownState && CurrentlyDown)
		{
			LastPressed = Application.RunTime;
		}
		if (PreviousDownState && !CurrentlyDown)
		{
			LastReleased = Application.RunTime;
		}
	}
}