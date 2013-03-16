#pragma once

#include "Includes.h"
#include "Player.h"
#include "Mobs.h"
#include "Platforms.h"
#include "MapObject.h"

////////////////////////////////////////////////////
//Manages gravity-related variables and functions.//
////////////////////////////////////////////////////

class CGravity
{
public:
	CGravity(void);
	~CGravity(void);

	bool GravityEnabled;

	bool IsStoppingGravity(CMapObject *Ground, CMapObject *Object, int FallingVelocity);
	void IsBesidePlatform(CMapObject *Platform, CMapObject *Object);
	void PlayerGravityCheck(CPlayer &Player);
	void MobGravityCheck(CMobs *Mob);
};