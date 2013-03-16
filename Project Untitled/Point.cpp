#include "Point.h"

Point::Point(void)
{
	Set(0, 0);
}

Point::Point(int _X, int _Y)
{
	Set(_X, _Y);
}

void Point::Set(int _X, int _Y)
{
	X = _X;
	Y = _Y;
}

void Point::Set(Point &p)
{
	Set(p.X, p.Y);
}

bool Point::operator == (Point &p)
{
	return this->X == p.X && this->Y == p.Y;
}

bool Point::operator != (Point &p)
{
	return !((*this) == p);
}