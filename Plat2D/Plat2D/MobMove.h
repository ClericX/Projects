#pragma once

#include "Includes.h"
#include "Mobs.h"

//////////////////////////////////////////////////////////
//Handles functionality of automatically moving monsters//
//////////////////////////////////////////////////////////

class CMobMove
{
public:
	CMobMove(void);
	~CMobMove(void);

	void GenerateMovement(CMobs *Mob);
	void DoDirection(CMobs *Mob);
	void DoMovement(CMobs *Mob);
	void DoJump(CMobs *Mob);
	void DoWait(CMobs *Mob);

	void ApplyMovements(CMobs *Mob);
};