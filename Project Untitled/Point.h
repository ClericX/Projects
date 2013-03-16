#pragma once

class Point
{
public:
	Point(void);
	Point(int _X, int _Y);

	int X;
	int Y;

	void Set(int _X, int _Y);
	void Set(Point &p);
	bool operator == (Point &p);
	bool operator != (Point &p);
};