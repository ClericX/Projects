#include <Windows.h>
#include <string>
#include <sstream>
#include <iostream>

#include "UI.h"

HFONT appFont = 0;

HANDLE HookListenThread = 0;
HANDLE PeriodicStatSavingThread = 0;
DWORD FileSaveDelay = 1000 * 60; // Save every minute.

HHOOK mouseHook = 0;
HHOOK keyboardHook = 0;

std::string getFilenameForDay(SYSTEMTIME t)
{
	std::stringstream targetFilename;
	targetFilename << "kbdamc-" << t.wMonth << "-" << t.wDay << "-" << t.wYear << ".dat";
	return targetFilename.str();
}

SYSTEMTIME getNow()
{
	SYSTEMTIME ret;
	GetLocalTime(&ret);
	return ret;
}

class StatisticsSet
{
public:
	unsigned int keypressCounts[0xFF];
	unsigned int clickCounts[2];
	SYSTEMTIME loadTime;
	SYSTEMTIME representedTime;
	SYSTEMTIME lastSaved;

	unsigned int getTotalKeypresses()
	{
		unsigned int total = 0;

		for (unsigned int i = 0; i < 0xFF; i++)
		{
			total += this->keypressCounts[i];
		}

		return total;
	}

	unsigned int getTotalMouseClicks()
	{
		return this->clickCounts[0] + this->clickCounts[1];
	}

	bool save()
	{
		FILE *f = fopen(getFilenameForDay(this->representedTime).c_str(), "w");

		if (f != 0)
		{
			lastSaved = getNow();
			fwrite(this->clickCounts, sizeof(unsigned int), 2, f);
			fwrite(this->keypressCounts, sizeof(unsigned int), 0xFF, f);
			fclose(f);
			return true;
		}

		return false;
	}

	bool load()
	{
		FILE *f = fopen(getFilenameForDay(this->representedTime).c_str(), "r");

		if (f != 0)
		{
			loadTime = getNow();
			fread(this->clickCounts, sizeof(unsigned int), 2, f);
			fread(this->keypressCounts, sizeof(unsigned int), 0xFF, f);
			fclose(f);
			return true;
		}

		return false;
	}

	bool representsDate(SYSTEMTIME d)
	{
		return this->representedTime.wMonth == d.wMonth && this->representedTime.wDay == d.wDay && this->representedTime.wYear == d.wYear;
	}
};

StatisticsSet *activeStatSet;

void startInputLogging();

class StatTrackerForm : public Form
{
public:
	ListView *InputHookLog;
	ListView *ActivityLog;
	Label *LastSaveLabel;
	Checkbox *ToggleStatSaving;
	Checkbox *ToggleMinimizeToTray;

	StatTrackerForm()
	{
		setText("ClericX's Keystroke and Mouse Click Tracker v2");
		setLocation(100, 100);
		setClientAreaSize(830, 220);

		appFont = CreateFont(12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");

		InputHookLog = new ListView(this);
		InputHookLog->setLocation(10, 10);
		InputHookLog->setSize(350, 200);
		InputHookLog->show();

		InputHookLog->insertColumn(75, 0, "Action");
		InputHookLog->insertColumn(100, 1, "Info");
		InputHookLog->insertColumn(75, 2, "Individual");
		InputHookLog->insertColumn(75, 3, "Total");

		ActivityLog = new ListView(this);
		ActivityLog->setLocation(370, 10);
		ActivityLog->setSize(450, 200);
		ActivityLog->show();

		ActivityLog->insertColumn(75, 0, "Date");
		ActivityLog->insertColumn(60, 1, "Time");
		ActivityLog->insertColumn(290, 2, "Message");

		LastSaveLabel = new Label(this);
		LastSaveLabel->setLocation(310, 220);
		LastSaveLabel->setSize(250, 12);
		LastSaveLabel->setText("No stats saved since the program was opened.");
		LastSaveLabel->setFont(appFont);
		LastSaveLabel->show();

		ToggleStatSaving = new Checkbox(this);
		ToggleStatSaving->setLocation(10, 220);
		ToggleStatSaving->setSize(150, 12);
		ToggleStatSaving->setText("Automatically Save Stats");
		ToggleStatSaving->setFont(appFont);
		ToggleStatSaving->show();

		ToggleMinimizeToTray = new Checkbox(this);
		ToggleMinimizeToTray->setLocation(700, 220);
		ToggleMinimizeToTray->setSize(100, 12);
		ToggleMinimizeToTray->setText("Minimize To Tray");
		ToggleMinimizeToTray->setFont(appFont);
		ToggleMinimizeToTray->show();

		show();

		// Start the input stuff.
		activeStatSet = new StatisticsSet();
		activeStatSet->representedTime = getNow();

		if (activeStatSet->load())
		{
			char buf[200];
			sprintf(buf, "Successfully loaded stats file '%s'", getFilenameForDay(activeStatSet->representedTime).c_str());
			logActivity(buf);
			sprintf(buf, "LOAD: Keystrokes: %d | Clicks: %d", activeStatSet->getTotalKeypresses(), activeStatSet->getTotalMouseClicks());
			logActivity(buf);
		}
		else
		{
			logActivity("No stats file exists for today, using new stat set.");
		}

		startInputLogging();
	}

	void logActivity(char *msg)
	{
		cleanLog(ActivityLog);
		SYSTEMTIME now = getNow();
		char buf[20];
		sprintf(buf, "%d/%d/%d", now.wMonth, now.wDay, now.wYear);
		ActivityLog->insertItem(0, buf);
		sprintf(buf, "%d:%d:%d", now.wHour, now.wMinute, now.wSecond);
		ActivityLog->setItem(0, 1, buf);
		ActivityLog->setItem(0, 2, msg);
	}

	void logKeystroke(int keycode)
	{
		cleanLog(InputHookLog);
		char buf[20];
		InputHookLog->insertItem(0, "Keystroke");
		sprintf(buf, "%d", keycode);
		InputHookLog->setItem(0, 1, buf);
		sprintf(buf, "%d", activeStatSet->keypressCounts[keycode]);
		InputHookLog->setItem(0, 2, buf);
		sprintf(buf, "%d", activeStatSet->getTotalKeypresses());
		InputHookLog->setItem(0, 3, buf);
	}

	void logMouseclick(int button)
	{
		cleanLog(InputHookLog);
		char buf[20];
		InputHookLog->insertItem(0, "Click");
		button == 0 ? strcpy(buf, "Left") : strcpy(buf, "Right");
		InputHookLog->setItem(0, 1, buf);
		sprintf(buf, "%d", activeStatSet->clickCounts[button]);
		InputHookLog->setItem(0, 2, buf);
		sprintf(buf, "%d", activeStatSet->getTotalMouseClicks());
		InputHookLog->setItem(0, 3, buf);
	}

	void cleanLog(ListView *log)
	{
		if (log->getItemCount() > 20)
		{
			log->deleteItem(10);
		}
	}

	static StatTrackerForm *getSingleton()
	{
		return s;
	}

	static StatTrackerForm *s;
};

StatTrackerForm *StatTrackerForm::s = 0;

enum InputType
{
	IT_KEYBOARD,
	IT_MOUSEMOVE,
	IT_MOUSECLICK
};

struct InputMessage
{
	InputType type;
	union
	{
		struct KeybInfo
		{
			unsigned long vkCode;
		} ki;
		struct MouseClickInfo
		{
			short type;
			long x;
			long y;
		} mci;
		struct MouseMoveInfo
		{
			int button;
			int state;
		} mmi;
	};
};

void logInputMessage(InputMessage im)
{
	if (im.type == IT_MOUSECLICK)
	{
		int button = im.mci.type == WM_LBUTTONDOWN ? 0 : 1;
		activeStatSet->clickCounts[button]++;
		StatTrackerForm::getSingleton()->logMouseclick(button);
	}
	else if (im.type == IT_KEYBOARD)
	{
		activeStatSet->keypressCounts[im.ki.vkCode]++;
		StatTrackerForm::getSingleton()->logKeystroke(im.ki.vkCode);
	}
}

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0 && (wParam == WM_LBUTTONDOWN || wParam == WM_RBUTTONDOWN /* || wParam == WM_MOUSEMOVE || wParam == WM_LBUTTONUP || wParam == WM_RBUTTONUP */))
	{
		InputMessage im;
		im.type = IT_MOUSECLICK;

		MSLLHOOKSTRUCT *llHookStruct = (MSLLHOOKSTRUCT *)lParam;

		im.mci.type = wParam;
		im.mci.x = llHookStruct->pt.x;
		im.mci.y = llHookStruct->pt.y;

		logInputMessage(im);
	}
	else
	{
		return CallNextHookEx(0, nCode, wParam, lParam);
	}

	return 0;
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0 && (wParam == WM_KEYUP || wParam == WM_SYSKEYUP))
	{
		InputMessage im;
		im.type = IT_KEYBOARD;

		KBDLLHOOKSTRUCT *llHookStruct = (KBDLLHOOKSTRUCT *)lParam;

		im.ki.vkCode = llHookStruct->vkCode;

		logInputMessage(im);
	}
	else
	{
		return CallNextHookEx(0, nCode, wParam, lParam);
	}

	return 0;
}

void loadMouseHook()
{
	mouseHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, 0, 0);

	if (mouseHook == 0)
	{
		StatTrackerForm::getSingleton()->logActivity("Loading the mouse hook failed.");
	}
}

void loadKeyboardHook()
{
	keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);

	if (keyboardHook == 0)
	{
		StatTrackerForm::getSingleton()->logActivity("Loading the keyboard hook failed.");
	}
}

void unloadMouseHook()
{
	UnhookWindowsHookEx(mouseHook);
}

void unloadKeyboardHook()
{
	UnhookWindowsHookEx(keyboardHook);
}

void HookListenProcFunc()
{
	loadKeyboardHook();
	loadMouseHook();
	MSG msg;
	GetMessageA(&msg, 0, 0, 0);
	unloadKeyboardHook();
	unloadMouseHook();
}

void PeriodicStatSaveProcFunc()
{
	char buf[200];

	// Check if the hook thread is still running.
	while (GetThreadId(HookListenThread) != 0)
	{
		Sleep(FileSaveDelay);
		activeStatSet->save();
		sprintf(buf, "Last saved stats to '%s' at %d:%d.", getFilenameForDay(activeStatSet->representedTime).c_str(), activeStatSet->lastSaved.wHour, activeStatSet->lastSaved.wMinute);
		StatTrackerForm::getSingleton()->LastSaveLabel->setText(buf);

		// start new set for new day
		if (!activeStatSet->representsDate(getNow()))
		{
			StatisticsSet *oldSet = activeStatSet;
			activeStatSet = new StatisticsSet();
			delete oldSet;
			activeStatSet->representedTime = getNow();
			StatTrackerForm::getSingleton()->logActivity("Started new stat set for today.");
		}

		// To gracefully end the hook thread (for future reference if necessary)
		//PostThreadMessage(GetThreadId(HookListenThread), WM_QUIT, 0, 0);
	}
}

void startInputHooks()
{
	HookListenThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HookListenProcFunc, 0, 0, 0);

	if (HookListenThread == 0)
	{
		StatTrackerForm::getSingleton()->logActivity("Failed to start the input hook thread.");
	}
}

void startPeriodicStatSaving()
{
	PeriodicStatSavingThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)PeriodicStatSaveProcFunc, 0, 0, 0);

	if (PeriodicStatSavingThread == 0)
	{
		StatTrackerForm::getSingleton()->logActivity("Failed to start the periodic stat saving thread.");
	}
}

void startInputLogging()
{
	startInputHooks();
	startPeriodicStatSaving();
}

void runApp(Form *f)
{
	f->messageLoop();
}

void EnableVisualStyles()
{
	INITCOMMONCONTROLSEX ccex;
	ccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	ccex.dwICC = ICC_LISTVIEW_CLASSES | ICC_PROGRESS_CLASS | ICC_STANDARD_CLASSES;
	InitCommonControlsEx(&ccex);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	EnableVisualStyles();
	StatTrackerForm::s = new StatTrackerForm();
	runApp(StatTrackerForm::getSingleton());
	return 0;
}