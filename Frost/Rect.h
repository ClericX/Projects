#pragma once

#include "DLL.h"
#include "Point.h"

namespace Frost
{
	class FROSTAPI Rect
	{
	public:
		Rect(void);
		Rect(Rect &rc);
		Rect(long left, long top, long right, long bottom);
		~Rect(void);

		long Left;
		long Top;
		long Right;
		long Bottom;
		long Width;
		long Height;

		void SetBounds(long _left, long _top, long _right, long _bottom);
		long GetWidth();
		void SetWidth(long NewWidth);
		long GetHeight();
		void SetHeight(long NewHeight);
		long CenterX();
		long CenterY();
		long HalfWidth();
		long HalfHeight();
		Point Center();
	};
};