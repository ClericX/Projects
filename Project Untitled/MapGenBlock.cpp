#include "MapGenBlock.h"

MapGenBlock::MapGenBlock(void)
{
	setVisited(false);
	IsEndpoint = false;
	for (unsigned int i = 0; i < WallPos::Count; i++)
	{
		WallRemoved[i] = false;
	}
}

MapGenBlock::~MapGenBlock(void)
{
	setVisited(true);
}

void MapGenBlock::setVisited(bool Status)
{
	if (Status)
	{
		Visited = true;
		NumUnvisitedBlocks--;
	}
	else
	{
		Visited = false;
		NumUnvisitedBlocks++;
	}
}

bool MapGenBlock::hasWall(WallPos wp)
{
	return !WallRemoved[wp];
}

int MapGenBlock::NumUnvisitedBlocks = 0;