#pragma once

#include <Windows.h>
#include <CommCtrl.h>

class WindowBase
{
public:
	WindowBase()
	{
		handle = 0;
		txtBuf = 0;
		WindowClassName = 0;
		ExtendedWindowStyle = 0;
		WindowStyle = 0;

		setParent(0);
		setText("");
		hide();
		setLocation(CW_USEDEFAULT, CW_USEDEFAULT);
		setSize(CW_USEDEFAULT, CW_USEDEFAULT);
	}
	~WindowBase()
	{
		clearTextBuffer();
	}

	HWND handle;
	unsigned int txtLen;
	char *txtBuf;
	bool visible;
	unsigned int x;
	unsigned int y;
	unsigned int width;
	unsigned int height;
	char *WindowClassName;
	DWORD ExtendedWindowStyle;
	DWORD WindowStyle;
	WindowBase *ParentWindow;
	HFONT usedFont;

	void clearTextBuffer()
	{
		if (txtBuf != 0)
		{
			delete[] txtBuf;
		}

		txtBuf = 0;
	}

	void setText(char *txt)
	{
		clearTextBuffer();
		txtLen = strlen(txt);
		txtBuf = new char[txtLen + 1];
		txtBuf[txtLen] = '\0';
		
		for (unsigned int i = 0; i < txtLen; i++)
		{
			txtBuf[i] = txt[i];
		}

		if (handle)
		{
			SetWindowText(handle, txt);
		}
	}

	char *getText()
	{
		clearTextBuffer();
		txtLen = GetWindowTextLength(handle);
		txtBuf = new char[txtLen + 1];
		GetWindowText(handle, txtBuf, txtLen);
	}

	void show()
	{
		visible = true;

		if (handle)
		{
			ShowWindow(handle, SW_SHOW);
			UpdateWindow(handle);
		}
	}

	void hide()
	{
		visible = false;

		if (handle)
		{
			ShowWindow(handle, SW_HIDE);
		}
	}

	void setLocation(unsigned int _x, unsigned int _y)
	{
		x = _x;
		y = _y;

		if (handle != 0)
		{
			SetWindowPos(handle, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}
	}

	void setSize(unsigned int _width, unsigned int _height)
	{
		width = _width;
		height = _height;

		if (handle != 0)
		{
			SetWindowPos(handle, 0, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
		}
	}

	void setParent(WindowBase *wnd)
	{
		ParentWindow = wnd;

		if (ParentWindow != 0)
		{
			WindowStyle |= WS_CHILD;
		}
	}

	void createNativeWindow()
	{
		handle = CreateWindowExA(
			ExtendedWindowStyle,
			WindowClassName,
			txtBuf,
			WindowStyle,
			x, y, width, height,
			(ParentWindow != 0 ? ParentWindow->handle : 0),
			NULL, GetModuleHandle(0), NULL);

		if(handle == 0)
		{
			MessageBox(0, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		}
	}

	void destroyNativeWindow()
	{
		DestroyWindow(handle);
		handle = 0;
	}

	void setFont(HFONT fnt)
	{
		usedFont = fnt;

		if (handle != 0)
		{
			SendMessage(handle, WM_SETFONT, (WPARAM)fnt, true);
		}
	}
};

class Form : public WindowBase
{
public:
	Form()
	{
		registerWindowClass();
		WindowClassName = (char *)wc.lpszClassName;
		ExtendedWindowStyle |= WS_EX_CLIENTEDGE;
		//WindowStyle = WS_OVERLAPPEDWINDOW;
		WindowStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
		createNativeWindow();
	}
	~Form()
	{
		unregisterWindowClass();
	}

	WNDCLASSEX wc;
	MSG Msg;

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch(msg)
		{
			case WM_CLOSE:
				DestroyWindow(hwnd);
			break;
			case WM_DESTROY:
				PostQuitMessage(0);
			break;
			default:
				return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		return 0;
	}

	void registerWindowClass()
	{
		//Step 1: Registering the Window Class
		wc.cbSize        = sizeof(WNDCLASSEX);
		wc.style         = 0;
		wc.lpfnWndProc   = WndProc;
		wc.cbClsExtra    = 0;
		wc.cbWndExtra    = 0;
		wc.hInstance     = GetModuleHandle(0);
		wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
		wc.lpszMenuName  = NULL;
		wc.lpszClassName = "myWindowClass";
		wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

		if(!RegisterClassEx(&wc))
		{
			MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		}
	}

	void unregisterWindowClass()
	{
		UnregisterClass(wc.lpszClassName, wc.hInstance);
	}

	int messageLoop()
	{
		// Step 3: The Message Loop
		while(GetMessage(&Msg, NULL, 0, 0) > 0)
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
		return Msg.wParam;
	}

	void setClientAreaSize(unsigned int _width, unsigned int _height)
	{
		setSize(_width, _height);
		RECT rc;
		rc.top = y;
		rc.left = x;
		rc.right = x + width;
		rc.bottom = y + height;
		AdjustWindowRectEx(&rc, WindowStyle, true, ExtendedWindowStyle);
		setSize(rc.right - rc.left, rc.bottom - rc.top);
	}
};

class Button : public WindowBase
{
public:
	Button(Form *parent)
	{
		WindowClassName = "BUTTON";
		setParent(parent);
		createNativeWindow();
	}
};

class Textbox : public WindowBase
{
public:
	Textbox(Form *parent)
	{
		WindowClassName = "EDIT";
		setParent(parent);
		createNativeWindow();
	}
};

class ListView : public WindowBase
{
public:
	ListView(Form *parent)
	{
		WindowClassName = "SysListView32";
		setParent(parent);
		WindowStyle |= LVS_REPORT;
		createNativeWindow();
	}

	void insertColumn(int _width, int index, char *text)
	{
		LVCOLUMN lvColumn;
		lvColumn.mask = LVCF_WIDTH | LVCF_TEXT;
		lvColumn.cx = _width;
		lvColumn.pszText = text;
		SendMessage(handle, LVM_INSERTCOLUMN, (WPARAM)index, (LPARAM)&lvColumn);
	}

	int insertItem(int index, char *text)
	{
		LVITEM lvItem;
		lvItem.mask = LVIF_TEXT | LVIF_STATE;
		lvItem.iItem = index;
		lvItem.pszText = text;
		lvItem.stateMask = 0;
		lvItem.iSubItem  = 0;
		lvItem.state     = 0;
		return SendMessage(handle, LVM_INSERTITEM, 0, (LPARAM)&lvItem);
	}

	void deleteItem(int index)
	{
		SendMessage(handle, LVM_DELETEITEM, index, 0);
	}

	void setItem(int index, int column, char *text)
	{
		LVITEM lvItem;
		lvItem.mask = LVIF_TEXT | LVIF_STATE;
		lvItem.iItem = index;
		lvItem.pszText = text;
		lvItem.stateMask = 0;
		lvItem.iSubItem  = column;
		lvItem.state     = 0;
		SendMessage(handle, LVM_SETITEM, 0, (LPARAM)&lvItem);
	}

	int getItemCount()
	{
		return SendMessage(handle, LVM_GETITEMCOUNT, 0, 0);
	}
};

class Label : public WindowBase
{
public:
	Label(Form *parent)
	{
		WindowClassName = "STATIC";
		setParent(parent);
		createNativeWindow();
	}
};

class Checkbox : public WindowBase
{
public:
	Checkbox(Form *parent)
	{
		WindowClassName = "BUTTON";
		setParent(parent);
		WindowStyle |= BS_CHECKBOX;
		createNativeWindow();
	}
};