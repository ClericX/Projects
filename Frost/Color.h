#pragma once

#include "DLL.h"

class FROSTAPI Color
{
public:
	Color(void)
	{
		R = 0;
		G = 0;
		B = 0;
		A = 0;
	}
	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		R = r;
		G = g;
		B = b;
		A = a;
	}
	~Color(void)
	{
	}
	unsigned char R;
	unsigned char G;
	unsigned char B;
	unsigned char A;

	Color operator - (Color &other)
	{
		//#pragma warning( disable : 4172 )
		return Color(this->R - other.R, this->G - other.G, this->B - other.B, this->A - other.A);
		//#pragma warning( default : 4172 )
	}

	bool operator == (Color &other)
	{
		return (R == other.R) && (G == other.G) && (B == other.B) && (A == other.A);
	}

	bool operator != (Color &other)
	{
		return !((*this) == other);
	}

	static Color FromIntRGBA(unsigned long rgba)
	{
		Color ret;
		unsigned char *ptr = (unsigned char *)&rgba;
		ret.R = *ptr;
		ret.G = *(ptr+1);
		ret.B = *(ptr+2);
		ret.A = *(ptr+3);
		return ret;
	}
};

namespace Colors
{
	static Color Nothing		= Color();
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