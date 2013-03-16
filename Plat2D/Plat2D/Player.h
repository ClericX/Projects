#pragma once

#include "Includes.h"
#include "MapObject.h"
#include "Platforms.h"

class CPlayer : public CMapObject
{
public:
	CPlayer(void);
	~CPlayer(void);

	char Name[50];
	CPlatforms* OnPlatform;

	unsigned int ID;
	unsigned int CurrentMapID;

	unsigned long HP;
	unsigned long MP;
	unsigned long EXP;

	unsigned long MaxHP;
	unsigned long MaxMP;
	unsigned long EXPNeeded;

	unsigned int Level;
	//unsigned long ExpToLvl[] = {10, 20, 30, 40, 50};

	long LastX;
	long LastY;

	bool ChangeHP(unsigned long ByWhat, bool Decrease, bool ThisIsChecked);
	bool ChangeMP(unsigned long ByWhat, bool Decrease, bool ThisIsChecked);
	bool ChangeEXP(unsigned long ByWhat, bool Decrease);
	void DeathCheck();

	unsigned long Money;

	bool Facing;
	bool Attacking;
	bool Jumping;
	bool CanFallOffPlatforms;

	GRAVITYVARS GravityVars;

	bool Haste;
};