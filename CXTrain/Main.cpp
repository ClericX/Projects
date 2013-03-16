#include <Windows.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")

#include "../../Moonlight Frost/Frost/XML.h"
#include "../../Moonlight Frost/Frost/String.h"

using namespace Frost;

typedef enum _AnimState
{
	WALKING_RIGHT = 2,
	WALKING_LEFT = 3,
	FACING_RIGHT = 4,
	FACING_LEFT = 5,
	FALLING_RIGHT = 6,
	FALLING_LEFT = 7,
	BREATHING_RIGHT = 8,
	BREATHING_LEFT = 9,
	DUCKING_RIGHT = 10,
	DUCKING_LEFT = 11,
	CLIMBING = 17,
} AnimState;

/*
BOOL MemoryEdit (VOID *lpMem,VOID *lpSrc,DWORD len)
{
	DWORD lpflOldProtect, flNewProtect = PAGE_READWRITE;
	unsigned char * pDst = (unsigned char *)lpMem,
	*pSrc = (unsigned char *)lpSrc;
	if (VirtualProtect(lpMem,len,flNewProtect,&lpflOldProtect))
	{
		while(len-- > 0) *pDst++ = *pSrc++;
		return (0);
	}  
	return (1);
}

DWORD ReadPointer(CONST LPDWORD lpdwBase, CONST INT iOffset)
{
	__try { return *(LPDWORD)(*lpdwBase + iOffset); }
	__except (EXCEPTION_EXECUTE_HANDLER) { return NULL; }
}

DWORD ReadValue(CONST LPDWORD lpdwBase, CONST INT iOffset)
{
	__try { return *(LPDWORD)(((char *)lpdwBase) + iOffset); }
	__except (EXCEPTION_EXECUTE_HANDLER) { return NULL; }
}

 //Write Pointer
bool WritePointer(unsigned long *ulBase, int iOffset, int iValue)
{
		__try { *(int*)(*ulBase + iOffset) = iValue; return true; }
		__except (EXCEPTION_EXECUTE_HANDLER) { return false; }
}
*/

__inline ULONG_PTR ReadPointer(ULONG_PTR* ulBase, INT nOffset)
{
   if (!IsBadReadPtr((void*)ulBase, sizeof(ULONG_PTR)))
	{
		if (!IsBadReadPtr((void*)((*(ULONG_PTR*)ulBase)+nOffset), sizeof(ULONG_PTR)))
		{
			return *(ULONG_PTR*)((*(ULONG_PTR*)ulBase)+nOffset);
		}
	}
	return 0;
}

__inline DWORD GetValue(DWORD ulBase, INT nOffset)
{
   if (!IsBadReadPtr((void*)(ulBase + nOffset), 4))
   {
		return *(DWORD*)(ulBase + nOffset);
   }
	return 0;
}

__inline void WritePointer(ULONG_PTR* ulBase, INT nOffset, DWORD dwValue)
{
   if ( !IsBadReadPtr((VOID*)ulBase, sizeof(ULONG_PTR)) )
	{
		if (!IsBadWritePtr((VOID*)((*(ULONG_PTR*)ulBase)+nOffset), sizeof(ULONG_PTR)) )
		{
			*(DWORD*)(*(DWORD*)ulBase + nOffset) = dwValue;
		}
	}

}

__inline DWORD WriteValue(DWORD dwBase, int iOffset, int iValue)
{
   if (!IsBadReadPtr((void*)(dwBase + iOffset), 4))
   {
		*(int*)(dwBase + iOffset) = iValue;
   }
	return 0;
}

HWND getProcessWindow(DWORD pID, char * className)
{
	char classBuff[256];
	DWORD tempID;
	HWND hWnd =	GetTopWindow(NULL);

	while (hWnd != NULL)
	{
		if (GetClassName(hWnd, classBuff, sizeof(classBuff)) > 0)
			if (strcmp(className, classBuff) == 0)
			{
				GetWindowThreadProcessId(hWnd, &tempID);
				if (tempID == pID)
					return hWnd;
			}
			hWnd = GetNextWindow(hWnd, GW_HWNDNEXT);
	}
	return NULL;
}

HWND getMapleWindow()
{
	return getProcessWindow(GetCurrentProcessId(), "MapleStoryClass");
}

typedef struct _RopeData
{
	int id;
	int unknown1; // 0
	int unknown2; // 1
	int x;
	int y1;
	int y2;
	int unknown3; // 2
} RopeData;

// Raw base / offset value list.
DWORD *MapleBase;
DWORD *AlertBase;
int HPAlertOffset;
int MPAlertOffset;
DWORD *StatBase;
int EXPPercentOffset;
int HPStatOffset;
int MPStatOffset;
DWORD *CharacterBase;
int CharBreathOffset;
int CharMovementFrameOffset;
int CharPosXOffset;
int CharPosYOffset;
int AttackCountOffset;
int MorphOffset;
DWORD *PeopleCountBase;
int PeopleCountOffset;
DWORD *ServerBase;
int TubiOffset;
int WorldIDOffset;
DWORD *ChannelAddress;
DWORD *MapInfoBase;
int MapIDOffset;
DWORD *MapBase;
int LeftWallOffset;
int TopWallOffset;
int RightWallOffset;
int BottomWallOffset;
int RopeDataOffset;
DWORD *MobBase;
int MobCountOffset;

void InitPointerLocations()
{
	MapleBase = (DWORD *)0x400000; //(DWORD *)GetModuleHandleA(0);
	AlertBase = (DWORD *)((char *)MapleBase + 0xD68FEC);
	HPAlertOffset = 0x50;
	MPAlertOffset = 0x54;
	StatBase = (DWORD *)((char *)MapleBase + 0xD692D0);
	EXPPercentOffset = 0x2120;
	HPStatOffset = 0x21f0;
	MPStatOffset = 0x21f4;
	CharacterBase = (DWORD *)((char *)MapleBase + 0xD647BC);
	CharBreathOffset = 0x6bc;
	CharMovementFrameOffset = 0x6c0;
	CharPosXOffset = 0x8230;
	CharPosYOffset = 0x8234;
	AttackCountOffset = 0x728c;
	MorphOffset = 0x634;
	PeopleCountBase = (DWORD *)((char *)MapleBase + 0xCEC140);
	PeopleCountOffset = 0x18;
	ServerBase = (DWORD *)((char *)MapleBase + 0xCE7930);
	TubiOffset = 0x2104;
	WorldIDOffset = 0x2098;
	ChannelAddress = (DWORD *)((char *)MapleBase + 0xD7ACE0);
	MapInfoBase = (DWORD *)((char *)MapleBase + 0xD69988);
	MapIDOffset = 0x11e0;
	MapBase = (DWORD *)((char *)MapleBase + 0xCEC0C0);
	LeftWallOffset = 0x1c;
	TopWallOffset = 0x20;
	RightWallOffset = 0x24;
	BottomWallOffset = 0x28;
	RopeDataOffset = 0xb4;
	MobBase = (DWORD *)((char *)MapleBase + 0xCEC144);
	MobCountOffset = 0x10;
}

unsigned int getHPAlert()
{
	return ReadPointer(AlertBase, HPAlertOffset);
}

unsigned int getMPAlert()
{
	return ReadPointer(AlertBase, MPAlertOffset);
}

void setHPAlert(unsigned int hpAlert)
{
	WritePointer(AlertBase, HPAlertOffset, hpAlert);
}

void setMPAlert(unsigned int mpAlert)
{
	WritePointer(AlertBase, MPAlertOffset, mpAlert);
}

double getEXPPercent()
{
	return ReadPointer(StatBase, EXPPercentOffset);
}

unsigned int getHP()
{
	if (getHPAlert() != 20)
	{
		setHPAlert(20);
	}

	return ReadPointer(StatBase, HPStatOffset);
}

unsigned int getMP()
{
	if (getMPAlert() != 20)
	{
		setMPAlert(20);
	}

	return ReadPointer(StatBase, MPStatOffset);
}

unsigned int getBreath()
{
	return ReadPointer(CharacterBase, CharBreathOffset);
}

int getX()
{
	return ReadPointer(CharacterBase, CharPosXOffset);
}

int getY()
{
	return ReadPointer(CharacterBase, CharPosYOffset);
}

unsigned int getMovementFrame()
{
	return ReadPointer(CharacterBase, CharMovementFrameOffset);
}

unsigned int getPeopleCount()
{
	return ReadPointer(PeopleCountBase, PeopleCountOffset);
}

unsigned int getTubi()
{
	return ReadPointer(ServerBase, TubiOffset);
}

void setTubi(unsigned int val)
{
	WritePointer(ServerBase, TubiOffset, val);
}

unsigned int getWorldID()
{
	return ReadPointer(ServerBase, WorldIDOffset);
}

unsigned int getChannel()
{
	return GetValue((DWORD)ChannelAddress, 0) + 1;
}

unsigned int getMapID()
{
	return ReadPointer(MapInfoBase, MapIDOffset);
}

int getLeftWall()
{
	return ReadPointer(MapBase, LeftWallOffset);
}

int getTopWall()
{
	return ReadPointer(MapBase, TopWallOffset);
}

int getRightWall()
{
	return ReadPointer(MapBase, RightWallOffset);
}

int getBottomWall()
{
	return ReadPointer(MapBase, BottomWallOffset);
}

unsigned int getNumRopes()
{
	return ReadPointer((ULONG_PTR*)ReadPointer(MapBase, RopeDataOffset), -4) - 1;
}

RopeData *getRopeData(unsigned int ropeNum)
{
	// last reported correct size of rope data struct was 1C (28) which is the size of our RopeData struct.
	return (RopeData *)(ReadPointer(MapBase, RopeDataOffset) + ( sizeof(RopeData) * (ropeNum + 1) ));
}

unsigned int getMobCount()
{
	return ReadPointer(MobBase, MobCountOffset);
}

int getMobX(unsigned int mobNum)
{
	/*
	Mob_Base 0x010EC144
	Mob_Count_Offset 0x10
	Mob_1 0x28
	Mob_2 0x04
	Mob_3 0x130
	Mob_4 0x24
	Mob_X 0x58
	Mob_Y 0x5C
	*/
	unsigned int ret = (DWORD)MobBase;
	ret = GetValue(ret, 0);
	ret = GetValue(ret, 0x28);
	ret = GetValue(ret, (0x04 + (0x1C * mobNum)));
	ret = GetValue(ret, 0x130);
	ret = GetValue(ret, 0x24);
	ret = GetValue(ret, 0x58);
	return ret;
}

int getMobY(unsigned int mobNum)
{
	unsigned int ret = (DWORD)MobBase;
	ret = GetValue(ret, 0);
	ret = GetValue(ret, 0x28);
	ret = GetValue(ret, (0x04 + (0x1C * mobNum)));
	ret = GetValue(ret, 0x130);
	ret = GetValue(ret, 0x24);
	ret = GetValue(ret, 0x5C);
	return ret;
}

// GUI-related code //

namespace Forms
{
	enum WindowType
	{
		Window,
		Button,
		TextBox,
		Menu,
		Label,
		ScrollBar,
		ComboBox,
		ListBox,
		GroupBox
	};
}

typedef enum _StartPosition
{
	Manual,
	CenterScreen,
	WindowsDefaultLocation,
	WindowsDefaultBounds,
	CenterParent
} StartPosition;

LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

class Window
{
public:
	Window(Forms::WindowType wndType)
	{
		WndList.push_back(this);

		onclick = 0;
		winHandle = 0;
		setWindowType(wndType);
		setStartPosition(StartPosition::Manual);
		setLocation(0, 0);
		setSize(0, 0);
		setParent(0);
		setVisible(false);
		setEnabled(true);
		setText("");
		setMinimizeBox(false);
		setMaximizeBox(false);
		setResizable(false);
		setBorder(false);
		setTitleBar(false);
		setWindowMenu(false);
		setToolWindow(false);
		setTopmost(false);

		winHandle = CreateWindowExA(ExStyle, WndClassName, Text, Style, x, y, nWidth, nHeight, Parent ? Parent->winHandle : 0, Menu, GetModuleHandle(0), 0);

		if (winHandle == 0)
		{
			MessageBoxA(0, "Failed to create a requested window.", "Fatal error", MB_OK);
		}
		else
		{
			setFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
		}
	}
	~Window()
	{
		if (winHandle)
		{
			DestroyWindow(winHandle);
		}

		if (wndType == Forms::WindowType::Window)
		{
			UnregisterClassA(WndClassName, GetModuleHandle(0));
		}
	}

	void setWindowType(Forms::WindowType wndType)
	{
		this->wndType = wndType;

		switch (this->wndType)
		{
		case Forms::WindowType::Window:
			{
				WndClassName = String("Frost_") + (int)GetTickCount();

				WNDCLASSEX WindowClass;

				WindowClass.lpszClassName	= WndClassName;
				WindowClass.cbSize			= sizeof(WNDCLASSEX);
				WindowClass.style			= 0; // CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
				WindowClass.lpfnWndProc		= WndProc;
				WindowClass.hInstance		= GetModuleHandle(0);
				WindowClass.hIcon			= LoadIcon(WindowClass.hInstance, IDI_APPLICATION);
				WindowClass.hIconSm			= LoadIcon(WindowClass.hInstance, IDI_APPLICATION);
				WindowClass.hCursor			= LoadCursorA(0, (LPCSTR)(IDC_ARROW));
				WindowClass.hbrBackground	= (HBRUSH)(COLOR_WINDOW); //(HBRUSH)GetStockObject(BLACK_BRUSH);
				WindowClass.lpszMenuName	= NULL;
				WindowClass.cbClsExtra		= 0;
				WindowClass.cbWndExtra		= 0;

				if( !RegisterClassEx(&WindowClass) )
				{
					MessageBoxA(0, "Failed to register window class.", "Fatal error", MB_OK);
				}
			}
			break;

		case Forms::WindowType::Button:
			{
				WndClassName = "BUTTON";
			}
			break;

		case Forms::WindowType::TextBox:
			{
				WndClassName = "EDIT";
				ExStyle |= WS_EX_CLIENTEDGE;
				setMultiline(true);
				//Style |= WS_VSCROLL;
			}
			break;

		case Forms::WindowType::Label:
			{
				WndClassName = "STATIC";
			}
			break;

		case Forms::WindowType::ScrollBar:
			{
				WndClassName = "SCROLLBAR";
			}
			break;

		case Forms::WindowType::ComboBox:
			{
				WndClassName = "COMBOBOX";
			}
			break;

		case Forms::WindowType::ListBox:
			{
				WndClassName = "LISTBOX";
			}
			break;

		case Forms::WindowType::GroupBox:
			{
				WndClassName = "BUTTON";
				Style |= BS_GROUPBOX;
			}
			break;
		}
	}

	void setStartPosition(StartPosition sp)
	{
		startPos = sp;
	}

	void setLocation(unsigned int _x, unsigned int _y)
	{
		Location.x = _x;
		Location.y = _y;

		if (startPos == StartPosition::WindowsDefaultLocation || startPos == StartPosition::WindowsDefaultBounds)
		{
			x = CW_USEDEFAULT;
			y = CW_USEDEFAULT;
		}
		else
		{
			x = Location.x;
			y = Location.y;
		}

		if (winHandle)
		{
			SetWindowPos(winHandle, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}
	}

	void setSize(unsigned int width, unsigned int height)
	{
		Size.x = width;
		Size.y = height;

		if (startPos == StartPosition::WindowsDefaultBounds)
		{
			nWidth = CW_USEDEFAULT;
			nHeight = CW_USEDEFAULT;
		}
		else
		{
			nWidth = Size.x;
			nHeight = Size.y;
		}

		if (winHandle)
		{
			SetWindowPos(winHandle, 0, 0, 0, nWidth, nHeight, SWP_NOMOVE | SWP_NOZORDER);
		}
	}

	void setBounds(unsigned int _x, unsigned int _y, unsigned int width, unsigned int height)
	{
		setLocation(_x, _y);
		setSize(width, height);
	}

	void setParent(Window *p)
	{
		Parent = p;

		setWindowStyle(ChildWindow, WS_CHILD, Parent ? true : false);

		if (winHandle)
		{
			SetParent(winHandle, p->winHandle);
		}
	}

	void setVisible(bool visible)
	{
		Visible = visible;

		if (Visible)
		{
			Style |= WS_VISIBLE;
		}
		else
		{
			Style &= ~WS_VISIBLE;
		}

		if (winHandle)
		{
			ShowWindow(winHandle, Visible ? SW_SHOWNA : SW_HIDE);
		}
	}

	void setEnabled(bool enabled)
	{
		Enabled = enabled;

		if (!Enabled)
		{
			Style |= WS_DISABLED;
		}
		else
		{
			Style &= ~WS_DISABLED;
		}

		if (winHandle)
		{
			EnableWindow(winHandle, Enabled);
		}
	}

	String getText()
	{
		return Text;
	}

	void setText(String txt)
	{
		if (Text != txt)
		{
			Text = txt;

			if (winHandle)
			{
				SetWindowTextA(winHandle, Text);
			}
		}
	}

	void setMinimizeBox(bool val)
	{
		setWindowStyle(MinimizeBox, WS_MINIMIZEBOX, val);
	}

	void setMaximizeBox(bool val)
	{
		setWindowStyle(MaximizeBox, WS_MAXIMIZEBOX, val);
	}

	void setResizable(bool val)
	{
		setWindowStyle(Resizable, WS_THICKFRAME, val);
	}

	void setBorder(bool val)
	{
		setWindowStyle(Border, WS_BORDER, val);
	}

	void setTitleBar(bool val)
	{
		setWindowStyle(TitleBar, WS_CAPTION, val);
	}

	void setWindowMenu(bool val)
	{
		setWindowStyle(WindowMenu, WS_SYSMENU, val);
	}

	void setToolWindow(bool val)
	{
		setExtendedWindowStyle(ToolWindow, WS_EX_TOOLWINDOW, val);
	}

	void setTopmost(bool val)
	{
		setExtendedWindowStyle(Topmost, WS_EX_TOPMOST, val);
	}

	void setMultiline(bool val)
	{
		setWindowStyle(Multiline, ES_MULTILINE, val);
	}

	void setFont(HFONT fnt)
	{
		if (winHandle)
		{
			SendMessageA(winHandle, WM_SETFONT, (WPARAM)fnt, (LPARAM)false);
		}
	}

	void (* onclick)();

	HWND getNativeHandle()
	{
		return winHandle;
	}

	static void EnableVisualStyles()
	{
		INITCOMMONCONTROLSEX icc;

		icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
		icc.dwICC = ICC_STANDARD_CLASSES;

		InitCommonControlsEx(&icc);
	}

	static int Run()
	{
		MSG Msg;

		while(GetMessageA(&Msg, NULL, 0, 0) > 0)
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}

		return Msg.wParam;
	}

	static std::vector<Window *> WndList;

private:
	// Higher level but still pretty raw variables.
	bool CreatedWndClass;
	Window *Parent;
	Forms::WindowType wndType;
	StartPosition startPos;
	POINT Location;
	POINT Size;
	bool Visible;
	bool Enabled;
	bool ChildWindow;
	bool MinimizeBox;
	bool MaximizeBox;
	bool Resizable;
	bool Border;
	bool TitleBar;
	bool WindowMenu;
	bool ToolWindow;
	bool Topmost;
	bool Multiline;

	// Native raw shit to pass to window commands as parameters.
	HWND winHandle;
	DWORD ExStyle;
	String WndClassName;
	String Text;
	DWORD Style;
	HMENU Menu;
	int x;
	int y;
	int nWidth;
	int nHeight;

	void setWindowStyle(bool &internalVar, DWORD styleFlag, bool val)
	{
		internalVar = val;

		if (internalVar)
		{
			Style |= styleFlag;
		}
		else
		{
			Style &= ~styleFlag;
		}

		if (winHandle)
		{
			SetWindowLongPtr(winHandle, GWL_STYLE, Style);
			SetWindowPos(winHandle, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
		}
	}

	void setExtendedWindowStyle(bool &internalVar, DWORD styleFlag, bool val)
	{
		internalVar = val;

		if (internalVar)
		{
			ExStyle |= styleFlag;
		}
		else
		{
			ExStyle &= ~styleFlag;
		}

		if (winHandle)
		{
			SetWindowLongPtr(winHandle, GWL_EXSTYLE, ExStyle);
			SetWindowPos(winHandle, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
		}
	}

	// Static shit.
	static unsigned int NumAllocWndClasses;
};

std::vector<Window *> Window::WndList;

LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
	case WM_CREATE:
		//here you can catch on window creation message (usually used to do initalization of variables and etc)
		break;
	case WM_SIZE:
		//here you can catch changes of size (could be used for auto resize)
		break;
	case WM_NOTIFY:
		//here you can catch norifcation messages (used by many controls for many diffrent purposses
		break;
	case WM_COMMAND:
		//here you can catch clicks (usually used to catch button clicks)
		for (unsigned int i = 0; i < Window::WndList.size(); i++)
		{
			Window *theWnd = Window::WndList[i];
			if (theWnd->getNativeHandle() == (HWND)lParam)
			{
				if (theWnd->onclick)
				{
					theWnd->onclick();
				}
			}
		}
		break;
	case WM_CLOSE:
		//on window close message (alt+f4, 'x' button click and etc)
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		//on window destroy message its pretty much self documents
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcA(hWnd, Msg, wParam, lParam);
		break;
	}
}

// GUI-related code //

unsigned int Distance(int x, int x2)
{
	return abs(x - x2);
}

Window *wnd;
Window *infoGroupBox;
Window *hpLabel;
Window *mpLabel;
Window *expPercentLabel;
Window *breathLabel;
Window *charLocLabel;
Window *peopleLabel;
Window *mobLabel;
Window *worldLabel;
Window *channelLabel;
Window *mapIDLabel;
Window *startBotButton;
Window *mobLocationsTextbox;
Window *horzAtkRangeTextbox;
Window *vertAtkRangeTextbox;
Window *hpPotAmountTextbox;
Window *mpPotAmountTextbox;

bool botting = false;

int HorizontalAttackRange = 200;
int VerticalAttackRange = 100;
unsigned int AutoPotHP = 100;
unsigned int AutoPotMP = 100;

bool isFacingLeft()
{
	return getMovementFrame() == AnimState::BREATHING_LEFT || getMovementFrame() == AnimState::DUCKING_LEFT || getMovementFrame() == AnimState::FACING_LEFT || getMovementFrame() == AnimState::FALLING_LEFT || getMovementFrame() == AnimState::WALKING_LEFT;
}

bool isFacingRight()
{
	return getMovementFrame() == AnimState::BREATHING_RIGHT || getMovementFrame() == AnimState::DUCKING_RIGHT || getMovementFrame() == AnimState::FACING_RIGHT || getMovementFrame() == AnimState::FALLING_RIGHT || getMovementFrame() == AnimState::WALKING_RIGHT;
}

#define MOVEDIR_IDLE 0
#define MOVEDIR_LEFT VK_LEFT
#define MOVEDIR_RIGHT VK_RIGHT

short curMoveDir = MOVEDIR_IDLE;

// Emulate physical keystrokes.
// With a bypass, this functionality could be replaced with the GetAsyncKeyState() hook combined with active window check hooks.
void sendKeyPress(short wVk, bool release)
{
	INPUT in;
	in.type = INPUT_KEYBOARD;
	in.ki.wVk = wVk;
	in.ki.wScan = 0;
	in.ki.dwFlags = (release ? KEYEVENTF_KEYUP : 0);
	in.ki.time = 0;
	in.ki.dwExtraInfo = 0;

	SendInput(1, &in, sizeof(INPUT));
}

void setMovementDirection(short desiredMoveDir)
{
	// Implicitly means if you set to idle the bot won't move you.
	if (desiredMoveDir != curMoveDir)
	{
		sendKeyPress(curMoveDir, true);
	}

	// Any direction other than idle should press a key after the previous release.
	if (desiredMoveDir != MOVEDIR_IDLE)
	{
		sendKeyPress(desiredMoveDir, false);
	}

	// This is purely internal to the bot.
	curMoveDir = desiredMoveDir;
}

void botProcedure()
{
	while (botting)
	{
		// Update info labels
		hpLabel->setText(String("HP: ") + getHP());
		mpLabel->setText(String("MP: ") + getMP());
		expPercentLabel->setText(String("EXP: ") + String((float)getEXPPercent(), 2) + "%");
		breathLabel->setText(String("Breath: ") + getBreath());
		charLocLabel->setText(String("Pos: ") + getX() + ", " + getY());
		peopleLabel->setText(String("People: ") + getPeopleCount());
		mobLabel->setText(String("Mobs: ") + getMobCount());
		worldLabel->setText(String("World: ") + getWorldID());
		channelLabel->setText(String("Channel: ") + getChannel());
		mapIDLabel->setText(String("Map ID: ") + getMapID());

		String mobLocs;

		for (int i = -20; i < 20; i++)
		{
			mobLocs << i << " : " << getMobX(i) << ", " << getMobY(i) << "\r\n";
		}

		mobLocationsTextbox->setText(mobLocs);

		if (getHP() <= AutoPotHP)
		{
			SendMessageA(getMapleWindow(), WM_KEYDOWN, VK_PRIOR, MapVirtualKey(VK_PRIOR, 0) << 16);
		}

		if (getMP() <= AutoPotMP)
		{
			SendMessageA(getMapleWindow(), WM_KEYDOWN, VK_NEXT, MapVirtualKey(VK_NEXT, 0) << 16);
		}

		// Only bother doing bot processing if there are actually mobs to potentially process.
		if (getMobCount() > 0)
		{
			/////////////////////////////////////////////////
			// STEP 1: Find the closest monster to attack. //
			/////////////////////////////////////////////////

			// Single platform logic (this can also be used on a per-platform basis.)
			unsigned int shortestDistanceMobID = 0;

			for (int i = -20; i < 20; i++)
			{
				if (Distance(getMobX(i), getX()) < Distance(getMobX(shortestDistanceMobID), getX()) && Distance(getMobY(shortestDistanceMobID), getY()) < VerticalAttackRange)
				{
					shortestDistanceMobID = i;
				}
			}

			////////////////////////////////////////////////////////////////////////////////////////////////
			// STEP 2: Walk toward it. (here you have logic for climbing ladders and shit for multi-plat) //
			////////////////////////////////////////////////////////////////////////////////////////////////

			// If the height is so that you need to jump, once reaching a certain closeness to the rope, attempt jumping.
			// If not climbing, turn around and try again (nnb style)

			if (getMobX(shortestDistanceMobID) < getX())
			{
				// walk left
				setMovementDirection(MOVEDIR_LEFT);
			}
			else
			{
				// walk right
				setMovementDirection(MOVEDIR_RIGHT);
			}

			///////////////////////////////
			// STEP 3: If close, attack. //
			///////////////////////////////

			if (Distance(getMobX(shortestDistanceMobID), getX()) < HorizontalAttackRange) // You can change this attack range as a setting.
			{
				// check if facing the mob correctly, if not move that way
				if (getMobX(shortestDistanceMobID) < getX() && !isFacingLeft())
				{
					setMovementDirection(MOVEDIR_LEFT);
				}
				else if (getMobX(shortestDistanceMobID) > getX() && !isFacingRight())
				{
					setMovementDirection(MOVEDIR_RIGHT);
				}
				else // attack if facing the right way
				{
					PostMessageA(getMapleWindow(), WM_KEYDOWN, 0x44, MapVirtualKeyA(0x44, 0) << 16);
				}
			}

			// check if monster is dead
			// can be done with checking if EXP % increased.
		}

		Sleep(100);
	}
}

void doBot()
{
	if (botting)
	{
		botting = false;
		startBotButton->setText("Start Botting");
	}
	else
	{
		botting = true;
		startBotButton->setText("Stop Botting");
		AutoPotHP = hpPotAmountTextbox->getText().toInt();
		AutoPotMP = mpPotAmountTextbox->getText().toInt();
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)botProcedure, 0, 0, 0);
	}
}

void InitializeComponent()
{
	wnd = new Window(Forms::WindowType::Window);
	wnd->setStartPosition(StartPosition::WindowsDefaultLocation);
	//wnd->setLocation(0, 0);
	wnd->setSize(400, 700);
	wnd->setMinimizeBox(true);
	wnd->setBorder(true);
	wnd->setTitleBar(true);
	wnd->setWindowMenu(true);
	wnd->setText("CXTrain v1");
	wnd->setVisible(true);

	infoGroupBox = new Window(Forms::WindowType::GroupBox);
	infoGroupBox->setBounds(10, 10, 170, 180);
	infoGroupBox->setText("Character Info");
	infoGroupBox->setParent(wnd);
	infoGroupBox->setVisible(true);

	hpLabel = new Window(Forms::WindowType::Label);
	hpLabel->setBounds(10, 20, 25, 15);
	hpLabel->setText("HP: 0 / 0");
	hpLabel->setParent(infoGroupBox);
	hpLabel->setVisible(true);

	mpLabel = new Window(Forms::WindowType::Label);
	mpLabel->setBounds(10, 35, 25, 15);
	mpLabel->setText("MP: 0 / 0");
	mpLabel->setParent(infoGroupBox);
	mpLabel->setVisible(true);

	expPercentLabel = new Window(Forms::WindowType::Label);
	expPercentLabel->setBounds(10, 50, 25, 15);
	expPercentLabel->setText("EXP: 0.00%");
	expPercentLabel->setParent(infoGroupBox);
	expPercentLabel->setVisible(true);

	breathLabel = new Window(Forms::WindowType::Label);
	breathLabel->setBounds(10, 65, 25, 15);
	breathLabel->setText("Breath: 0");
	breathLabel->setParent(infoGroupBox);
	breathLabel->setVisible(true);

	charLocLabel = new Window(Forms::WindowType::Label);
	charLocLabel->setBounds(10, 80, 25, 15);
	charLocLabel->setText("Pos: 0, 0");
	charLocLabel->setParent(infoGroupBox);
	charLocLabel->setVisible(true);

	peopleLabel = new Window(Forms::WindowType::Label);
	peopleLabel->setBounds(10, 95, 25, 15);
	peopleLabel->setText("People: 0");
	peopleLabel->setParent(infoGroupBox);
	peopleLabel->setVisible(true);

	mobLabel = new Window(Forms::WindowType::Label);
	mobLabel->setBounds(10, 110, 25, 15);
	mobLabel->setText("Mobs: 0");
	mobLabel->setParent(infoGroupBox);
	mobLabel->setVisible(true);

	worldLabel = new Window(Forms::WindowType::Label);
	worldLabel->setBounds(10, 125, 25, 15);
	worldLabel->setText("World: Scania");
	worldLabel->setParent(infoGroupBox);
	worldLabel->setVisible(true);

	channelLabel = new Window(Forms::WindowType::Label);
	channelLabel->setBounds(10, 140, 25, 15);
	channelLabel->setText("Channel: 1");
	channelLabel->setParent(infoGroupBox);
	channelLabel->setVisible(true);

	mapIDLabel = new Window(Forms::WindowType::Label);
	mapIDLabel->setBounds(10, 155, 25, 15);
	mapIDLabel->setText("Map ID: 00000000");
	mapIDLabel->setParent(infoGroupBox);
	mapIDLabel->setVisible(true);

	startBotButton = new Window(Forms::WindowType::Button);
	startBotButton->setBounds(200, 10, 100, 20);
	startBotButton->setText("Start Botting");
	startBotButton->setParent(wnd);
	startBotButton->onclick = doBot;
	startBotButton->setVisible(true);

	mobLocationsTextbox = new Window(Forms::WindowType::TextBox);
	mobLocationsTextbox->setBounds(200, 50, 150, 600);
	mobLocationsTextbox->setMultiline(true);
	mobLocationsTextbox->setParent(wnd);
	mobLocationsTextbox->setVisible(true);

	horzAtkRangeTextbox = new Window(Forms::WindowType::TextBox);
	horzAtkRangeTextbox->setBounds(10, 220, 50, 15);
	horzAtkRangeTextbox->setText("200");
	horzAtkRangeTextbox->setParent(wnd);
	horzAtkRangeTextbox->setVisible(true);

	vertAtkRangeTextbox = new Window(Forms::WindowType::TextBox);
	vertAtkRangeTextbox->setBounds(10, 240, 50, 15);
	vertAtkRangeTextbox->setText("100");
	vertAtkRangeTextbox->setParent(wnd);
	vertAtkRangeTextbox->setVisible(true);

	hpPotAmountTextbox = new Window(Forms::WindowType::TextBox);
	hpPotAmountTextbox->setBounds(70, 220, 50, 15);
	hpPotAmountTextbox->setText("100");
	hpPotAmountTextbox->setParent(wnd);
	hpPotAmountTextbox->setVisible(true);

	mpPotAmountTextbox = new Window(Forms::WindowType::TextBox);
	mpPotAmountTextbox->setBounds(70, 240, 50, 15);
	mpPotAmountTextbox->setText("100");
	mpPotAmountTextbox->setParent(wnd);
	mpPotAmountTextbox->setVisible(true);
}

void __stdcall r34l()
{
	InitPointerLocations();

	Window::EnableVisualStyles();
	InitializeComponent();
	Window::Run();
}

BOOL WINAPI DllMain(
	HINSTANCE hinstDLL,  // handle to DLL module
	DWORD fdwReason,     // reason for calling function
	LPVOID lpReserved )  // reserved
{
	// Perform actions based on the reason for calling.
	switch( fdwReason ) 
	{ 
		case DLL_PROCESS_ATTACH:
		 // Initialize once for each new process.
		 // Return FALSE to fail DLL load.
			DisableThreadLibraryCalls(hinstDLL);
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)r34l, 0, 0, 0);
			break;

		case DLL_THREAD_ATTACH:
		 // Do thread-specific initialization.
			break;

		case DLL_THREAD_DETACH:
		 // Do thread-specific cleanup.
			break;

		case DLL_PROCESS_DETACH:
		 // Perform any necessary cleanup.
			break;
	}
	return TRUE;  // Successful DLL_PROCESS_ATTACH.
}