#pragma once

#include <Windows.h>

class Capture
{
public:
	Capture(void)
	{
		hdcScreen = 0;
		hdcMemDC = 0;
		hbmScreen = 0;
		dwBmpSize = 0;
		hDIB = 0;
		lpbitmap = 0;
	}
	~Capture(void)
	{
		Release();
	}

	HDC hdcScreen;
	HDC hdcMemDC;
	HBITMAP hbmScreen;
	BITMAP bmpScreen;
	DWORD dwBmpSize;
	HANDLE hDIB;
	char *lpbitmap;

	void Acquire()
	{
		hdcScreen = GetDC(0);
		hdcMemDC = CreateCompatibleDC(hdcScreen);

		if (!hdcMemDC)
		{
			MessageBoxA(0, "Failed to create compatible device context.", "fail", MB_OK);
			return;
		}

		int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
		int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

		hbmScreen = CreateCompatibleBitmap(hdcScreen, ScreenWidth, ScreenHeight);

		if (!hbmScreen)
		{
			MessageBoxA(0, "Failed to create a compatible bitmap.", "fail", MB_OK);
		}

		SelectObject(hdcMemDC,hbmScreen);

		if (!BitBlt(hdcMemDC, 0,0, ScreenWidth, ScreenHeight, hdcScreen, 0,0, SRCCOPY|CAPTUREBLT))
		{
			MessageBoxA(0, "BitBlt failed.", "fail", MB_OK);
		}

		GetObjectA(hbmScreen,sizeof(BITMAP),&bmpScreen);

		BITMAPINFOHEADER   bi;

		bi.biSize = sizeof(BITMAPINFOHEADER);
		bi.biWidth = bmpScreen.bmWidth;
		bi.biHeight = bmpScreen.bmHeight;
		bi.biPlanes = 1;
		bi.biBitCount = 32;
		bi.biCompression = BI_RGB;
		bi.biSizeImage = 0;
		bi.biXPelsPerMeter = 0;
		bi.biYPelsPerMeter = 0;
		bi.biClrUsed = 0;
		bi.biClrImportant = 0;

		dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;
		hDIB = GlobalAlloc(GHND,dwBmpSize);
		lpbitmap = (char *)GlobalLock(hDIB);

		// Gets the "bits" from the bitmap and copies them into a buffer 
		// which is pointed to by lpbitmap.
		GetDIBits(hdcScreen, hbmScreen, 0, (UINT)bmpScreen.bmHeight, lpbitmap, (BITMAPINFO *)&bi, DIB_RGB_COLORS);
	}

	void Release()
	{
		if (hDIB)
		{
			GlobalUnlock(hDIB);
			GlobalFree(hDIB);
		}

		if (hbmScreen)
		{
			DeleteObject(hbmScreen);
		}

		if (hdcMemDC)
		{
			DeleteObject(hdcMemDC);
		}

		if (hdcScreen)
		{
			ReleaseDC(NULL, hdcScreen);
		}
	}

	Color Get(unsigned int X, unsigned int Y)
	{
		DWORD Offset = dwBmpSize - (bmpScreen.bmWidth * 4 * Y) - ( (bmpScreen.bmWidth * 4) - (X * 4) );
		Color ret(lpbitmap[Offset+2], lpbitmap[Offset+1], lpbitmap[Offset], lpbitmap[Offset+3]);
		return ret;
	}
};