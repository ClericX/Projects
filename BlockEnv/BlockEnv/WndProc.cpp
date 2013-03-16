#include <Windows.h>
#include "Application.h"
#include "Input.h"

LRESULT CALLBACK Application::WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_KEYDOWN:
		{
			switch (wParam)
			{
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
			}
		}
		break;

	case WM_CLOSE:
		{
			if (MessageBoxA(Application::Window, "Are you sure you want to quit?", "Quit", MB_YESNO) == IDYES)
			{
				//PostQuitMessage(0);
				DestroyWindow(hWnd);
			}
		}
		break;

	case WM_DESTROY:
		{
			Application::Running = false;
			PostQuitMessage(0);
		}
		break;

	case WM_MOUSEMOVE:
		{
			if (hWnd == Application::Window)
			{
				GetClientRect(Application::Window, &Application::ClientRect);
				Application::WndTopLeft.x = Application::ClientRect.left;
				Application::WndTopLeft.y = Application::ClientRect.top;
				Application::WndBottomRight.x = Application::ClientRect.right;
				Application::WndBottomRight.y = Application::ClientRect.bottom;
				ClientToScreen(Application::Window, &Application::WndTopLeft);
				ClientToScreen(Application::Window, &Application::WndBottomRight);
				// Cursor position updating is performed in the Input::Update() function so OldCursorPos is syncronized.

				if (Input::CursorPos.x < Application::WndTopLeft.x || Input::CursorPos.x > Application::WndBottomRight.x || Input::CursorPos.y < Application::WndTopLeft.y || Input::CursorPos.y > Application::WndBottomRight.y)
				{
					if (GetCursor() != Application::DefaultCursor)
					{
						//Out of bounds, change to regular cursor
						SetCursor(Application::DefaultCursor);
					}
				}
				else
				{
					if (GetCursor() != Application::InGameCursor)
					{
						//In bounds, change to in-game cursor
						SetCursor(Application::InGameCursor);
					}
				}
			}
		}
		break;

	default:
		{
			return DefWindowProc(hWnd, Msg, wParam, lParam);
		}
		break;
	}

	return 0;
}