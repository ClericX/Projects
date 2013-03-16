#pragma once

#include "MapGenBlock.h"

#include <vector>

class MapGenerator
{
public:
	MapGenerator(void);
	MapGenerator(unsigned int width, unsigned int height, unsigned int startX = 0, unsigned int startY = 0, bool autoGen = true);
	~MapGenerator(void);

	unsigned int MapWidth;
	unsigned int MapHeight;
	MapGenBlock **map;
	Point StartBlockPos;
	Point MapExitPos;
	Point CurBlockPos;
	Point LastBlockPos;
	std::vector<Point> BlockStack;

	void cleanMap();
	void setSize(unsigned int Width, unsigned int Height);
	void setStartBlock(unsigned int x, unsigned int y);
	void SetCurrentBlock(int X, int Y);
	bool IsVisitableBlock(int X, int Y);
	void DepthFirstSearch();
	void Generate();
	bool hasWallAt(unsigned int x, unsigned int y, WallPos p);
	unsigned int getWidth();
	unsigned int getHeight();
};