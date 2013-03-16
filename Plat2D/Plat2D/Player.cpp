#include "Player.h"
#include "Main.h"

CPlayer::CPlayer(void)
{
	GravityVars.GravityPulling = true;
	GravityVars.FallingVelocity = 1;
	GravityVars.NumOfVelocityIncreases = 1;
	GravityVars.FallingTime = 0;
	GravityVars.FoundSomething = false;

	Haste = false;

	CurrentMapID = 0;

	Money = 0;
}

CPlayer::~CPlayer(void)
{
}

/*
This function handles gaining / losing HP.
It handles various different scenarios, and should be able to handle every possible one.
*/
bool CPlayer::ChangeHP(unsigned long ByWhat, bool Decrease, bool ThisIsChecked)
{
	// Handle if we have a negative number.
	if ( Decrease )
	{
		// If we have enough HP left to take away.
		if (HP >= ByWhat)
		{
			HP -= ByWhat;
		}

		// Else if we don't, just set the HP to 0.
		else if (HP < ByWhat)
		{
			// If this is not a check of some sort, kill the player (Like if this is monster damage).
			if (!ThisIsChecked)
			{
				HP = 0;
			}

			// If this is a check, just return false so we know the player doesn't have enough
			// HP to perform the operation they are trying to do (like use a skill).
			else if (ThisIsChecked)
			{
				return false;
			}
		}

		// If the player has 0 HP, they are dead. Set this value accordingly.
		if (HP == 0)
		{
			Dead = true;
		}
	}

	// Handle if we have a positive number.
	else if ( !Decrease )
	{
		// If we are trying to gain more hp than the max.
		if ( (HP + ByWhat) > MaxHP )
		{
			// Just heal to full.
			HP = MaxHP;
		}

		// Otherwise, we're just trying a regular heal, so just increase HP normally.
		else
		{
			HP += ByWhat;
		}

		// If the player has the dead flag set, we just healed HP meaning they can't have 0 anymore. Set this value accordingly.
		// This function might be called with a value of 0 for whatever reason, and since in this case we don't want a major exploit where
		// you could be alive with 0 HP, we will always check if they are dead AND have greater than 0 hp before setting them to alive.
		if (Dead == true && HP > 0)
		{
			Dead = false;
		}
	}

	DeathCheck();

	return true;
}

bool CPlayer::ChangeMP(unsigned long ByWhat, bool Decrease, bool ThisIsChecked)
{
	// Handle if we have a negative number.
	if ( Decrease )
	{
		// If we have enough MP left to take away.
		if (MP >= ByWhat)
		{
			MP -= ByWhat;
		}

		// Else if we don't, just set the MP to 0.
		else if (MP < ByWhat)
		{
			// If this is not a check of some sort, drain remaining MP (Like if this is monster damage to MP).
			if (!ThisIsChecked)
			{
				MP = 0;
			}

			// If this is a check, just return false so we know the player doesn't have enough
			// MP to perform the operation they are trying to do (like use a skill).
			else if (ThisIsChecked)
			{
				return false;
			}
		}
	}

	// Handle if we have a positive number.
	else if ( !Decrease )
	{
		// If we are trying to gain more mp than the max.
		if ( (MP + ByWhat) > MaxMP )
		{
			// Just heal to full.
			MP = MaxMP;
		}

		// Otherwise, we're just trying a regular heal, so just increase MP normally.
		else
		{
			MP += ByWhat;
		}
	}

	return true;
}

bool CPlayer::ChangeEXP(unsigned long ByWhat, bool Decrease)
{
	// Handle if we have a negative number.
	if ( Decrease )
	{
		// If we have enough EXP left to take away.
		if (EXP >= ByWhat)
		{
			EXP -= ByWhat;
		}

		// Else if we don't, just set the EXP to 0.
		else if (EXP < ByWhat)
		{
			EXP = 0;
		}
	}

	// Handle if we have a positive number.
	else if ( !Decrease )
	{
		EXP += ByWhat;

		// If we are trying to gain more EXP than a level up.
		if ( EXP >= EXPNeeded )
		{
			EXP -= EXPNeeded;
			Level++;
			EXPNeeded += 100; // Change this later to an array of EXP required for each level.
			return true;
		}
	}

	return false;
}

void CPlayer::DeathCheck()
{
	if (Dead == true)
	{
		Visible = false;
	}
	else if (Dead == false)
	{
		Visible = true;
	}
}