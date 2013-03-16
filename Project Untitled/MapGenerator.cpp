#include "MapGenerator.h"

#include <time.h>

// rand shit -- TEMP

bool randInit = false;

int GetRand(int low, int high)
{
	if (!randInit)
	{
		srand((unsigned int)time(0));
	}

	return (rand() % ( (high + 1) - low ) ) + low;
}

// rand shit -- TEMP

MapGenerator::MapGenerator(void)
{
	MapWidth = 0;
	MapHeight = 0;
	map = 0;
}

MapGenerator::MapGenerator(unsigned int width, unsigned int height, unsigned int startX, unsigned int startY, bool autoGen)
{
	map = 0;
	setSize(width, height);
	setStartBlock(startX, startY);

	if (autoGen)
	{
		Generate();
	}
}

MapGenerator::~MapGenerator(void)
{
	if (map != 0)
	{
		cleanMap();
	}
}

void MapGenerator::cleanMap()
{
	for (unsigned int i = 0; i < MapWidth; i++)
	{
		delete[] map[i];
	}
	delete map;
	map = 0;
	MapWidth = 0;
	MapHeight = 0;
}

void MapGenerator::setSize(unsigned int Width, unsigned int Height)
{
	if (map != 0)
	{
		cleanMap();
	}

	MapWidth = Width;
	MapHeight = Height;

	// Alloc a whole 2D array.
	map = new MapGenBlock *[MapWidth];
	for (unsigned int i = 0; i < MapWidth; i++)
	{
		map[i] = new MapGenBlock[MapHeight];
		for (unsigned int ix = 0; ix < MapHeight; ix++)
		{
			map[i][ix].Pos.Set(i, ix);
		}
	}
}

void MapGenerator::setStartBlock(unsigned int x, unsigned int y)
{
	// Enables custom starting pos which is also separate from current pos.
	StartBlockPos.Set(x, y);
}

void MapGenerator::SetCurrentBlock(int X, int Y)
{
	CurBlockPos.Set(X, Y);
	map[CurBlockPos.X][CurBlockPos.Y].setVisited(true);
}

bool MapGenerator::IsVisitableBlock(int X, int Y)
{
	if (X < 0 || X > (MapWidth - 1) || Y < 0 || Y > (MapHeight - 1))
	{
		return false;
	}
	else
	{
		return !map[X][Y].Visited;
	}
}

void MapGenerator::DepthFirstSearch()
{
	std::vector<Point> VisitableNeighbours;
	
	if (IsVisitableBlock(CurBlockPos.X + 1, CurBlockPos.Y))
	{
		VisitableNeighbours.push_back(Point(CurBlockPos.X + 1, CurBlockPos.Y));
	}
	if (IsVisitableBlock(CurBlockPos.X - 1, CurBlockPos.Y))
	{
		VisitableNeighbours.push_back(Point(CurBlockPos.X - 1, CurBlockPos.Y));
	}
	if (IsVisitableBlock(CurBlockPos.X, CurBlockPos.Y + 1))
	{
		VisitableNeighbours.push_back(Point(CurBlockPos.X, CurBlockPos.Y + 1));
	}
	if (IsVisitableBlock(CurBlockPos.X, CurBlockPos.Y - 1))
	{
		VisitableNeighbours.push_back(Point(CurBlockPos.X, CurBlockPos.Y - 1));
	}
	if (IsVisitableBlock(CurBlockPos.X - 1, CurBlockPos.Y - 1))
	{
		VisitableNeighbours.push_back(Point(CurBlockPos.X - 1, CurBlockPos.Y - 1));
	}
	if (IsVisitableBlock(CurBlockPos.X + 1, CurBlockPos.Y - 1))
	{
		VisitableNeighbours.push_back(Point(CurBlockPos.X + 1, CurBlockPos.Y - 1));
	}
	if (IsVisitableBlock(CurBlockPos.X - 1, CurBlockPos.Y + 1))
	{
		VisitableNeighbours.push_back(Point(CurBlockPos.X - 1, CurBlockPos.Y + 1));
	}
	if (IsVisitableBlock(CurBlockPos.X + 1, CurBlockPos.Y + 1))
	{
		VisitableNeighbours.push_back(Point(CurBlockPos.X + 1, CurBlockPos.Y + 1));
	}

	if (VisitableNeighbours.size() > 0)
	{
		unsigned int RandomNeighbour = GetRand(0, VisitableNeighbours.size() - 1);
		Point NewBlock = VisitableNeighbours[RandomNeighbour];

		// ADD WALL REMOVAL
		if (NewBlock == Point(CurBlockPos.X + 1, CurBlockPos.Y))
		{
			map[CurBlockPos.X][CurBlockPos.Y].WallRemoved[WallPos::Right] = true;
			map[CurBlockPos.X + 1][CurBlockPos.Y].WallRemoved[WallPos::Left] = true;
		}
		else if (NewBlock == Point(CurBlockPos.X - 1, CurBlockPos.Y))
		{
			map[CurBlockPos.X][CurBlockPos.Y].WallRemoved[WallPos::Left] = true;
			map[CurBlockPos.X - 1][CurBlockPos.Y].WallRemoved[WallPos::Right] = true;
		}
		else if (NewBlock == Point(CurBlockPos.X, CurBlockPos.Y + 1))
		{
			map[CurBlockPos.X][CurBlockPos.Y].WallRemoved[WallPos::Down] = true;
			map[CurBlockPos.X][CurBlockPos.Y + 1].WallRemoved[WallPos::Up] = true;
		}
		else if (NewBlock == Point(CurBlockPos.X, CurBlockPos.Y - 1))
		{
			map[CurBlockPos.X][CurBlockPos.Y].WallRemoved[WallPos::Up] = true;
			map[CurBlockPos.X][CurBlockPos.Y - 1].WallRemoved[WallPos::Down] = true;
		}
		else if (NewBlock == Point(CurBlockPos.X - 1, CurBlockPos.Y - 1))
		{
			map[CurBlockPos.X][CurBlockPos.Y].WallRemoved[WallPos::UpLeft] = true;
			map[CurBlockPos.X - 1][CurBlockPos.Y - 1].WallRemoved[WallPos::DownRight] = true;
		}
		else if (NewBlock == Point(CurBlockPos.X + 1, CurBlockPos.Y - 1))
		{
			map[CurBlockPos.X][CurBlockPos.Y].WallRemoved[WallPos::UpRight] = true;
			map[CurBlockPos.X + 1][CurBlockPos.Y - 1].WallRemoved[WallPos::DownLeft] = true;
		}
		else if (NewBlock == Point(CurBlockPos.X - 1, CurBlockPos.Y + 1))
		{
			map[CurBlockPos.X][CurBlockPos.Y].WallRemoved[WallPos::DownLeft] = true;
			map[CurBlockPos.X - 1][CurBlockPos.Y + 1].WallRemoved[WallPos::UpRight] = true;
		}
		else if (NewBlock == Point(CurBlockPos.X + 1, CurBlockPos.Y + 1))
		{
			map[CurBlockPos.X][CurBlockPos.Y].WallRemoved[WallPos::DownRight] = true;
			map[CurBlockPos.X + 1][CurBlockPos.Y + 1].WallRemoved[WallPos::UpLeft] = true;
		}

		LastBlockPos.Set(CurBlockPos);
		CurBlockPos.Set(NewBlock);
		map[CurBlockPos.X][CurBlockPos.Y].setVisited(true);
		BlockStack.push_back(CurBlockPos);
	}
	else
	{
		BlockStack.pop_back();
		Point NewCurBlock = BlockStack[BlockStack.size()-1];

		if (NewCurBlock == LastBlockPos)
		{
			map[CurBlockPos.X][CurBlockPos.Y].IsEndpoint = true;
		}

		CurBlockPos.Set(NewCurBlock);
	}
}

void MapGenerator::Generate()
{
	// Set the current block to the desired starting block before starting.
	CurBlockPos.Set(StartBlockPos);

	// Depth-First Search
	// Push the starting point onto the stack.
	map[CurBlockPos.X][CurBlockPos.Y].setVisited(true);
	BlockStack.push_back(CurBlockPos);
	// Perform the algorithm.
	while (MapGenBlock::NumUnvisitedBlocks > 0)
	{
		DepthFirstSearch();
	}
	// Once more to identify the final endpoint.
	DepthFirstSearch();
}

bool MapGenerator::hasWallAt(unsigned int x, unsigned int y, WallPos p)
{
	return !map[x][y].hasWall(p);
}

unsigned int MapGenerator::getWidth()
{
	return MapWidth;
}

unsigned int MapGenerator::getHeight()
{
	return MapHeight;
}