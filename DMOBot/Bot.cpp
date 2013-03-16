#include "Bot.h"

namespace Bot
{
	bool bAutoChat;
	bool bAutoPotion;
	bool bFMStealer;
	int AutoChatDelay;
	POINT HPpotPos;
	POINT MPpotPos;
	Color HPpotClr;
	Color MPpotClr;
	POINT fmPos;
	Color fmClr;
	String autoChatStr;
	unsigned int curAutoChatOffset = 0;
	Capture screenCap;
	unsigned int LastScreenCapTime;

	void UpdateScreenCapture()
	{
		if (GetTickCount() - LastScreenCapTime > 500)
		{
			screenCap.Release();
			screenCap.Acquire();
			LastScreenCapTime = GetTickCount();
		}
	}

	void PostString(char *str)
	{
		for (unsigned int i = 0; i < strlen(str); i++)
		{
			PostMessageA(FindWindow("MapleStoryClass", 0), WM_CHAR, str[i], 0);
		}
	}

	void PostKey(char key)
	{
		PostMessageA(FindWindowA("MapleStoryClass", 0), WM_KEYDOWN, key, MapVirtualKeyA(key, 0) << 16);
		Sleep(50);
		PostMessageA(FindWindowA("MapleStoryClass", 0), WM_KEYUP, key, MapVirtualKeyA(key, 0) << 16);
	}

	void AutoChatProcedure()
	{
		while (bAutoChat)
		{
			String curMsg;
			while (true)
			{
				if (curAutoChatOffset >= autoChatStr.length())
				{
					curAutoChatOffset = 0;
				}
				if (autoChatStr[curAutoChatOffset] == '\n')
				{
					curAutoChatOffset++;
					break;
				}
				else
				{
					curMsg += autoChatStr[curAutoChatOffset];
					curAutoChatOffset++;
				}
			}
			PostKey(VK_RETURN);
			PostString(curMsg);
			PostKey(VK_RETURN);
			PostKey(VK_RETURN);

			Sleep(1000);
		}
	}

	void AutoPotionProcedure()
	{
		while (bAutoPotion)
		{
			Bot::UpdateScreenCapture();

			if (screenCap.Get(HPpotPos.x, HPpotPos.y) != HPpotClr)
			{
				PostKey(VK_PRIOR);
			}
			if (screenCap.Get(MPpotPos.x, MPpotPos.y) != MPpotClr)
			{
				PostKey(VK_NEXT);
			}

			Sleep(50);
		}
	}

	void FMStealerProcedure()
	{
		while (bFMStealer)
		{
			Bot::UpdateScreenCapture();

			if (screenCap.Get(fmPos.x, fmPos.y) != fmClr)
			{
				// Open shop
				keybd_event(VK_RETURN, MapVirtualKeyA(VK_RETURN, 0), 0, 0);
				Sleep(1000);
				// Click use tab
				SetCursorPos(913, 66);
				mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				Sleep(1000);
				// Use slot 1
				SetCursorPos(884, 95);
				mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				Sleep(1000);
				// First slot of shop
				SetCursorPos(353, 336);
				mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				Sleep(1000);
				// Type 1
				PostString("1");
				Sleep(1000);
				// Press tab
				PostKey(VK_TAB);
				Sleep(1000);
				// Type 99999999
				PostString("9999999");
				Sleep(1000);
				// Press enter
				keybd_event(VK_RETURN, MapVirtualKeyA(VK_RETURN, 0), 0, 0);
				Sleep(1000);
				// Click open shop
				SetCursorPos(439, 232);
				mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

				TriggerFMStealer(false);
			}

			Sleep(50);
		}
	}

	void Bot::TriggerAutoChat(bool Status)
	{
		//
	}

	void Bot::TriggerAutoPotion(bool Status)
	{
		bAutoPotion = Status;

		if (bAutoPotion)
		{
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&AutoPotionProcedure, 0, 0, 0);
		}
	}

	void Bot::TriggerFMStealer(bool Status)
	{
		bFMStealer = Status;

		if (bFMStealer)
		{
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&FMStealerProcedure, 0, 0, 0);
		}
	}

	bool IsKeyPressed(int vkey)
	{
		return (bool)(GetAsyncKeyState(vkey) & 0x8000);
	}
};