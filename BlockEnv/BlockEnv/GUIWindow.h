#pragma once

#include "Rect.h"
#include "../Frost/String.h"
#include "Point.h"
#include "KeyboardKey.h"
#include <vector>

class GUIWindow
{
public:
	GUIWindow(void);
	GUIWindow(int iWindowType, long lX, long lY, long lWidth, long lHeight, GUIWindow *pParent);
	~GUIWindow(void);

	Rect ClientArea;
	String Text;
	String DefaultText;
	bool Visible;
	bool ReadOnly;
	bool Disabled;
	int WindowType;
	std::vector<GUIWindow*> Children;
	GUIWindow *Parent;
	GUIWindow *ActiveChild;
	int PushState;
	float CaretTime;
	unsigned int CaretPosition;
	bool UsesScrollbar;
	float ScrollbarPosition;
	unsigned long WindowFlags;
	bool BeingDragged;
	Point DragPoint;

	void SetParent(GUIWindow *newParent);
	bool HasChildren();
	void AddChild(GUIWindow *child);
	void RemoveChild(GUIWindow *child);
	void SetActiveChild(GUIWindow *child);
	GUIWindow *ChildAtPoint(Point coord);
	void BringToTop();

	void Hide();
	void Show();
	bool IsVisible();
	bool Active();
	bool IsForegroundWindow();
	bool IsForegroundChild();

	long X();
	void X(long NewX);
	long Y();
	void Y(long NewY);
	long Width();
	void Width(long NewWidth);
	long Height();
	void Height(long NewHeight);

	bool IsDesktop();
	bool IsWindow();
	bool IsButton();
	bool IsTextbox();

	bool CanBeTypedIn();
	bool IsMousedOver();
	
	void ResetCaretPosition();

	Point WindowToClient(Point coord);
	Point ClientToWindow(Point coord);

	void Draw();

	void (*OnClick)();
	void (*OnEnter)();
};

namespace GUI
{
	GUIWindow *GetDesktop();
	GUIWindow *GetActiveWindow();
	GUIWindow *GetActiveControl();
	void RecieveKeyboardKey(KeyboardKey *Key);

	namespace Chatbox
	{
		extern "C" GUIWindow *MainWnd;
		extern "C" GUIWindow *Message;
		extern "C" GUIWindow *ChatHistory;
		void Init();
		void OnEnter();
	};
};