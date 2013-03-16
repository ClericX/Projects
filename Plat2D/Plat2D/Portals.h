#pragma once
#include "MapObject.h"

class CPortals :
	public CMapObject
{
public:
	CPortals(void);
	~CPortals(void);

	unsigned int PortalID;
	unsigned int TargetMapID;
};