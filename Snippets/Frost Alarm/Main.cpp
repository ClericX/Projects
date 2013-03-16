#include <windows.h>
#include <stdio.h>

void main()
{
	SYSTEMTIME st, lt;
	bool HitTime = false;
	int TargetHour = 6, TargetMinute = 30;
	
	while (!HitTime)
	{
		//GetSystemTime(&st);
		GetLocalTime(&lt);
	
		//printf("The system time is: %02d:%02d\n", st.wHour, st.wMinute);
		printf(" The local time is: %02d:%02d\n", lt.wHour, lt.wMinute);
		int TimeCalcMin = TargetMinute - lt.wMinute;
		int TimeCalcHr = TargetHour - lt.wHour;
		if (TimeCalcMin < 0)
		{
			TimeCalcMin = (60 - TimeCalcMin);
			TimeCalcHr--;
		}
		printf("Time left until alarm goes off: %d:%d", TimeCalcHr, TimeCalcMin);
		Sleep(1000);
		system("cls");

		if (lt.wHour == TargetHour && lt.wMinute == TargetMinute)
		{
			HitTime = true;
		}
	}
	ShellExecuteA(0, 0, "C:/Program Files (x86)/Windows Media Player/wmplayer.exe", "/Play -Embedding \"C:/Users/Yoko/Music/2NE1 - Fire.mp3\"", 0, SW_SHOW);
	system("pause");
}