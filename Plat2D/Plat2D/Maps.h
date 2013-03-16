#pragma once

#include "Includes.h"

class CMaps
{
public:
	CMaps(void);
	~CMaps(void);

	unsigned int MapID;
	string MapName;
	unsigned int NumberOfPlatforms;
	unsigned int NumberOfBackgrounds;
	unsigned int NumberOfMobs;
	unsigned int NumberOfPortals;
	Rect Bounds;

	vector <MAPOBJECT*> Backgrounds;
	vector <MAPOBJECT*> Platforms;
	vector <MAPOBJECT*> Mobs;
	vector <MAPOBJECT*> Portals;
};