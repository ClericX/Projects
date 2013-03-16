#pragma once

#include "Includes.h"
#include "MapObject.h"

class CPlatforms : public CMapObject
{
public:
	CPlatforms(void);
	~CPlatforms(void);

	unsigned int Instance;
	unsigned int MapID;
	unsigned int PlatformID;
};