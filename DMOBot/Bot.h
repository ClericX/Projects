#pragma once

#include <Windows.h>
#include "../../Moonlight Frost/Frost/File.h"
#include "../../Moonlight Frost/Frost/String.h"
#include "Color.h"
#include "Capture.h"

namespace Bot
{
	extern "C" bool bAutoChat;
	extern "C" bool bAutoPotion;
	extern "C" bool bFMStealer;
	extern "C" int AutoChatDelay;
	extern "C" POINT HPpotPos;
	extern "C" POINT MPpotPos;
	extern "C" Color HPpotClr;
	extern "C" Color MPpotClr;
	extern "C" POINT fmPos;
	extern "C" Color fmClr;
	extern "C" String autoChatStr;
	extern "C" unsigned int curAutoChatOffset;
	extern "C" Capture screenCap;
	extern "C" unsigned int LastScreenCapTime;

	void UpdateScreenCapture();
	void PostString(char *str);
	void PostKey(char key);
	void AutoChatProcedure();
	void AutoPotionProcedure();
	void FMStealerProcedure();
	void TriggerAutoChat(bool Status);
	void TriggerAutoPotion(bool Status);
	void TriggerFMStealer(bool Status);

	bool IsKeyPressed(int vkey);
};