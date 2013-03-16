#pragma once

class Point
{
public:
	long X;
	long Y;
	Point(void)
	{
		X = 0;
		Y = 0;
	}
	Point(long _x, long _y)
	{
		X = _x;
		Y = _y;
	}
	~Point(void)
	{
	}
};