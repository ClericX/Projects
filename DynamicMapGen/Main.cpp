/*
TODO

Add removal of walls randomly to create less "pathing" and more flow.
Add "wall sets" where a just a bunch of static pathing can be placed all at once during the pathing process.
*/

#include <Windows.h>
#include <vector>
#include <time.h>

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

int GetRand(int low, int high)
{
	return (rand() % ( (high + 1) - low ) ) + low;
}

class MapGenBlock
{
public:
	MapGenBlock(void)
	{
		setVisited(false);
		IsEndpoint = false;
		for (unsigned int i = 0; i < WallPos::Count; i++)
		{
			WallRemoved[i] = false;
		}
	}
	~MapGenBlock(void)
	{
		setVisited(true);
	}

	Point Pos;
	bool Visited;
	bool IsEndpoint;
	bool WallRemoved[WallPos::Count];

	void setVisited(bool Status)
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

	void setPos(int X, int Y)
	{
		Pos.Set(X, Y);
	}

	Point getPos()
	{
		return Pos;
	}

	static int NumUnvisitedBlocks;
};

int MapGenBlock::NumUnvisitedBlocks = 0;


class MapGenerator
{
public:
	MapGenerator(void)
	{
		MapWidth = 0;
		MapHeight = 0;
		map = 0;
	}
	~MapGenerator(void)
	{
		if (map != 0)
		{
			cleanMap();
		}
	}

	unsigned int MapWidth;
	unsigned int MapHeight;
	MapGenBlock **map;
	Point StartBlockPos;
	Point MapExitPos;
	Point CurBlockPos;
	Point LastBlockPos;
	std::vector<Point> BlockStack;

	void cleanMap()
	{
		//
	}

	void setSize(unsigned int Width, unsigned int Height)
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
				map[i][ix].setPos(i, ix);
			}
		}
	}

	void setStartBlock(unsigned int x, unsigned int y)
	{
		// Enables custom starting pos which is also separate from current pos.
		StartBlockPos.Set(x, y);
	}

	void SetCurrentBlock(int X, int Y)
	{
		CurBlockPos.Set(X, Y);
		map[CurBlockPos.X][CurBlockPos.Y].setVisited(true);
	}

	bool IsVisitableBlock(int X, int Y)
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

	void DepthFirstSearch()
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

	void Generate()
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
};

int main()
{
	// Needed but temp; in Frost.
	srand((unsigned int)time(0));

	MapGenerator mapGen;

	mapGen.setSize(30, 30);
	mapGen.setStartBlock(10, 10);
	mapGen.Generate();

	FILE *f = 0;
	fopen_s(&f, "maze.html", "w+");

	fwrite("<html><head><style>img{ width: 5px; height: 5px; }</style></head><body>", 71, 1, f);

	for (int h = 0; h < MapHeight; h++)
	{
		for (int bh = 0; bh < 3; bh++)
		{
			for (int w = 0; w < MapWidth; w++)
			{
				if (bh == 0)
				{
					if (!map[w][h].WallRemoved[WallPos::UpLeft])
					{
						fwrite("<img src='img/tlc.png' />", 25, 1, f);
					}
					else
					{
						fwrite("<img src='img/ooo.png' />", 25, 1, f);
					}

					if (!map[w][h].WallRemoved[WallPos::Up])
					{
						fwrite("<img src='img/upc.png' />", 25, 1, f);
					}
					else
					{
						fwrite("<img src='img/ooo.png' />", 25, 1, f);
					}

					if (!map[w][h].WallRemoved[WallPos::UpRight])
					{
						fwrite("<img src='img/trc.png' />", 25, 1, f);
					}
					else
					{
						fwrite("<img src='img/ooo.png' />", 25, 1, f);
					}
				}
				else if (bh == 1)
				{
					if (!map[w][h].WallRemoved[WallPos::Left])
					{
						fwrite("<img src='img/lll.png' />", 25, 1, f);
					}
					else
					{
						fwrite("<img src='img/ooo.png' />", 25, 1, f);
					}

					if (map[w][h].IsEndpoint)
					{
						fwrite("<img src='img/epp.png' />", 25, 1, f);
					}
					else if (map[w][h].getPos() == StartBlockPos)
					{
						fwrite("<img src='img/sss.png' />", 25, 1, f);
					}
					/////////////////
					// ADD END POS //
					/////////////////
					else
					{
						fwrite("<img src='img/ccc.png' />", 25, 1, f);
					}

					if (!map[w][h].WallRemoved[WallPos::Right])
					{
						fwrite("<img src='img/rrr.png' />", 25, 1, f);
					}
					else
					{
						fwrite("<img src='img/ooo.png' />", 25, 1, f);
					}
				}
				else if (bh == 2)
				{
					if (!map[w][h].WallRemoved[WallPos::DownLeft])
					{
						fwrite("<img src='img/blc.png' />", 25, 1, f);
					}
					else
					{
						fwrite("<img src='img/ooo.png' />", 25, 1, f);
					}

					if (!map[w][h].WallRemoved[WallPos::Down])
					{
						fwrite("<img src='img/btc.png' />", 25, 1, f);
					}
					else
					{
						fwrite("<img src='img/ooo.png' />", 25, 1, f);
					}

					if (!map[w][h].WallRemoved[WallPos::DownRight])
					{
						fwrite("<img src='img/brc.png' />", 25, 1, f);
					}
					else
					{
						fwrite("<img src='img/ooo.png' />", 25, 1, f);
					}
				}
			}
			fwrite("<br />", 6, 1, f);
			fwrite("\r\n", 2, 1, f);
		}
	}

	fwrite("</body></html>", 14, 1, f);

	fclose(f);


	// need map gen cleanup
}