#pragma once

#include "DLL.h"

class FROSTAPI Vector2f
{
public:
	float x;
	float y;

	Vector2f(void);
	Vector2f(float _x, float _y);
	void Set(float _x, float _y);
	~Vector2f(void);
};