#pragma once

class Color
{
public:
	Color(void)
	{
	}
	Color(unsigned char R, unsigned char G, unsigned char B, unsigned char A)
	{
		Set(R, G, B, A);
	}
	~Color(void)
	{
	}

	unsigned char R;
	unsigned char G;
	unsigned char B;
	unsigned char A;

	void Set(unsigned char R, unsigned char G, unsigned char B, unsigned char A)
	{
		this->R = R;
		this->G = G;
		this->B = B;
		this->A = A;
	}
	void Set(Color &clr)
	{
		Set(clr.R, clr.G, clr.B, clr.A);
	}
	unsigned long getRGBA()
	{
		return (R << 24) | (G << 16) | (B << 8) | (A);
	}
	bool operator == (Color &clr)
	{
		return (R == clr.R && G == clr.G && B == clr.B && A == clr.A);
	}
	bool operator != (Color &clr)
	{
		return !((*this) == clr);
	}
};