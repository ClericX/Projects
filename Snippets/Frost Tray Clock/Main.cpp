// Use SetWindowsHookEx for checking whne mouse is clicked and doing mosue over checks? ghetto as fuck son.

#include <Windows.h>
#include <stdio.h>

HWND TrayWindow = 0;
HWND TrayNotifyWindow = 0;
HWND TrayClockWindow = 0;
WNDPROC OldWndProc = 0;
HINSTANCE hInst = 0;
HBITMAP testImg = 0;

void SizeWindow(HWND hwnd, int width, int height)
{
	SetWindowPos(hwnd, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void logWndMsg(HWND hWnd, UINT Msg)
{
	char buf[100];
	char itoaBuf[10];
	char wndStr[20];
	if (hWnd == TrayWindow)
	{
		strcpy(wndStr, "TrayWnd");
	}
	else if (hWnd == TrayNotifyWindow)
	{
		strcpy(wndStr, "TrayNotifyWnd");
	}
	else if (hWnd == TrayClockWindow)
	{
		strcpy(wndStr, "TrayClockWnd");
	}
	else
	{
		strcpy(wndStr, "UNKNOWN");
	}
	memset(buf, 0, sizeof(buf));
	strcat(buf, "[");
	strcat(buf, itoa(GetTickCount(), itoaBuf, 10));
	strcat(buf, "] ");
	strcat(buf, "hWnd: ");
	strcat(buf, itoa((int)hWnd, itoaBuf, 16));
	strcat(buf, " (");
	strcat(buf, wndStr);
	strcat(buf, ")");
	strcat(buf, " | Msg: ");
	strcat(buf, itoa(Msg, itoaBuf, 10));
	strcat(buf, "\n");

	FILE *f = fopen("C:/derp.txt", "a+");
	fwrite(buf, strlen(buf), 1, f);
	fclose(f);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	logWndMsg(hWnd, Msg);

	switch (Msg)
	{
		// General window painting.
	case WM_PAINT:
		{
			RECT updRect;

			if (!GetUpdateRect(hWnd, &updRect, false))
			{
				break;
			}

			PAINTSTRUCT 	ps;
			HDC 			hdc;
			BITMAP 			bitmap;
			HDC 			hdcMem;
			HGDIOBJ 		oldBitmap;

			hdc = BeginPaint(hWnd, &ps);

			hdcMem = CreateCompatibleDC(hdc);
			oldBitmap = SelectObject(hdcMem, testImg);

			GetObject(testImg, sizeof(bitmap), &bitmap);
			BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

			SelectObject(hdcMem, oldBitmap);
			DeleteDC(hdcMem);

			EndPaint(hWnd, &ps);
		}
		break;
		/*
		// Erasing the window background.
	case WM_ERASEBKGND:
		//
		break;
		*/
		// Drawing the window frame.
	case WM_NCPAINT:
		//
		break;

		// Timer calls, these happen every minute and redraw the clock in a way that bypasses the other 3 messages.
	case WM_TIMER:
		//
		break;

		// Mouse button down msg for clock for some reason....
	case 1128:
		{
			RECT rc;
			GetWindowRect(hWnd, &rc);
			int width = rc.right - rc.left;
			int height = rc.bottom - rc.top;
			SizeWindow(hWnd, width + 10, height);
			MessageBoxA(0, "derp", "Derp", MB_OK);
		}
		break;

		// Mouse button up msg for clock
	case 1127:
		{
			//
		}
		break;

		// Mouse right button down.
	case WM_RBUTTONDOWN:
		//
		break;

		// Mouse right button up.
	case WM_RBUTTONUP:
		//
		break;

	case WM_NCHITTEST:
		{
			short xPos = LOWORD(lParam); 
			short yPos = HIWORD(lParam);

			char buf[200];
			char ibuf[10];
			memset(buf, 0, 200);
			strcat(buf, "WM_NCHITTEST: ");
			strcat(buf, itoa(xPos, ibuf, 10));
			strcat(buf, ", ");
			strcat(buf, itoa(yPos, ibuf, 10));
			strcat(buf, "\r\n");
			FILE *f = fopen("C:/debug.txt", "a+");
			fwrite(buf, strlen(buf), 1, f);
			fclose(f);

			return HTCLIENT;
		}
		break;

		// Let every other message pass so the clock doesn't die.
	default:
		return CallWindowProc(OldWndProc, hWnd, Msg, wParam, lParam);
	}
	return 0;
}

void getClockHandle()
{
	TrayWindow = FindWindowEx(0, 0, "Shell_TrayWnd", 0);
	TrayNotifyWindow = FindWindowEx(TrayWindow, 0, "TrayNotifyWnd", 0);
	TrayClockWindow = FindWindowEx(TrayNotifyWindow, 0, "TrayClockWClass", 0);

	if (TrayClockWindow == 0)
	{
		MessageBoxA(0, "Failed to acquire a handle to the clock window." ,"Error", MB_OK);
	}
}

void hookClockWndProc()
{
	OldWndProc = (WNDPROC)SetWindowLongPtr(TrayClockWindow, GWLP_WNDPROC, (long long)WndProc);
}

void unhookClockWndProc()
{
	SetWindowLongPtr(TrayClockWindow, GWLP_WNDPROC, (long long)OldWndProc);
}

HBITMAP loadBMP(char *path)
{
	return (HBITMAP)LoadImageA(hInst, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	// Perform actions based on the reason for calling.
	switch( fdwReason ) 
	{
		// Initialize once for each new process.
		// Return FALSE to fail DLL load.
		case DLL_PROCESS_ATTACH:
			{
				DisableThreadLibraryCalls(hinstDLL);

				getClockHandle();
				hookClockWndProc();
				testImg = loadBMP("C:/test.bmp");

				//////////////

				RECT wndRect;

				GetWindowRect(TrayClockWindow, &wndRect);

				for (int w = 0; w < wndRect.right - wndRect.left; w++)
				{
					for (int h = 0; h < wndRect.bottom - wndRect.top; h++)
					{
						SetPixel(GetDC(TrayClockWindow), w, h, 0xFFFFFF00);
					}
				}
			}
			break;

		case DLL_THREAD_ATTACH:
		 // Do thread-specific initialization.
			break;

		case DLL_THREAD_DETACH:
		 // Do thread-specific cleanup.
			break;

		case DLL_PROCESS_DETACH:
		 // Perform any necessary cleanup.

			unhookClockWndProc();
			DeleteObject(testImg);

			break;
	}
	return TRUE;  // Successful DLL_PROCESS_ATTACH.
}