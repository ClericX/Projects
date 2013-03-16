#pragma once

#include "Includes.h"
#include "MapObject.h"
#include "Platforms.h"

class CMobs : public CMapObject
{
public:
	CMobs(void);
	~CMobs(void);

	string MobName;
	int Instance;
	bool Facing;
	int CanFallOffPlatforms;
	CPlatforms* OnPlatform;
	unsigned int MobID;
	MOBFRAMESTRUCT* MobFrames;

	bool Attacked(CMapObject *Obj);
	bool ChangeHP(unsigned long ByWhat, bool Decrease, bool ThisIsChecked);
	bool ChangeMP(unsigned long ByWhat, bool Decrease, bool ThisIsChecked);
	void DeathCheck();
	void DrawStats();
	void Respawn();

	unsigned long HP;
	unsigned long MP;
	unsigned long EXPGiven;
	unsigned long MaxHP;
	unsigned long MaxMP;
	long Accuracy;

	struct TARGETMOVEMENT
	{
		bool Done;

		struct PORTIONOFMOVEMENT
		{
			int Value;
			bool Done;
			int Progress;
			int Completion;
		} Direction, Movement, Jump, Wait;

	} TargetMovement;

	GRAVITYVARS GravityVars;
};