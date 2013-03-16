#pragma once

#include "Includes.h"
#include "MapObject.h"

class CGUIWindow
{
public:
	CGUIWindow(void);
	~CGUIWindow(void);
	
	CGUIWindow *Parent;
	CGUIWindow *FindChildAtPoint(POINT coord);
	void SetParent(CGUIWindow* parent);
	void BringToTop();
	void AddChild(CGUIWindow* child);
	void RemoveChild(CGUIWindow* child);
	vector<CGUIWindow*> Children;
	bool HasChildren() {return (Children.size() > 0);}
	LOADEDTEXTURE* Background;
	bool Active;
	bool IsActive() {return (Active ? true : false);}
	bool IsAnElement;
	bool IsWindow() {return (!IsAnElement ? true : false);}
	bool IsElement() {return (IsAnElement ? true : false);}
	string Caption;
	long Height;
	long Width;
	bool Visible;
	void Hide() {	Visible = false;	}
	void Show() {	Visible = true;		}
	DWORD Color;
	long X;
	long Y;
	Rect GetRect();
	void DrawCore();
	virtual void DrawAll()
	{
		DrawCore();
	}
	POINT ScreenToWindow(POINT coord);
	POINT WindowToScreen(POINT coord);
	
	virtual void CX_WM_LBUTTONDOWN();
	virtual void CX_WM_LBUTTONUP();
	virtual void CX_WM_CLICK();
};

class CGUIDesktop : public CGUIWindow
{
public:
	CGUIDesktop(void);
	~CGUIDesktop(void);
	
	CGUIWindow *ActiveWindow;
	CGUIWindow *ActiveElement;
	void MakeActive(CGUIWindow *target);
	
	void CX_WM_LBUTTONDOWN();
	void CX_WM_LBUTTONUP();
};

class CGUITextbox : public CGUIWindow
{
public:
	CGUITextbox(void);
	~CGUITextbox(void);
	
	float CaretTime;
	void DrawAll();
};

class CGUIButton : public CGUIWindow
{
public:
	CGUIButton(void);
	~CGUIButton(void);
	
	int PushState;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CUI
{
public:
	CUI(void);
	~CUI(void);

	CMapObject *BarBG;
	CMapObject *HPBar;
	CMapObject *MPBar;
	CMapObject *EXPBar;
	CMapObject *LevelBG;

	void LoadBarBG();
	void LoadHPBar();
	void LoadMPBar();
	void LoadEXPBar();
	void LoadLevelBG();

	void DrawBarBGs();
	void DrawHPBar();
	void DrawMPBar();
	void DrawEXPBar();
	void DrawLevelBG();

	void DrawHPText(LPD3DXFONT Font, int X, int Y, DWORD Color);
	void DrawMPText(LPD3DXFONT Font, int X, int Y, DWORD Color);
	void DrawEXPText(LPD3DXFONT Font, int X, int Y, DWORD Color);

	int GetHPPixels();
	int GetMPPixels();
	int GetEXPPixels();

	void AutoScrollCamera();

	CGUIDesktop *Desktop;
};