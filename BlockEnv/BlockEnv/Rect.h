#pragma once

#include <Windows.h>

struct Rect
{
	long Left;
	long Top;
	long Right;
	long Bottom;
	Rect(void)
	{
		Left = 0;
		Top = 0;
		Right = 0;
		Bottom = 0;
	}
	Rect(Rect &rc)
	{
		Left = rc.Left;
		Top = rc.Top;
		Right = rc.Right;
		Bottom = rc.Bottom;
	}
	Rect(long left, long top, long right, long bottom)
	{
		Left = left;
		Top = top;
		Right = right;
		Bottom = bottom;
	}
	~Rect(void)
	{
	}
	long Width()
	{
		return Right - Left;
	}
	void setWidth(long NewWidth)
	{
		Right = Left + NewWidth;
	}
	long Height()
	{
		return Bottom - Top;
	}
	void setHeight(long NewHeight)
	{
		Bottom = Top + NewHeight;
	}
	long CenterX()
	{
		return Left + (Width() / 2);
	}
	long CenterY()
	{
		return Top + (Height() / 2);
	}
	long HalfWidth()
	{
		return Width() / 2;
	}
	long HalfHeight()
	{
		return Height() / 2;
	}
	POINT Center()
	{
		POINT ret = { CenterX() , CenterY() };
		return ret;
	}
};