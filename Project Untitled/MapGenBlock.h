#pragma once

#include "Point.h"

typedef enum __WallPos_t
{
	Up,
	Down,
	Left,
	Right,
	UpLeft,
	UpRight,
	DownLeft,
	DownRight,
	Count
} WallPos;

class MapGenBlock
{
public:
	MapGenBlock(void);
	~MapGenBlock(void);

	Point Pos;
	bool Visited;
	bool IsEndpoint;
	bool WallRemoved[WallPos::Count];

	void setVisited(bool Status);
	bool hasWall(WallPos wp);

	static int NumUnvisitedBlocks;
};