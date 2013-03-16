#include "Includes.h"
#include "Main.h"

LRESULT CALLBACK Application::WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_CLOSE:
		{
			DestroyWindow(hWnd);
		}
		break;

	case WM_DESTROY:
		{
			PostQuitMessage(0);
			App.Running = false;
			// TerminateProcess(GetCurrentProcess(), 0);
		}
		break;

	case WM_MOUSEMOVE:
		{
			if (hWnd == App.hWnd)
			{
				GetClientRect(App.hWnd, &App.ClientRect);
				App.WndTopLeft.x = App.ClientRect.left;
				App.WndTopLeft.y = App.ClientRect.top;
				App.WndBottomRight.x = App.ClientRect.right;
				App.WndBottomRight.y = App.ClientRect.bottom;
				ClientToScreen(App.hWnd, &App.WndTopLeft);
				ClientToScreen(App.hWnd, &App.WndBottomRight);
				GetCursorPos(&App.CursorPos);
				//memcpy(&App.ClientCursorPos, &App.CursorPos, sizeof(POINT));
				App.ClientCursorPos = App.CursorPos;
				ScreenToClient(App.hWnd, &App.ClientCursorPos);

				if (App.CursorPos.x < App.WndTopLeft.x || App.CursorPos.x > App.WndBottomRight.x || App.CursorPos.y < App.WndTopLeft.y || App.CursorPos.y > App.WndBottomRight.y)
				{
					if (GetCursor() != App.DefaultCursor)
					{
						//Out of bounds, change to regular cursor
						SetCursor(App.DefaultCursor);
					}

					PostMessage(hWnd, WM_LBUTTONUP, 0, 0);
				}
				else
				{
					if (GetCursor() != App.InGameCursor)
					{
						//In bounds, change to in-game cursor
						SetCursor(App.InGameCursor);
					}
				}
			}
		}
		break;

	// When the activation changes to a different top-level window in the same application.
	case WM_ACTIVATE:
		{
			if (hWnd == App.hWnd)
			{
				if (LOWORD(wParam) == WA_INACTIVE)
				{
					App.WndActive = false;
				}
				else if (LOWORD(wParam) == WA_ACTIVE)
				{
					App.WndActive = true;
				}

				if (DI.KeyboardDevice != NULL)
				{
					DI.KeyboardDevice->Acquire();
				}

				if (DI.MouseDevice != NULL)
				{
					DI.MouseDevice->Acquire();
				}
			}
		}
		break;

	// Sent when a window belonging to a different application than the active window is about to be activated.
	// (WPARAM) Indicates whether the window is being activated or deactivated. This parameter is
	// TRUE if the window is being activated; it is FALSE if the window is being deactivated.
	case WM_ACTIVATEAPP:
		{
			switch (LOWORD(wParam))
			{
			case TRUE:
				{
					App.WndActive = true;

					if (DI.KeyboardDevice != NULL)
					{
						DI.KeyboardDevice->Acquire();
					}

					if (DI.MouseDevice != NULL)
					{
						DI.MouseDevice->Acquire();
					}
				}
				break;

			case FALSE:
				{
					App.WndActive = false;
				}
				break;
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			GetDesktop()->CX_WM_LBUTTONDOWN();
		}
		break;

	case WM_LBUTTONUP:
		{
			GetDesktop()->CX_WM_LBUTTONUP();
		}
		break;

	default:
		{
			return DefWindowProc(hWnd, Msg, wParam, lParam);
		}
	}

	return DefWindowProc(hWnd, Msg, wParam, lParam);
}