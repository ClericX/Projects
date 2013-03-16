#pragma once

#include <Windows.h>

class Color
{
public:
	Color(void)
	{
	}
	Color(BYTE r, BYTE g, BYTE b, BYTE a)
	{
		R = r;
		G = g;
		B = b;
		A = a;
	}
	~Color(void)
	{
	}
	BYTE R;
	BYTE G;
	BYTE B;
	BYTE A;

	Color &operator - (Color &other)
	{
		#pragma warning( disable : 4172 )
		return Color(this->R - other.R, this->G - other.G, this->B - other.B, this->A - other.A);
		#pragma warning( default : 4172 )
	}
};

namespace Colors
{
	static Color White			= Color(0xFF, 0xFF, 0xFF, 0xFF);
	static Color Black			= Color(0, 0, 0, 0xFF);
	static Color Red			= Color(0xFF, 0x00, 0x00, 0xFF);
	static Color Green			= Color(0x00, 0xFF, 0x00, 0xFF);
	static Color Blue			= Color(0x00, 0x00, 0xFF, 0xFF);
	static Color DarkBlue		= Blue - Color(0, 0, 100, 0);
	static Color DarkRed		= Red - Color(100, 0, 0, 0);
	static Color DarkGreen		= Green - Color(0, 100, 0, 0);
	static Color LightGreen		= Green - Color(-50, 0, -50, 0);
};