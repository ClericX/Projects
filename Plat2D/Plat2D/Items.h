#pragma once

#include "Includes.h"
#include "MapObject.h"

class CItems : public CMapObject
{
public:
	CItems(unsigned int ItmID, unsigned int NumOf, Rect DroppingObject);
	CItems(void);
	~CItems(void);

	unsigned int Amount;
	unsigned int ItemID;
};