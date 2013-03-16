#pragma once
#include "MapObject.h"

class CBackgrounds :
	public CMapObject
{
public:
	CBackgrounds(void);
	~CBackgrounds(void);

	unsigned int Instance;
	unsigned int MapID;
	unsigned int BackgroundID;
};
