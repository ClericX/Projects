#define FROST_EXPORT
#include "Rect.h"

namespace Frost
{
	Rect::Rect(void)
	{
		SetBounds(0, 0, 0, 0);
	}

	Rect::Rect(Rect &rc)
	{
		SetBounds(rc.Left, rc.Top, rc.Right, rc.Bottom);
	}

	Rect::Rect(long left, long top, long right, long bottom)
	{
		SetBounds(left, top, right, bottom);
	}

	Rect::~Rect(void)
	{
	}

	void Rect::SetBounds(long _left, long _top, long _right, long _bottom)
	{
		if (Left != _left || Right != _right)
		{
			Left = _left;
			Right = _right;
			Width = Right - Left;
		}

		if (Top != _top || Bottom != _bottom)
		{
			Top = _top;
			Bottom = _bottom;
			Height = Bottom - Top;
		}
	}

	long Rect::GetWidth()
	{
		return Width;
	}

	void Rect::SetWidth(long NewWidth)
	{
		SetBounds(Left, Top, Left + NewWidth, Bottom);
	}

	long Rect::GetHeight()
	{
		return Height;
	}

	void Rect::SetHeight(long NewHeight)
	{
		SetBounds(Left, Top, Right, Top + NewHeight);
	}

	long Rect::CenterX()
	{
		return Left + (GetWidth() / 2);
	}

	long Rect::CenterY()
	{
		return Top + (GetHeight() / 2);
	}

	long Rect::HalfWidth()
	{
		return GetWidth() / 2;
	}

	long Rect::HalfHeight()
	{
		return GetHeight() / 2;
	}

	Point Rect::Center()
	{
		Point ret(CenterX(), CenterY());
		return ret;
	}
};